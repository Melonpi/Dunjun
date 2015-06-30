#include <Dunjun/System/Murmur.hpp>
#include <Dunjun/Config.hpp>

namespace Dunjun
{
// clang-format off

#if defined(DUNJUN_64_BIT)

u64 murmurHash64(const void* key, s32 len, u64 seed)
{
	const u64 m{0xc6a4a7935bd1e995ULL};
	const s32 r{47};

	u64 h{seed ^ (len * m)};

	const u64* data{(const u64*)key};
	const u64* end{data + (len / 8)};

	while (data != end)
	{
		u64 k{*data++};

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const u8* data2{(const u8*)data};

	switch (len & 7)
	{
	case 7: h ^= u64{data2[6]} << 48;
	case 6: h ^= u64{data2[5]} << 40;
	case 5: h ^= u64{data2[4]} << 32;
	case 4: h ^= u64{data2[3]} << 24;
	case 3: h ^= u64{data2[2]} << 16;
	case 2: h ^= u64{data2[1]} << 8;
	case 1: h ^= u64{data2[0]};
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

#elif defined(DUNJUN_32_BIT)

u64 murmurHash64(const void* key, s32 len, u64 seed)
{
	const u32 m = 0x5bd1e995;
	const s32 r = 24;

	u32 h1 = u32(seed) ^ len;
	u32 h2 = u32(seed >> 32);

	const u32* data{(const u32*)key};

	while (len >= 8)
	{
		u32 k1{*data++};
		k1 *= m;
		k1 ^= k1 >> r;
		k1 *= m;
		h1 *= m;
		h1 ^= k1;
		len -= 4;

		u32 k2{*data++};
		k2 *= m;
		k2 ^= k2 >> r;
		k2 *= m;
		h2 *= m;
		h2 ^= k2;
		len -= 4;
	}

	if (len >= 4)
	{
		u32 k1{*data++};
		k1 *= m;
		k1 ^= k1 >> r;
		k1 *= m;
		h1 *= m;
		h1 ^= k1;
		len -= 4;
	}

	switch (len)
	{
	case 3: h2 ^= ((u8*)data)[2] << 16;
	case 2: h2 ^= ((u8*)data)[1] << 8;
	case 1: h2 ^= ((u8*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18;
	h1 *= m;
	h2 ^= h1 >> 22;
	h2 *= m;
	h1 ^= h2 >> 17;
	h1 *= m;
	h2 ^= h1 >> 19;
	h2 *= m;

	u64 h{h1};

	h = (h << 32) | h2;

	return h;
}

#else
	#error This bit architecture is not supported
#endif

// clang-format on
} // namespace Dunjun
