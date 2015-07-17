#include <Dunjun/RenderSystem.hpp>

#include <Dunjun/ResourceHolders.hpp>

#include <Dunjun/Core/Array.hpp>
#include <Dunjun/Core/HashMap.hpp>

namespace Dunjun
{
RenderSystem::RenderSystem(Allocator& a, SceneGraph& sg)
: allocator{a}
, data{}
, map{a}
, sceneGraph{sg}

, gbuffer{}
, lbuffer{}

, ambientColor({222, 227, 234, 255})
, ambientIntensity{0.02f}

, directionalLights{allocator}
, pointLights{allocator}
, spotLights{allocator}

, camera{nullptr}
, currentTextures{}
{
	allocate(16);
}

RenderSystem::~RenderSystem()
{
	allocator.deallocate(data.buffer);

	destroyGBuffer(gbuffer);
	destroyRenderTexture(lbuffer);
	destroyRenderTexture(outTexture);
}

void RenderSystem::allocate(u32 capacity)
{
	if (capacity <= data.length)
		return;

	const usize bytes = capacity * (sizeof(EntityId) + sizeof(RenderComponent));

	InstanceData newData;
	newData.length   = data.length;
	newData.capacity = capacity;
	newData.buffer   = allocator.allocate(bytes);

	newData.entityId  = (EntityId*)(newData.buffer);
	newData.component = (RenderComponent*)(newData.entityId + capacity);

	memcpy(newData.entityId, data.entityId, data.length * sizeof(EntityId));
	memcpy(newData.component,
	       data.component,
	       data.length * sizeof(RenderComponent));
	allocator.deallocate(data.buffer);
	data = newData;
}

RenderSystem::ComponentId
RenderSystem::addComponent(EntityId id, const RenderComponent& component)
{
	if (data.capacity == data.length) // grow
		allocate(2 * data.length + 1);

	const ComponentId last = data.length;

	data.entityId[last]  = id;
	data.component[last] = component;

	set(map, id, last);

	data.length++;

	return last;
}

void RenderSystem::removeComponent(ComponentId id)
{
	const ComponentId last    = data.length - 1;
	const EntityId entity     = data.entityId[id];
	const EntityId lastEntity = data.entityId[last];

	data.entityId[id]  = data.entityId[last];
	data.component[id] = data.component[last];

	set(map, lastEntity, id);
	set(map, entity, EmptyComponentId);

	data.length--;
}

RenderSystem::ComponentId RenderSystem::getComponentId(EntityId id)
{
	return get(map, id, EmptyComponentId);
}

bool RenderSystem::isValid(ComponentId id) const
{
	return id != EmptyComponentId;
}

void RenderSystem::resetAllPointers()
{
	camera = nullptr;
	for (u32 i = 0; i < 32; i++)
		setTexture(nullptr, i);
}

void RenderSystem::render()
{
	if (!camera)
	{
		fprintf(stderr, "No Camera* for RenderSystem\n");
		return;
	}

	createGBuffer(gbuffer, fbSize.x, fbSize.y);

	geometryPass();
	lightPass();
	outPass();
}

void RenderSystem::geometryPass()
{
	glEnable(GL_TEXTURE_2D);

	ShaderProgram& shaders = g_shaderHolder.get("geometryPass");

	bindGBuffer(&gbuffer);
	defer(bindGBuffer(nullptr));
	{
		glClearColor(0, 0, 0, 0);
		glViewport(0, 0, gbuffer.width, gbuffer.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders.use();
		defer(shaders.stopUsing());
		{
			shaders.setUniform("u_camera", cameraMatrix(*camera));
			shaders.setUniform("u_cameraPosition", camera->transform.position);

			for (u32 i = 0; i < data.length; i++)
			{
				const EntityId entityId          = data.entityId[i];
				const RenderComponent& component = data.component[i];
				const Material& material         = component.material;

				shaders.setUniform("u_material.diffuseColor",
				                   material.diffuseColor);
				shaders.setUniform("u_material.diffuseMap", (s32)0);
				setTexture(material.diffuseMap, 0);

				// TODO(bill): improve performance - set at render time
				auto node = sceneGraph.getNodeId(entityId);
				shaders.setUniform("u_transform",
				                   sceneGraph.getGlobalTransform(node));

				drawMesh(component.mesh);
			}
		}
	}
}

void RenderSystem::lightPass()
{
	createRenderTexture(
	    lbuffer, gbuffer.width, gbuffer.height, RenderTexture::Lighting);

	bindTexture(&gbuffer.textures[GBuffer::Diffuse], 0);
	bindTexture(&gbuffer.textures[GBuffer::Specular], 1);
	bindTexture(&gbuffer.textures[GBuffer::Normal], 2);
	bindTexture(&gbuffer.textures[GBuffer::Depth], 3);

	bindRenderTexture(&lbuffer);
	defer(bindRenderTexture(nullptr));
	{
		glClearColor(0, 0, 0, 0);
		glViewport(0, 0, lbuffer.width, lbuffer.height);
		glClear(GL_COLOR_BUFFER_BIT);

		glDepthMask(false);
		defer(glDepthMask(true));

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		defer(glDisable(GL_BLEND));
		glBlendFunc(GL_ONE, GL_ONE);

		// Ambient Light
		{
			auto& shaders = g_shaderHolder.get("ambientLight");

			shaders.use();
			defer(shaders.stopUsing());

			Vector3 intensities;
			intensities.r = ambientColor.r * ambientIntensity;
			intensities.g = ambientColor.g * ambientIntensity;
			intensities.b = ambientColor.b * ambientIntensity;
			intensities /= 255.0f;

			shaders.setUniform("u_light.intensities", intensities);

			drawMesh(g_meshHolder.get("quad"));
		}
		// Directional Lights
		{
			auto& shaders = g_shaderHolder.get("directionalLight");

			shaders.use();
			defer(shaders.stopUsing());

			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal", 2);

			for (const auto& light : directionalLights)
			{
				Vector3 lightIntensities;

				lightIntensities.r = light.color.r / 255.0f;
				lightIntensities.g = light.color.g / 255.0f;
				lightIntensities.b = light.color.b / 255.0f;
				lightIntensities *= light.intensity;

				shaders.setUniform("u_light.base.intensities",
				                   lightIntensities);
				shaders.setUniform("u_light.direction",
				                   normalize(light.direction));

				drawMesh(g_meshHolder.get("quad"));
			}
		}
		// Point Lights
		{
			auto& shaders = g_shaderHolder.get("pointLight");

			shaders.use();
			defer(shaders.stopUsing());

			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal", 2);
			shaders.setUniform("u_depth", 3);

			shaders.setUniform("u_cameraInverse",
			                   inverse(cameraMatrix(*camera)));

			for (const PointLight& light : pointLights)
			{
				light.range = calculateLightRange(light);

				Vector3 lightIntensities;

				lightIntensities.r = light.color.r / 255.0f;
				lightIntensities.g = light.color.g / 255.0f;
				lightIntensities.b = light.color.b / 255.0f;
				lightIntensities *= light.intensity;

				shaders.setUniform("u_light.base.intensities",
				                   lightIntensities);
				shaders.setUniform("u_light.position", light.position);

				shaders.setUniform("u_light.attenuation.constant",
				                   light.attenuation.constant);
				shaders.setUniform("u_light.attenuation.linear",
				                   light.attenuation.linear);
				shaders.setUniform("u_light.attenuation.quadratic",
				                   light.attenuation.quadratic);

				shaders.setUniform("u_light.range", light.range);

				drawMesh(g_meshHolder.get("quad"));
			}
		}
		// Spot Lights
		{
			auto& shaders = g_shaderHolder.get("spotLight");

			shaders.use();
			defer(shaders.stopUsing());

			shaders.setUniform("u_diffuse", 0);
			shaders.setUniform("u_specular", 1);
			shaders.setUniform("u_normal", 2);
			shaders.setUniform("u_depth", 3);

			shaders.setUniform("u_cameraInverse",
			                   inverse(cameraMatrix(*camera)));

			for (const SpotLight& light : spotLights)
			{
				light.range = calculateLightRange(light);

				Vector3 lightIntensities;

				lightIntensities.r = light.color.r / 255.0f;
				lightIntensities.g = light.color.g / 255.0f;
				lightIntensities.b = light.color.b / 255.0f;
				lightIntensities *= light.intensity;

				shaders.setUniform("u_light.point.base.intensities",
				                   lightIntensities);
				shaders.setUniform("u_light.point.position", light.position);

				shaders.setUniform("u_light.point.attenuation.constant",
				                   light.attenuation.constant);
				shaders.setUniform("u_light.point.attenuation.linear",
				                   light.attenuation.linear);
				shaders.setUniform("u_light.point.attenuation.quadratic",
				                   light.attenuation.quadratic);

				shaders.setUniform("u_light.point.range", light.range);

				shaders.setUniform("u_light.direction", light.direction);
				shaders.setUniform("u_light.coneAngle", (f32)(light.coneAngle));

				drawMesh(g_meshHolder.get("quad"));
			}
		}
	}
}

void RenderSystem::outPass()
{
	createRenderTexture(
	    outTexture, gbuffer.width, gbuffer.height, RenderTexture::Color);

	bindTexture(nullptr, 0);
	bindTexture(nullptr, 1);
	bindTexture(&gbuffer.textures[GBuffer::Diffuse], 0);
	bindTexture(&lbuffer.colorTexture, 1);

	bindRenderTexture(&outTexture);
	defer(bindRenderTexture(nullptr));
	{
		glClearColor(1, 1, 1, 1);
		glViewport(0, 0, outTexture.width, outTexture.height);
		glClear(GL_COLOR_BUFFER_BIT);

		auto& shaders = g_shaderHolder.get("out");

		shaders.use();
		defer(shaders.stopUsing());

		shaders.setUniform("u_diffuse", (s32)0);
		shaders.setUniform("u_lighting", (s32)1);

		drawMesh(g_meshHolder.get("quad"));
	}
}

bool RenderSystem::setTexture(const Texture* texture, u32 position)
{
	if (texture != currentTextures[position])
	{
		currentTextures[position] = texture;

		bindTexture(currentTextures[position], position);

		return true;
	}

	return false;
}
} // namespace Dunjun
