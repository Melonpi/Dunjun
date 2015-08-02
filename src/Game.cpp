#include <Dunjun/Config.hpp>

#include <Dunjun/Core.hpp>
#include <Dunjun/Window.hpp>
#include <Dunjun/ResourceHolders.hpp>

#include <Dunjun/Core/ContainerTypes.hpp>
#include <Dunjun/Core/Array.hpp>
#include <Dunjun/Core/Queue.hpp>
#include <Dunjun/Core/HashMap.hpp>
#include <Dunjun/Core/Murmur.hpp>
#include <Dunjun/Core/TempAllocator.hpp>
#include <Dunjun/Core/ConfigFile.hpp>

#include <Dunjun/World.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/RenderSystem.hpp>

#include <fstream>

#include <cassert>
#include <cstdlib>

using namespace Dunjun; // TODO(bill): DO NOT USE THIS IN PRODUCTION

namespace
{
GLOBAL const Time TimeStep  = seconds(1.0f / 60.0f);
GLOBAL const u32 FrameLimit = 288;

GLOBAL bool g_running = true;

GLOBAL Window g_window;

GLOBAL ModelAsset g_sprite;

GLOBAL World* g_world;

GLOBAL Texture g_defaultTexture;
GLOBAL Texture g_kittenTexture;
GLOBAL Texture g_stoneTexture;
GLOBAL Texture g_woodTexture;
GLOBAL Texture g_terrainTexture;
GLOBAL Texture g_brickTexture;

GLOBAL Texture g_defaultNormalTexture;
GLOBAL Texture g_brickNormalTexture;

GLOBAL Material g_kittenMaterial;
GLOBAL Material g_terrainMaterial;
GLOBAL Material g_stoneMaterial;
GLOBAL Material g_woodMaterial;
GLOBAL Material g_brickMaterial;
} // namespace (anonymous)

INTERNAL void glInit()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

INTERNAL void loadShaders()
{
	g_shaderHolder.insertFromFile("texPass",                     //
	                              "texPass.vert.glsl",           //
	                              "texPass.frag.glsl");          //
	g_shaderHolder.insertFromFile("geometryPass",                //
	                              "geometryPass.vert.glsl",      //
	                              "geometryPass.frag.glsl");     //
	g_shaderHolder.insertFromFile("ambientLight",                //
	                              "lightPass.vert.glsl",         //
	                              "ambientLight.frag.glsl");     //
	g_shaderHolder.insertFromFile("pointLight",                  //
	                              "lightPass.vert.glsl",         //
	                              "pointLight.frag.glsl");       //
	g_shaderHolder.insertFromFile("directionalLight",            //
	                              "lightPass.vert.glsl",         //
	                              "directionalLight.frag.glsl"); //
	g_shaderHolder.insertFromFile("spotLight",                   //
	                              "lightPass.vert.glsl",         //
	                              "spotLight.frag.glsl");        //
	g_shaderHolder.insertFromFile("out",                         //
	                              "lightPass.vert.glsl",         //
	                              "out.frag.glsl");              //
}

INTERNAL void loadMaterials()
{
	g_defaultTexture = loadTextureFromFile("data/textures/default.png");
	g_kittenTexture  = loadTextureFromFile("data/textures/kitten.jpg");
	g_stoneTexture = loadTextureFromFile("data/textures/stone.png", //
	                                     TextureFilter::Nearest);
	g_woodTexture = loadTextureFromFile("data/textures/wood.png", //
	                                    TextureFilter::Nearest);
	g_terrainTexture = loadTextureFromFile("data/textures/terrain.png",
	                                       TextureFilter::Nearest);

	g_brickTexture = loadTextureFromFile("data/textures/bricks.jpg");
	g_brickNormalTexture =
	    loadTextureFromFile("data/textures/bricks_normal.png");
	g_defaultNormalTexture =
	    loadTextureFromFile("data/textures/default_normal.jpg");

	g_kittenMaterial            = Material{};
	g_kittenMaterial.diffuseMap = &g_kittenTexture;
	g_kittenMaterial.normalMap  = &g_defaultNormalTexture;

	g_terrainMaterial            = Material{};
	g_terrainMaterial.diffuseMap = &g_terrainTexture;
	g_terrainMaterial.normalMap  = &g_defaultNormalTexture;

	g_stoneMaterial            = Material{};
	g_stoneMaterial.diffuseMap = &g_stoneTexture;
	g_stoneMaterial.normalMap  = &g_defaultNormalTexture;

	g_woodMaterial            = Material{};
	g_woodMaterial.diffuseMap = &g_woodTexture;
	g_woodMaterial.normalMap  = &g_defaultNormalTexture;

	g_brickMaterial = Material{};
	g_brickMaterial.diffuseMap = &g_brickTexture;
	g_brickMaterial.normalMap = &g_brickNormalTexture;
}

INTERNAL void loadSpriteAsset()
{
	{
		MeshData meshData{defaultAllocator()};
		meshData.drawType = DrawType::Triangles;

		reserve(meshData.vertices, 4);
		append(meshData.vertices, Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, +0.5f, 0.0f}, {1.0f, 1.0f}});
		append(meshData.vertices, Vertex{{-0.5f, +0.5f, 0.0f}, {0.0f, 1.0f}});

		reserve(meshData.indices, 6);
		meshData.addFace(0, 1, 2).addFace(2, 3, 0);
		meshData.generateNormals();

		g_meshHolder.insert(
		    "sprite", std::unique_ptr<Mesh>(new Mesh(generateMesh(meshData))));
	}
	{
		MeshData meshData{defaultAllocator()};
		meshData.drawType = DrawType::Triangles;

		reserve(meshData.vertices, 4);
		append(meshData.vertices, Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, -0.5f, 0.0f}, {2.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, +0.5f, 0.0f}, {2.0f, 2.0f}});
		append(meshData.vertices, Vertex{{-0.5f, +0.5f, 0.0f}, {0.0f, 2.0f}});

		reserve(meshData.indices, 6);
		meshData.addFace(0, 1, 2).addFace(2, 3, 0);
		meshData.generateNormals();

		g_meshHolder.insert(
		    "wall", std::unique_ptr<Mesh>(new Mesh(generateMesh(meshData))));
	}
	{
		MeshData meshData{defaultAllocator()};
		meshData.drawType = DrawType::Triangles;

		reserve(meshData.vertices, 4);
		append(meshData.vertices, Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+1.0f, +1.0f, 0.0f}, {1.0f, 1.0f}});
		append(meshData.vertices, Vertex{{-1.0f, +1.0f, 0.0f}, {0.0f, 1.0f}});

		reserve(meshData.indices, 6);
		meshData.addFace(0, 1, 2).addFace(2, 3, 0);
		meshData.generateNormals();

		g_meshHolder.insert(
		    "quad", std::unique_ptr<Mesh>(new Mesh(generateMesh(meshData))));
	}
}

INTERNAL void update(Time dt)
{
	g_world->update(dt);

	SceneGraph& sg = g_world->sceneGraph;

	{
		auto node   = sg.getNodeId(0); // crate
		Vector3 pos = sg.getGlobalPosition(node);

		f32 wt = 3.0f * Time::now().asSeconds();
		f32 a  = 0.5f;
		pos.y  = a * Math::sin(Radian{wt});

		sg.setGlobalPosition(node, pos);
	}

	{
		auto node   = sg.getNodeId(g_world->player);
		Vector3 pos = sg.getLocalPosition(node);

		f32 wt = 1.0f * Time::now().asSeconds();
		f32 a  = 2.0f;
		pos.x  = a * Math::cos(Radian{wt});
		pos.z  = a * Math::sin(Radian{wt});

		sg.setLocalPosition(node, pos);
	}

	{
		Camera& c = g_world->camera;
		f32 wt = 0.25f * Time::now().asSeconds();
		f32 a  = 1.0f;
		c.transform.position.x = a * Math::sin(Radian{wt});
		c.transform.position.z = a * Math::cos(Radian{wt});
		c.transform.position.y = 0.1f;

		cameraLookAt(c, {0, 0, 0});
	}
}

INTERNAL void handleEvents()
{
	Event event;
	while (g_window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
		{
			g_window.close();
			exit(EXIT_SUCCESS);
			break;
		}
		case Event::Resized:
		{
			glViewport(0, 0, event.size.width, event.size.height);
			break;
		}
		case Event::ControllerConnected:
		{
			printf("Controller %d added\n", event.controller.index);
			break;
		}
		case Event::ControllerDisconnected:
		{
			printf("Controller %d removed\n", event.controller.index);
			break;
		}
		case Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case Input::Key::Escape:
			{
				g_window.close();
				exit(EXIT_SUCCESS);
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}

		g_world->handleEvent(event);
	}
}

INTERNAL void render()
{
	g_world->renderSystem.fbSize.x = g_window.getSize().width;
	g_world->renderSystem.fbSize.y = g_window.getSize().height;

	glViewport(0, 0, g_window.getSize().width, g_window.getSize().height);
	glClearColor(0.5, 0.69, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_world->render();
}

INTERNAL void initWorld()
{
	auto& es = g_world->entitySystem;
	auto& sg = g_world->sceneGraph;
	auto& rs = g_world->renderSystem;

	// Add entities
	auto crate      = es.addEntity(Component_Name | Component_Render);
	g_world->player = es.addEntity(Component_Name | Component_Render);
	auto player     = g_world->player;
	auto surface    = es.addEntity(Component_Render);
	auto wall0      = es.addEntity(Component_Render);
	auto wall1      = es.addEntity(Component_Render);
	auto wall2      = es.addEntity(Component_Render);

	// Name entities
	{
		auto& names   = g_world->names;
		names[crate]  = "crate";
		names[player] = "Bob";
	}
	// Add entities to scene graph and set transforms
	{
		Transform crateTransform;
		auto crateNode  = sg.addNode(crate, crateTransform);
		auto playerNode = sg.addNode(player, Transform{});
		sg.linkNodes(crateNode, playerNode);

		Transform surfaceTransform;
		surfaceTransform.position    = {0, -1, 0};
		surfaceTransform.orientation = angleAxis(Degree{-90}, {1, 0, 0});
		surfaceTransform.scale = {8, 8, 8};
		sg.addNode(surface, surfaceTransform);

		Transform wallTransform;
		wallTransform.position = {0, 0, -4};
		wallTransform.scale = {8, 8, 8};
		sg.addNode(wall0, wallTransform);

		wallTransform.position    = {-4, 0, 0};
		wallTransform.orientation = angleAxis(Degree{90}, {0, 1, 0});
		sg.addNode(wall1, wallTransform);

		wallTransform.position    = {+4, 0, 0};
		wallTransform.orientation = angleAxis(Degree{-90}, {0, 1, 0});
		sg.addNode(wall2, wallTransform);
	}
	// Add render components to entities
	{
		rs.addComponent(crate, {g_meshHolder.get("sprite"), g_kittenMaterial});
		rs.addComponent(player, {g_meshHolder.get("sprite"), g_kittenMaterial});
		rs.addComponent(surface, {g_meshHolder.get("wall"), g_brickMaterial});
		rs.addComponent(wall0, {g_meshHolder.get("wall"), g_stoneMaterial});
		rs.addComponent(wall1, {g_meshHolder.get("wall"), g_stoneMaterial});
		rs.addComponent(wall2, {g_meshHolder.get("wall"), g_stoneMaterial});
	}
	// Add lights
	{
		DirectionalLight dlight;
		dlight.direction = normalize(Vector3{0, -1, -1});
		// append(rs.directionalLights, dlight);

		auto r = Random{1337};
		for (int i = 0; i < 10; i++)
		{
			PointLight pl;
			pl.position.x = r.getFloat(-4, 4);
			pl.position.y = r.getFloat(0.1f, 1.0f);
			pl.position.z = r.getFloat(-4, 4);

			pl.intensity = r.getFloat(0.5f, 2.0f);
			pl.color.r   = r.getInt(50, 255);
			pl.color.g   = r.getInt(50, 255);
			pl.color.b   = r.getInt(50, 255);
			pl.color.a   = 255;
			pl.range     = calculateLightRange(pl);

			append(rs.pointLights, pl);
		}
	}

	glInit();
}

INTERNAL void shutdown()
{
	// NOTE(bill): There is no real need to call this as the OS should do this
	defaultAllocator().makeDelete(g_world);
	Input::shutdown();
	g_window.close();
	SDL_Quit();
	Memory::shutdown();

	exit(EXIT_SUCCESS);
}

INTERNAL void init(int /*argCount*/, char** /*args*/)
{
	Memory::init();

	u32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
	               SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC;

	if (SDL_Init(sdlFlags) != 0)
	{
		std::cerr << "SDL Failed to initialize. Error: " << SDL_GetError()
		          << "\n";
		exit(EXIT_FAILURE);
	}

	auto cf = loadConfigFileFromFile("data/settings.fred");

	{
		VideoMode vm    = {};
		vm.width        = getUintFromConfigFile(cf, "Window.width", 854);
		vm.height       = getUintFromConfigFile(cf, "Window.height", 480);
		vm.bitsPerPixel = getUintFromConfigFile(cf, "Window.bitsPerPixel", 24);
		printf("%u x %u x %u\n", vm.width, vm.height, vm.bitsPerPixel);

		String title = getStringFromConfigFile(cf, "Window.title", "Dunjun");

		g_window.create(vm, title);
		// g_window.setFramerateLimit(FrameLimit);
		// g_window.setVerticalSyncEnabled(true);
	}

	glewInit();

	// Initial OpenGL settings
	glInit();

	Input::init();
	Input::setCursorPosition(g_window, {0, 0});

	loadShaders();
	loadMaterials();
	loadSpriteAsset();

	g_world = defaultAllocator().makeNew<World>();

	initWorld();
}

// NOTE(bill): Ignore SDL main
#undef main
int main(int argCount, char** args)
{
	using namespace Dunjun;

	init(argCount, args);

	// NOTE(bill): Only enable if you need to destroy everything
	// E.g. check for memory leaks else where in the code
	// defer(shutdown());

	TickCounter tc;

	Time accumulator;
	Time prevTime = Time::now();

	usize frames = 0;

	while (g_running)
	{
		Time currentTime = Time::now();
		Time dt          = currentTime - prevTime;
		prevTime         = currentTime;
		accumulator += dt;

		if (accumulator > milliseconds(1200)) // remove loop of death
			accumulator = milliseconds(1200);

		handleEvents();

		while (accumulator >= TimeStep)
		{
			accumulator -= TimeStep;

			handleEvents();
			update(TimeStep);
		}

		// Set Window Title
		if (tc.update(milliseconds(500)))
		{
			g_window.setTitle(stringFormat("Dunjun - %.3f ms - %d fps",
			                               1000.0f / tc.tickRate,
			                               (u32)tc.tickRate));
		}

		render();

		g_window.display();
	}

	return 0;
}
