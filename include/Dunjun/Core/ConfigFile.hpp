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

	// TODO(bill): implement other type
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
	// TODO(bill): store variable names with there hash

	ConfigFile();
	~ConfigFile() = default;
};

ConfigFile loadConfigFileFromFile(const String& filepath);

// TODO(bill): void saveConfigFileToFile(const ConfigFile& cf, const String& filepath);

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



u32 getUintFromConfigFile(const ConfigFile& cf,
                          const String& name,
                          const u32& defaultValue);

s32 getIntFromConfigFile(const ConfigFile& cf,
                         const String& name,
                         const s32& defaultValue);

f32 getFloatFromConfigFile(const ConfigFile& cf,
                           const String& name,
                           const f32& defaultValue);

b8 getBoolFromConfigFile(const ConfigFile& cf,
                         const String& name,
                         const b8& defaultValue);

String getStringFromConfigFile(const ConfigFile& cf,
                               const String& name,
                               const String& defaultValue);

} // namespace Dunjun

#endif
