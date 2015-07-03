#include <Dunjun/Config.hpp>
#include <Dunjun/Game.hpp>

#include <Dunjun/System.hpp>
#include <Dunjun/Window.hpp>
#include <Dunjun/ResourceHolders.hpp>
#include <Dunjun/World.hpp>

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Dunjun/System/Containers.hpp>
#include <Dunjun/System/Array.hpp>
#include <Dunjun/System/Queue.hpp>
#include <Dunjun/System/HashMap.hpp>
#include <Dunjun/System/Murmur.hpp>
#include <Dunjun/System/TempAllocator.hpp>

namespace Dunjun
{
struct ModelInstance
{
	ModelAsset* asset;
	Transform transform;
};

namespace
{
GLOBAL const Time TimeStep  = seconds(1.0f / 60.0f);
GLOBAL const u32 FrameLimit = 288;

GLOBAL bool g_running = true;
} // namespace (anonymous)

GLOBAL Window g_window;

GLOBAL ModelAsset g_sprite;

GLOBAL World* g_world;

GLOBAL Texture g_defaultTexture;
GLOBAL Texture g_kittenTexture;
GLOBAL Texture g_stoneTexture;
GLOBAL Texture g_terrainTexture;


namespace Game
{
INTERNAL void loadShaders()
{
	g_shaderHolder.insertFromFile("texPass",           //
	                              "texPass.vert.glsl", //
	                              "texPass.frag.glsl");
	g_shaderHolder.insertFromFile("geometryPass",           //
	                              "geometryPass.vert.glsl", //
	                              "geometryPass.frag.glsl");
	g_shaderHolder.insertFromFile("ambientLight",        //
	                              "lightPass.vert.glsl", //
	                              "ambientLight.frag.glsl");
	g_shaderHolder.insertFromFile("pointLight",          //
	                              "lightPass.vert.glsl", //
	                              "pointLight.frag.glsl");
	g_shaderHolder.insertFromFile("directionalLight",    //
	                              "lightPass.vert.glsl", //
	                              "directionalLight.frag.glsl");
	g_shaderHolder.insertFromFile("spotLight",           //
	                              "lightPass.vert.glsl", //
	                              "spotLight.frag.glsl");
	g_shaderHolder.insertFromFile("out",                 //
	                              "lightPass.vert.glsl", //
	                              "out.frag.glsl");
}
INTERNAL void loadMaterials()
{
	g_defaultTexture = loadTextureFromFile("data/textures/default.png");
	g_kittenTexture = loadTextureFromFile("data/textures/kitten.jpg");
	g_stoneTexture = loadTextureFromFile("data/textures/stone.png");
	g_terrainTexture = loadTextureFromFile("data/textures/terrain.png",
										   TextureFilter::Nearest);

	{
		auto mat        = make_unique<Material>();
		mat->shaders    = &g_shaderHolder.get("geometryPass");
		mat->diffuseMap = &g_defaultTexture;
		g_materialHolder.insert("default", std::move(mat));
	}
	{
		auto mat              = make_unique<Material>();
		mat->shaders          = &g_shaderHolder.get("geometryPass");
		mat->diffuseMap       = &g_kittenTexture;
		mat->specularExponent = 1e5;
		g_materialHolder.insert("cat", std::move(mat));
	}
	{
		auto mat        = make_unique<Material>();
		mat->shaders    = &g_shaderHolder.get("geometryPass");
		mat->diffuseMap = &g_stoneTexture;
		g_materialHolder.insert("stone", std::move(mat));
	}
	{
		auto mat        = make_unique<Material>();
		mat->shaders    = &g_shaderHolder.get("geometryPass");
		mat->diffuseMap = &g_terrainTexture;
		g_materialHolder.insert("terrain", std::move(mat));
	}
}
INTERNAL void loadSpriteAsset()
{
	{
		Mesh::Data meshData = {};

		reserve(meshData.vertices, 4);
		append(meshData.vertices, Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, +0.5f, 0.0f}, {1.0f, 1.0f}});
		append(meshData.vertices, Vertex{{-0.5f, +0.5f, 0.0f}, {0.0f, 1.0f}});

		reserve(meshData.indices, 6);
		meshData.addFace(0, 1, 2).addFace(2, 3, 0);
		meshData.generateNormals();

		g_meshHolder.insert("sprite", make_unique<Mesh>(meshData));

		g_sprite.material = &g_materialHolder.get("cat");
		g_sprite.mesh     = &g_meshHolder.get("sprite");
	}
	{
		Mesh::Data meshData = {};

		reserve(meshData.vertices, 4);
		append(meshData.vertices, Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+1.0f, +1.0f, 0.0f}, {1.0f, 1.0f}});
		append(meshData.vertices, Vertex{{-1.0f, +1.0f, 0.0f}, {0.0f, 1.0f}});

		reserve(meshData.indices, 6);
		meshData.addFace(0, 1, 2).addFace(2, 3, 0);
		meshData.generateNormals();

		g_meshHolder.insert("quad", make_unique<Mesh>(meshData));
	}
}

INTERNAL void update(Time dt)
{
	g_world->update(dt);
	if (Input::isKeyPressed(Input::Key::F11))
	{
		// TODO(bill): Toggle fullscreen

		// Initial OpenGL settings
		glInit();
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

INTERNAL void render() { g_world->render(); }

void init(int /*argc*/, char** /*argv*/)
{
	Memory::init();

	u32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
	               SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC;

	if (SDL_Init(sdlFlags) != 0)
	{
		std::cerr << "SDL Failed to initialize. Error: " << SDL_GetError()
		          << "\n";
		std::exit(EXIT_FAILURE);
	}

	g_window.create({854, 480, 24}, "Dunjun");
	g_window.setFramerateLimit(FrameLimit);

	glewInit();

	// Initial OpenGL settings
	glInit();

	Input::init();
	Input::setCursorPosition(g_window, {0, 0});

	loadShaders();
	loadMaterials();
	loadSpriteAsset();

	g_world = defaultAllocator().makeNew<World>();

	g_world->init(Context{g_window,
	                      g_shaderHolder,
	                      g_meshHolder,
	                      g_materialHolder});
}

void run()
{
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
			                               1000.0f / tc.getTickRate(),
			                               (u32)tc.getTickRate())
			                      .c_str());
		}

		render();

		g_window.display();
	}
}

void shutdown()
{
	defaultAllocator().makeDelete<World>(g_world);
	Input::shutdown();
	g_window.close();
	SDL_Quit();
	Memory::shutdown();

	std::exit(EXIT_SUCCESS);
}

void glInit()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
} // namespace Game
} // namespace Dunjun
