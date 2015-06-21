#include <Dunjun/World.hpp>

#include <Dunjun/Math/Random.hpp>

#include <Dunjun/Window/Input.hpp>

#include <Dunjun/Scene/MeshRenderer.hpp>
#include <Dunjun/Scene/FaceCamera.hpp>

namespace Dunjun
{
World::World()
{
	renderer.world = this;
}

World::~World() {}

void World::init(Context context_)
{
	context = context_;

	{
		auto player = make_unique<SceneNode>();

		player->name = "player";
		player->transform.position = {2, 0.5, 2};
		player->transform.orientation = angleAxis(Degree{45}, {0, 1, 0});
		//player->addComponent<FaceCamera>(mainCamera);
		player->addComponent<MeshRenderer>(
		    &context.meshHolder->get("sprite"),
		    &context.materialHolder->get("cat"));


		this->player = player.get();

		sceneGraph.attachChild(std::move(player));
	}

	{
		auto level = make_unique<Level>();

		level->material = &context.materialHolder->get("terrain");
		level->generate();

		this->level = level.get();

		sceneGraph.attachChild(std::move(level));
	}

	Random random{1};
	for (int i{0}; i < 20; i++)
	{
		PointLight light;
		f32 radius{random.getFloat(0.1f, 16.0f)};
		Radian angle{random.getFloat(0, Math::Tau)};
		light.position.x = 4.0f + radius * Math::cos(angle);
		light.position.y = random.getFloat(0.5, 2.5);
		light.position.z = 4.0f + radius * Math::sin(angle);

		light.intensity = 1.0;

		light.color.r = random.getInt(50, 255);
		light.color.g = random.getInt(50, 255);
		light.color.b = random.getInt(50, 255);

		pointLights.emplace_back(light);
	}

	{
		DirectionalLight light;
		light.color = Color{255, 255, 250};
		light.direction = Vector3{-1, -1, 0.5};
		light.intensity = 0.1f;

		directionalLights.emplace_back(light);
	}

	{
		SpotLight light;
		light.color = Color{255, 255, 250};
		light.direction = {0, -1, 0};
		light.position = {4, 1.5f, 4};
		light.intensity = 2.0f;
		light.coneAngle = Degree{50};

		spotLights.emplace_back(light);
	}

	{
		// Init Camera
		playerCamera.transform.position = {5, 2, 5};
		playerCamera.transform.orientation =
		    angleAxis(Degree{45}, {0, 1, 0}) *
		    angleAxis(Degree{-30}, {1, 0, 0});

		playerCamera.fieldOfView = Degree{50.0f};
		playerCamera.orthoScale = 8;

		mainCamera = playerCamera;

		playerCamera.projectionType = ProjectionType::Orthographic;
	}

	currentCamera = &mainCamera;

	sceneGraph.init();
}

void World::update(Time dt)
{
	sceneGraph.update(dt);

	f32 camVel{10.0f};
	{
		if (Input::isControllerPresent(0))
		{
			f32 ltsX{Input::getControllerAxis(0, Input::ControllerAxis::LeftX)};
			f32 ltsY{Input::getControllerAxis(0, Input::ControllerAxis::LeftY)};

			f32 rtsX{
			    Input::getControllerAxis(0, Input::ControllerAxis::RightX)};
			f32 rtsY{
			    Input::getControllerAxis(0, Input::ControllerAxis::RightY)};

			const f32 lookSensitivity{2.0f};
			const f32 deadZone{0.21f};

			Vector2 rts{rtsX, rtsY};
			if (Math::abs(rts.x) < deadZone)
				rts.x = 0;
			if (Math::abs(rts.y) < deadZone)
				rts.y = 0;

			mainCamera.offsetOrientation(
			    lookSensitivity * Radian{-rts.x * dt.asSeconds()},
			    lookSensitivity * Radian{-rts.y * dt.asSeconds()});

			Vector2 lts{ltsX, ltsY};

			if (Math::abs(lts.x) < deadZone)
				lts.x = 0;
			if (Math::abs(lts.y) < deadZone)
				lts.y = 0;

			if (length(lts) > 1.0f)
				lts = normalize(lts);
			Vector3 velDir{0, 0, 0};

			Vector3 forward = mainCamera.forward();
			forward.y = 0;
			forward = normalize(forward);
			velDir += lts.x * mainCamera.right();
			velDir += lts.y * forward;

			if (Input::isControllerButtonPressed(
			        0, Input::ControllerButton::RightShoulder))
				velDir.y += 1;
			if (Input::isControllerButtonPressed(
			        0, Input::ControllerButton::LeftShoulder))
				velDir.y -= 1;

			if (Input::isControllerButtonPressed(
			        0, Input::ControllerButton::DpadUp))
			{
				Vector3 f = mainCamera.forward();
				f.y = 0;
				f = normalize(f);
				velDir += f;
			}
			if (Input::isControllerButtonPressed(
			        0, Input::ControllerButton::DpadDown))
			{
				Vector3 b = mainCamera.backward();
				b.y = 0;
				b = normalize(b);
				velDir += b;
			}

			if (Input::isControllerButtonPressed(
			        0, Input::ControllerButton::DpadLeft))
			{
				Vector3 l = mainCamera.left();
				l.y = 0;
				l = normalize(l);
				velDir += l;
			}
			if (Input::isControllerButtonPressed(
			        0, Input::ControllerButton::DpadRight))
			{
				Vector3 r = mainCamera.right();
				r.y = 0;
				r = normalize(r);
				velDir += r;
			}

			if (length(velDir) > 1.0f)
				velDir = normalize(velDir);

			mainCamera.transform.position += camVel * velDir * dt.asSeconds();

			// Vibrate
			if (Input::isControllerButtonPressed(0, Input::ControllerButton::A))
			{
				printf("HERE\n");
				Input::setControllerVibration(0, 0.5f);
			}
			else
			{
				Input::setControllerVibration(0, 0.0f);
			}
		}
	}

	f32 playerVel{4.0f};
	{
		Vector3 velDir = {0, 0, 0};

		if (Input::isKeyPressed(Input::Key::Up))
			velDir += {0, 0, -1};
		if (Input::isKeyPressed(Input::Key::Down))
			velDir += {0, 0, +1};

		if (Input::isKeyPressed(Input::Key::Left))
			velDir += {-1, 0, 0};
		if (Input::isKeyPressed(Input::Key::Right))
			velDir += {+1, 0, 0};

		if (Input::isKeyPressed(Input::Key::RShift))
			velDir += {0, +1, 0};
		if (Input::isKeyPressed(Input::Key::RControl))
			velDir += {0, -1, 0};

		if (length(velDir) > 0)
			velDir = normalize(velDir);

		player->transform.position += playerVel * velDir * dt.asSeconds();
	}

	playerCamera.transform.position.x =
	    Math::lerp(playerCamera.transform.position.x,
	               player->transform.position.x,
	               10.0f * dt.asSeconds());
	playerCamera.transform.position.z =
	    Math::lerp(playerCamera.transform.position.z,
	               player->transform.position.z,
	               10.0f * dt.asSeconds());

	// g_camera.transform.position.x = player.transform.position.x;
	f32 aspectRatio{context.window->getSize().aspectRatio()};
	if (aspectRatio && context.window->getSize().height > 0)
	{
		playerCamera.viewportAspectRatio = aspectRatio;
		mainCamera.viewportAspectRatio = aspectRatio;
	}

	if (Input::isKeyPressed(Input::Key::Num1))
		currentCamera = &playerCamera;
	else if (Input::isKeyPressed(Input::Key::Num2))
	{
		mainCamera.transform = playerCamera.transform;
		currentCamera = &mainCamera;
	}

#if 1
	// TODO(bill): Make bounding boxes for SceneNodes and implement this
	// in SceneRenderer
	for (auto& room : level->rooms)
	{
		Vector3 roomPos = room->transform.position;
		roomPos.x += room->size.x / 2;
		roomPos.z += room->size.y / 2;
		const Vector3 playerPos = mainCamera.transform.position;

		const Vector3 dp = roomPos - playerPos;

		const f32 dist{length(dp)};


		room->enabled = false;

		// Distance Culling
		if (dist < mainCamera.farPlane)
		{
			const Vector3 f = mainCamera.forward();

			f32 cosTheta{dot(f, normalize(dp))};

			Radian theta{Math::acos(cosTheta)};

			// Cone/(bad) Frustum Culling
			if (Math::abs(theta) <= 2.0f * mainCamera.fieldOfView ||
			    dist < 10)
				room->enabled = true;
		}
	}
#endif
}

void World::handleEvent(const Event& event)
{
	sceneGraph.handleEvent(event);
}

void World::render()
{
	renderer.fbWidth = context.window->getSize().width;
	renderer.fbHeight = context.window->getSize().height;

	renderer.render();

	glViewport(0, 0, renderer.fbWidth, renderer.fbHeight);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	{
		ShaderProgram& shaders = context.shaderHolder->get("texPass");
		shaders.use();

		shaders.setUniform("u_scale", Vector3{1, 1, 1});
		shaders.setUniform("u_tex", 0);
		Texture::bind(&renderer.outTexture.colorTexture, 0);
		// Texture::bind(&renderer.lightingTexture.colorTexture, 0);

		renderer.draw(&context.meshHolder->get("quad"));

		shaders.stopUsing();
	}
}
} // namespace Dunjun
