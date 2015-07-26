#ifndef DUNJUN_CORE_CONFIGFILE_HPP
#define DUNJUN_CORE_CONFIGFILE_HPP

#include <Dunjun/Types.hpp>
#include <Dunjun/Core/String.hpp>
#include <Dunjun/Core/ContainerTypes.hpp>

namespace Dunjun
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
	Array<b8> bools;

	u32 stringsLength;
	String strings[1024];

	HashMap<Variable> map;

	ConfigFile();
	~ConfigFile() = default;
};

ConfigFile loadConfigFileFromFile(const String& filepath);

ConfigFile::Variable getConfigFileVariable(const ConfigFile& cf,
                                           const String& name);
bool addUintToConfigFile(ConfigFile& configFile,
                         const String& name,
                         const String& value);

bool addIntToConfigFile(ConfigFile& configFile,
                        const String& name,
                        const String& value);

bool addFloatToConfigFile(ConfigFile& configFile,
                          const String& name,
                          const String& value);

bool addBoolToConfigFile(ConfigFile& configFile,
                         const String& name,
                         const String& value);

bool addStringToConfigFile(ConfigFile& configFile,
                           const String& name,
                           const String& value);

} // namespace Dunjun

#endif
