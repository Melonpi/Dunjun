#ifndef DUNJUN_CORE_MURMUR_HPP
#define DUNJUN_CORE_MURMUR_HPP

#include <Dunjun/Types.hpp>
#include <Dunjun/Core/String.hpp>

namespace Dunjun
{
// A hashing function that can be used with a HashMap<T>
// Source: https://code.google.com/p/smhasher/
u64 murmurHash64(const void* key, s32 len, u64 seed = 0x9747b28c);

inline u64 stringHash(const char* str, u64 seed = 0x9747b28c)
{
	return murmurHash64(str, (s32)len(str), seed);
}

inline u64 stringHash(const String& str, u64 seed = 0x9747b28c)
{
	return murmurHash64(cString(str), (s32)len(str), seed);
}
} // namespace Dunjun

#endif
