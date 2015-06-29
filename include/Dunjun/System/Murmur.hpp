#ifndef DUNJUN_SYSTEM_MURMUR_HPP
#define DUNJUN_SYSTEM_MURMUR_HPP

#include <Dunjun/Types.hpp>

#include <cstring>

namespace Dunjun
{
u64 murmurHash64(const void* key, s32 len, u64 seed = 0x9747b28c);

inline u64 stringHash(const char* str, u64 seed = 0x9747b28c)
{
	return murmurHash64(str, strlen(str), seed);
}

} // namespace Dunjun

#endif

