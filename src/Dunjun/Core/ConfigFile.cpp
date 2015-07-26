#include <Dunjun/Core/ConfigFile.hpp>

#include <Dunjun/Core/StringFunctions.hpp>

#include <fstream>

namespace Dunjun
{
INTERNAL String removeConfigFileComment(const String& s)
{
	bool stringMode  = false;
	const usize sLen = len(s);
	usize i = 0;
	for (; i < sLen; i++)
	{
		char c = s[i];
		if (c == '\"')
			stringMode = !stringMode;

		if (!stringMode)
		{
			if (c == '#')
				break;
		}
	}
	return substring(s, 0, i);
}

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

ConfigFile loadConfigFileFromFile(const String& filepath)
{
	std::ifstream file;
	file.open(cString(filepath), std::ios::in | std::ios::binary);
	if (!file.is_open())
		panic("Cannot read settings file");
	defer(file.close());

	ConfigFile cf{};

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
			fprintf(stderr, "Error parsing line %d\n", lineNum);
			continue;
		}

		String name  = substring(line, 0, declarationPos);
		String type  = substring(line, declarationPos + 1, initializePos);
		String value = substring(line, initializePos + 1, len(line));

		name  = Strings::trimSpace(name);
		type  = Strings::toLower(Strings::trimSpace(type));
		value = removeConfigFileComment(value);
		value = Strings::trimSpace(value);

		// TODO(bill): handle errors

		if (type == "string")
			addStringToConfigFile(cf, name, value);
		else if (type == "uint")
			addUintToConfigFile(cf, name, value);
		else if (type == "int")
			addIntToConfigFile(cf, name, value);
		else if (type == "float")
			addFloatToConfigFile(cf, name, value);
		else if (type == "bool")
			addBoolToConfigFile(cf, name, value);
		else
		{
			fprintf(stderr, "Unknown type: %s\n", cString(type));
			continue;
		}
	}

	return cf;
}

ConfigFile::Variable getConfigFileVariable(const ConfigFile& cf,
                                           const String& name)
{
	ConfigFile::Variable d = {0, ConfigType_Unknown};
	ConfigFile::Variable v = get(cf.map, stringHash(name), d);
	return v;
}

bool addUintToConfigFile(ConfigFile& configFile,
                         const String& name,
                         const String& value)
{
	// TODO(bill): Better string conversion functions
	u32 a;

	if (sscanf(cString(value), "%u", &a) != 1)
	{
		fprintf(stderr, "%s is not an unsigned integer\n", cString(value));
		return false;
	}

	ConfigFile::Variable v = {};
	v.type                 = ConfigType_Uint;
	v.index                = 0;

	if (len(configFile.uints) > 0)
		v.index = len(configFile.uints) - 1;

	if (has(configFile.map, stringHash(name)))
	{
		fprintf(stderr,
		        "Variable with the name `%s` already exists\n",
		        cString(name));
		return false;
	}

	set(configFile.map, stringHash(name), v);

	append(configFile.uints, a);

	return true;
}

bool addIntToConfigFile(ConfigFile& configFile,
                        const String& name,
                        const String& value)
{
	// TODO(bill): Better string conversion functions
	s32 a;
	if (sscanf(cString(value), "%d", &a) != 1)
	{
		fprintf(stderr, "%s is not a signed integer\n", cString(value));
		return false;
	}

	ConfigFile::Variable v = {};
	v.type                 = ConfigType_Int;
	v.index                = 0;

	if (len(configFile.ints) > 0)
		v.index = len(configFile.ints) - 1;

	if (has(configFile.map, stringHash(name)))
	{
		fprintf(stderr,
		        "Variable with the name `%s` already exists\n",
		        cString(name));
		return false;
	}

	set(configFile.map, stringHash(name), v);

	append(configFile.ints, a);

	return true;
}

bool addFloatToConfigFile(ConfigFile& configFile,
                          const String& name,
                          const String& value)
{
	f32 f;

	if (sscanf(cString(value), "%f", &f) != 1)
	{
		fprintf(stderr, "%s is not a float\n", cString(value));
		return false;
	}

	ConfigFile::Variable v = {};
	v.type                 = ConfigType_Float;
	v.index                = 0;

	if (len(configFile.floats) > 0)
		v.index = len(configFile.floats) - 1;

	if (has(configFile.map, stringHash(name)))
	{
		fprintf(stderr,
		        "Variable with the name `%s` already exists\n",
		        cString(name));
		return false;
	}

	set(configFile.map, stringHash(name), v);

	append(configFile.floats, f);

	return true;
}

bool addBoolToConfigFile(ConfigFile& configFile,
                         const String& name,
                         const String& value)
{
	b8 a;
	if (value == "false" || value == "FALSE" || value == "0")
		a = false;
	else if (value == "true" || value == "TRUE" || value == "1")
		a = true;
	else
		return false;

	ConfigFile::Variable v = {};
	v.type                 = ConfigType_Bool;
	v.index                = 0;

	if (len(configFile.bools) > 0)
		v.index = len(configFile.bools) - 1;

	if (has(configFile.map, stringHash(name)))
	{
		fprintf(stderr,
		        "Variable with the name `%s` already exists\n",
		        cString(name));
		return false;
	}

	set(configFile.map, stringHash(name), v);

	append(configFile.bools, a);

	return true;
}

bool addStringToConfigFile(ConfigFile& configFile,
                           const String& name,
                           const String& value)
{
	if (value[0] != '\"' && value[len(value) - 2] != '\"')
	{
		fprintf(stderr, "A string must be surrounded by speech marks\n");
		return false;
	}

	ConfigFile::Variable v = {};
	v.type                 = ConfigType_String;
	v.index                = configFile.stringsLength++;

	if (has(configFile.map, stringHash(name)))
	{
		fprintf(stderr,
		        "Variable with the name `%s` already exists\n",
		        cString(name));
		return false;
	}

	set(configFile.map, stringHash(name), v);

	// NOTE(bill): Ignore `"` and `"` at the ends
	configFile.strings[v.index] = substring(value, 1, len(value) - 1);

	return true;
}
} // namespace Dunjun
