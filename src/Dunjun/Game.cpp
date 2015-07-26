#include <Dunjun/Config.hpp>
#include <Dunjun/Game.hpp>

#include <Dunjun/Core.hpp>
#include <Dunjun/Window.hpp>
#include <Dunjun/ResourceHolders.hpp>

#include <Dunjun/Core/ContainerTypes.hpp>
#include <Dunjun/Core/Array.hpp>
#include <Dunjun/Core/Queue.hpp>
#include <Dunjun/Core/HashMap.hpp>
#include <Dunjun/Core/Murmur.hpp>
#include <Dunjun/Core/TempAllocator.hpp>
#include <Dunjun/Core/StringFunctions.hpp>

#include <Dunjun/World.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/RenderSystem.hpp>

#include <fstream>

#include <cassert>
#include <cstdlib>

namespace Dunjun
{
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

GLOBAL Material g_kittenMaterial;
GLOBAL Material g_terrainMaterial;
GLOBAL Material g_stoneMaterial;
GLOBAL Material g_woodMaterial;
} // namespace (anonymous)

namespace Game
{
enum ConfigType
{
	ConfigType_Unknown,

	ConfigType_Uint,
	ConfigType_Int,
	ConfigType_Float,
	ConfigType_Bool,
	ConfigType_String,
};

struct ConfigFile
{
	struct Variable
	{
		u32 index;
		ConfigType type;
	};

	Array<u32> uints;
	Array<s32> ints;
	Array<f32> floats;
	Array<b8>  bools;

	u32 stringsLength;
	String strings[1024];

	HashMap<Variable> map;

	ConfigFile();
	~ConfigFile() = default;
};

ConfigFile::ConfigFile()
: uints{defaultAllocator()}
, ints{defaultAllocator()}
, floats{defaultAllocator()}
, bools{defaultAllocator()}
, stringsLength{0}
, strings{}
, map{defaultAllocator()}
{
}

ConfigFile::Variable getConfigFileVariable(const ConfigFile& cf, const String& name)
{
	ConfigFile::Variable d = {0, ConfigType_Unknown};
	ConfigFile::Variable v = get(cf.map, stringHash(name), d);
	return v;
}

bool addUintToConfigFile(ConfigFile& configFile, const String& name, const String& value)
{
	usize i = 0;
	if (value[0] == '+')
		i = 1;
	for (; i < len(value); i++)
	{
		if (!Strings::isDigit(value[i]))
			return false;
	}

	// TODO(bill): Better string conversion functions
	u32 a = atoi(cString(value));

	ConfigFile::Variable v = {};
	v.type = ConfigType_Uint;
	v.index = 0;

	if (len(configFile.uints) > 0)
		v.index = len(configFile.uints) - 1;

	if (has(configFile.map, stringHash(name)))
		return false;

	set(configFile.map, stringHash(name), v);

	append(configFile.uints, a);

	return true;
}

bool addIntToConfigFile(ConfigFile& configFile, const String& name, const String& value)
{
	usize i = 0;
	if (value[0] == '+' || value[0] == '-')
		i = 1;
	for (; i < len(value); i++)
	{
		if (!Strings::isDigit(value[i]))
			return false;
	}

	// TODO(bill): Better string conversion functions
	s32 a = atoi(cString(value));

	ConfigFile::Variable v = {};
	v.type = ConfigType_Int;
	v.index = 0;

	if (len(configFile.ints) > 0)
		v.index = len(configFile.ints) - 1;

	if (has(configFile.map, stringHash(name)))
		return false;

	set(configFile.map, stringHash(name), v);

	append(configFile.ints, a);

	return true;
}

bool addFloatToConfigFile(ConfigFile& configFile, const String& name, const String& value)
{
	f32 f;

	if (sscanf(cString(value), "%f", &f) != 1)
		return false;

	ConfigFile::Variable v = {};
	v.type = ConfigType_Float;
	v.index = 0;

	if (len(configFile.floats) > 0)
		v.index = len(configFile.floats) - 1;

	if (has(configFile.map, stringHash(name)))
		return false;

	set(configFile.map, stringHash(name), v);

	append(configFile.floats, f);

	return true;
}

bool addBoolToConfigFile(ConfigFile& configFile, const String& name, const String& value)
{
	b8 a;
	if (value == "false" || value == "FALSE" || value == "0")
		a = false;
	else if (value == "true" || value == "TRUE" || value == "1")
		a = true;
	else
		return false;

	ConfigFile::Variable v = {};
	v.type = ConfigType_Bool;
	v.index = 0;

	if (len(configFile.bools) > 0)
		v.index = len(configFile.bools) - 1;

	if (has(configFile.map, stringHash(name)))
		return false;

	set(configFile.map, stringHash(name), v);

	append(configFile.bools, a);

	return true;
}

bool addStringToConfigFile(ConfigFile& configFile, const String& name, const String& value)
{
	if (value[0] != '\"' && value[len(value)-2] != '\"')
		return false;

	ConfigFile::Variable v = {};
	v.type = ConfigType_String;
	v.index = configFile.stringsLength++;

	if (has(configFile.map, stringHash(name)))
		return false;

	set(configFile.map, stringHash(name), v);

	// NOTE(bill): Ignore `"` and `"` at the ends
	configFile.strings[v.index] = substring(value, 1, len(value)-1);

	return true;
}

INTERNAL void configTest()
{
	std::ifstream file;
	file.open("data/settings.fred", std::ios::in | std::ios::binary);
	if (!file.is_open())
		panic("Cannot read settings file");
	defer(file.close());

	ConfigFile configFile{};

	usize lineNum = 0;
	String line;
	while (file.good())
	{
		defer(lineNum++);

		getline(file, line);
		line = Strings::trimSpace(line);

		if (len(line) == 0)
			continue;
		if (line[0] == '#')
			continue;

		ssize declarationPos = -1;
		ssize initializePos  = -1;

		const usize lineLen = len(line);
		for (usize i = 0; i < lineLen; i++)
		{
			if (line[i] == ':' && declarationPos == -1)
				declarationPos = i;
			else if (line[i] == '=' && initializePos == -1)
				initializePos = i;
		}

		if (declarationPos == -1 || initializePos == -1)
		{
			std::cerr << "Error parsing line " << lineNum << "\n";
			continue;
		}

		String name  = substring(line, 0, declarationPos);
		String type  = substring(line, declarationPos+1, initializePos);
		String value = substring(line, initializePos+1, len(line));


		name = Strings::trimSpace(name);
		type = Strings::toLower(Strings::trimSpace(type));
		value = Strings::trimSpace(value);

		// TODO(bill): handle errors

		if (type == "string")
			addStringToConfigFile(configFile, name, value);
		else if (type == "uint")
			addUintToConfigFile(configFile, name, value);
		else if (type == "int")
			addIntToConfigFile(configFile, name, value);
		else if (type == "float")
			addFloatToConfigFile(configFile, name, value);
		else if (type == "bool")
			addBoolToConfigFile(configFile, name, value);
		else
		{
			std::cerr << "Unknown type: " << type << "\n";
			continue;
		}
	}

	{
		auto v = getConfigFileVariable(configFile, "var3");
		if (v.type == ConfigType_String)
		{
			std::cout << configFile.strings[v.index] << "\n";
		}
	}


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

	g_kittenMaterial            = Material{};
	g_kittenMaterial.diffuseMap = &g_kittenTexture;

	g_terrainMaterial            = Material{};
	g_terrainMaterial.diffuseMap = &g_terrainTexture;

	g_stoneMaterial            = Material{};
	g_stoneMaterial.diffuseMap = &g_stoneTexture;

	g_woodMaterial            = Material{};
	g_woodMaterial.diffuseMap = &g_woodTexture;
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
		append(meshData.vertices, Vertex{{+0.5f, -0.5f, 0.0f}, {8.0f, 0.0f}});
		append(meshData.vertices, Vertex{{+0.5f, +0.5f, 0.0f}, {8.0f, 8.0f}});
		append(meshData.vertices, Vertex{{-0.5f, +0.5f, 0.0f}, {0.0f, 8.0f}});

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
		rs.addComponent(surface, {g_meshHolder.get("wall"), g_woodMaterial});
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
		for (int i = 0; i < 3; i++)
		{
			PointLight pl;
			pl.position.x = r.getFloat(-4, 4);
			pl.position.y = r.getFloat(0.1f, 1.0f);
			pl.position.z = r.getFloat(-4, 4);

			pl.intensity = r.getFloat(0.5f, 2.0f);
			pl.color.r   = r.getInt(50, 255);
			pl.color.g   = r.getInt(50, 255);
			pl.color.b   = r.getInt(50, 255);
			pl.color.a = 255;
			pl.range = calculateLightRange(pl);

			append(rs.pointLights, pl);
		}
	}

	glInit();
}

void init(int /*argCount*/, char** /*args*/)
{
	Memory::init();

	configTest();
	exit(0);

	u32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
	               SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC;

	if (SDL_Init(sdlFlags) != 0)
	{
		std::cerr << "SDL Failed to initialize. Error: " << SDL_GetError()
		          << "\n";
		exit(EXIT_FAILURE);
	}

	g_window.create({854, 480, 24}, "Dunjun");
	// g_window.setFramerateLimit(FrameLimit);
	// g_window.setVerticalSyncEnabled(true);

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
			                               1000.0f / tc.tickRate,
			                               (u32)tc.tickRate));
		}

		render();

		g_window.display();
	}
}

void shutdown()
{
	// NOTE(bill): There is no real need to call this as the OS should do this
	defaultAllocator().makeDelete(g_world);
	Input::shutdown();
	g_window.close();
	SDL_Quit();
	Memory::shutdown();

	exit(EXIT_SUCCESS);
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
