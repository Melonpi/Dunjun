#include <Dunjun/Scene/SceneRenderer.hpp>

#include <Dunjun/Scene/SceneNode.hpp>
#include <Dunjun/Scene/MeshRenderer.hpp>
#include <Dunjun/World.hpp>

#include <string>

#include <algorithm>
#include <iterator>

namespace Dunjun
{
SceneRenderer& SceneRenderer::reset()
{
	if (currentShaders)
		currentShaders->stopUsing();
	currentShaders = nullptr;

	currentTexture = nullptr;
	Texture::bind(nullptr, 0);

	currentMaterial = nullptr;

	return *this;
}

SceneRenderer& SceneRenderer::clearAll()
{
	modelInstances.clear();

	return *this;
}

SceneRenderer& SceneRenderer::addSceneGraph(const SceneNode& node, Transform t)
{
	node.draw(*this, t);

	return *this;
}

void SceneRenderer::draw(const Mesh* mesh) const
{
	if (mesh)
		mesh->draw();
}

void SceneRenderer::addModelInstance(const MeshRenderer& meshRenderer,
                                     Transform t)
{
	ModelInstance mi;
	mi.meshRenderer = &meshRenderer;
	mi.transform = t;
	modelInstances.push_back(mi);
}

void SceneRenderer::render()
{
	reset().clearAll();

	addSceneGraph(world->sceneGraph);
	camera = world->currentCamera;

	gBuffer.create(fbWidth, fbHeight);

	geometryPass().lightPass().outPass();
}

SceneRenderer& SceneRenderer::geometryPass()
{
	// TODO(bill): Sort by mesh? - Instancing?
	std::sort(std::begin(modelInstances),
			  std::end(modelInstances),
	          [](const ModelInstance& a, const ModelInstance& b) -> bool
	          {
		          const auto* A = a.meshRenderer->material;
		          const auto* B = b.meshRenderer->material;

		          if (A != B && A && B)
		          {
			          if (A->shaders == B->shaders)
				          return A->diffuseMap < B->diffuseMap;
			          return A->shaders < B->shaders;
		          }
		          return false;
		      });

	auto& shaders = g_shaderHolder.get("geometryPass");

	GBuffer::bind(&gBuffer);
	{
		glViewport(0, 0, gBuffer.width, gBuffer.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders.use();

		shaders.setUniform("u_camera", camera->getMatrix());
		shaders.setUniform("u_cameraPosition", camera->transform.position);
		for (const auto& inst : modelInstances)
		{
			if (inst.meshRenderer->material == nullptr)
				continue;

			{
				shaders.setUniform("u_material.diffuseMap", (int)0);
				shaders.setUniform("u_material.diffuseColor",
				                   inst.meshRenderer->material->diffuseColor);
				shaders.setUniform("u_material.specularColor",
				                   inst.meshRenderer->material->specularColor);
				shaders.setUniform("u_material.specularExponent", inst.meshRenderer->material->specularExponent);
			}
			setTexture(inst.meshRenderer->material->diffuseMap, 0);

			shaders.setUniform("u_transform", inst.transform);

			draw(inst.meshRenderer->mesh);
		}

		glFlush();
	}
	GBuffer::bind(nullptr);

	return *this;
}

SceneRenderer& SceneRenderer::lightPass()
{
	lBuffer.create(gBuffer.width, gBuffer.height, RenderTexture::Lighting);

	Texture::bind(&gBuffer.textures[GBuffer::Diffuse],  0);
	Texture::bind(&gBuffer.textures[GBuffer::Specular], 1);
	Texture::bind(&gBuffer.textures[GBuffer::Normal],   2);
	Texture::bind(&gBuffer.textures[GBuffer::Depth],    3);

	RenderTexture::bind(&lBuffer);
	{
		glClearColor(0, 0, 0, 0);
		glViewport(0, 0, lBuffer.width, lBuffer.height);
		glClear(GL_COLOR_BUFFER_BIT);

		glDepthMask(false);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		// Ambient Light
		{
			auto& shaders = g_shaderHolder.get("ambientLight");

			shaders.use();

			Vector3 intensities{};
			intensities.r = ambientColor.r * ambientIntensity;
			intensities.g = ambientColor.g * ambientIntensity;
			intensities.b = ambientColor.b * ambientIntensity;
			intensities /= 255.0f;

			shaders.setUniform("u_light.intensities", intensities);

			draw(&g_meshHolder.get("quad"));

			shaders.stopUsing();
		}
		// Directional Lights
		{
			auto& shaders = g_shaderHolder.get("directionalLight");

			shaders.use();
			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal", 2);

			for (const auto& light : world->directionalLights)
			{
				Vector3 lightIntensities;

				lightIntensities.r = light.color.r / 255.0f;
				lightIntensities.g = light.color.g / 255.0f;
				lightIntensities.b = light.color.b / 255.0f;
				lightIntensities *= light.intensity;

				shaders.setUniform("u_light.base.intensities", lightIntensities);
				shaders.setUniform("u_light.direction", normalize(light.direction));

				draw(&g_meshHolder.get("quad"));
			}
			shaders.stopUsing();
		}
		// Point Lights
		{
			auto& shaders = g_shaderHolder.get("pointLight");

			shaders.use();
			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal", 2);
			shaders.setUniform("u_depth", 3);

			shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

			for (const PointLight& light : world->pointLights)
			{
				light.range = calculateLightRange(light);

				Vector3 lightIntensities;

				lightIntensities.r = light.color.r / 255.0f;
				lightIntensities.g = light.color.g / 255.0f;
				lightIntensities.b = light.color.b / 255.0f;
				lightIntensities *= light.intensity;

				shaders.setUniform("u_light.base.intensities", lightIntensities);
				shaders.setUniform("u_light.position", light.position);

				shaders.setUniform("u_light.attenuation.constant",
								   light.attenuation.constant);
				shaders.setUniform("u_light.attenuation.linear",
								   light.attenuation.linear);
				shaders.setUniform("u_light.attenuation.quadratic",
								   light.attenuation.quadratic);

				shaders.setUniform("u_light.range", light.range);

				draw(&g_meshHolder.get("quad"));
			}

			shaders.stopUsing();
		}
		// Spot Lights
		{
			auto& shaders = g_shaderHolder.get("spotLight");

			shaders.use();
			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal", 2);
			shaders.setUniform("u_depth", 3);

			shaders.setUniform("u_cameraInverse", inverse(camera->getMatrix()));

			for (const SpotLight& light : world->spotLights)
			{
				light.range = calculateLightRange(light);

				Vector3 lightIntensities;

				lightIntensities.r = light.color.r / 255.0f;
				lightIntensities.g = light.color.g / 255.0f;
				lightIntensities.b = light.color.b / 255.0f;
				lightIntensities *= light.intensity;

				shaders.setUniform("u_light.point.base.intensities", lightIntensities);
				shaders.setUniform("u_light.point.position", light.position);

				shaders.setUniform("u_light.point.attenuation.constant",
								   light.attenuation.constant);
				shaders.setUniform("u_light.point.attenuation.linear",
								   light.attenuation.linear);
				shaders.setUniform("u_light.point.attenuation.quadratic",
								   light.attenuation.quadratic);

				shaders.setUniform("u_light.point.range", light.range);

				shaders.setUniform("u_light.direction", light.direction);
				shaders.setUniform("u_light.coneAngle", static_cast<f32>(light.coneAngle));

				draw(&g_meshHolder.get("quad"));
			}

			shaders.stopUsing();
		}

		glDisable(GL_BLEND);
		glDepthMask(true);
	}
	RenderTexture::bind(nullptr);

	return *this;
}

SceneRenderer& SceneRenderer::outPass()
{
	outTexture.create(gBuffer.width, gBuffer.height, RenderTexture::Color);

	Texture::bind(&gBuffer.textures[GBuffer::Diffuse], 0);
	Texture::bind(&lBuffer.colorTexture, 1);

	RenderTexture::bind(&outTexture);
	{
		glClearColor(1, 1, 1, 1);
		glViewport(0, 0, outTexture.width, outTexture.height);
		glClear(GL_COLOR_BUFFER_BIT);

		auto& shaders = g_shaderHolder.get("out");

		shaders.use();
		shaders.setUniform("u_diffuse", 0);
		shaders.setUniform("u_lighting", 1);

		draw(&g_meshHolder.get("quad"));

		shaders.stopUsing();
	}
	RenderTexture::bind(nullptr);

	return *this;
}

bool SceneRenderer::setShaders(const ShaderProgram* shaders)
{
	if (shaders != currentShaders)
	{
		if (currentShaders)
			currentShaders->stopUsing();

		currentShaders = shaders;
		currentShaders->use();

		return true;
	}

	return false;
}

bool SceneRenderer::setTexture(const Texture* texture, u32 position)
{
	if (texture != currentTexture)
	{
		currentTexture = texture;

		Texture::bind(currentTexture, position);

		return true;
	}

	return false;
}
} // namespace Dunjun
