#ifndef DUNJUN_CORE_TEMPALLOCATOR_HPP
#define DUNJUN_CORE_TEMPALLOCATOR_HPP

#include <Dunjun/Core/Allocator.hpp>
#include <Dunjun/Core/Memory.hpp>

namespace Dunjun
{
// TempAllocator is a temporary memory allocator that mainly allocates memory
// from a local stack buffer with size `Size`.
// If the memory is used up, it will use the backing allocator (at the moment,
// that is the default (heap) allocator)
// NOTE(bill): Memory in a TempAllocator does not need to deallocated as it will
// be upon the destruction of the TempAllocator
template <usize Size>
class TempAllocator : public Allocator
{
public:
	TempAllocator(Allocator& backing = defaultScratchAllocator());
	~TempAllocator();

	void* allocate(usize size, usize align = DefaultAlign);
	void deallocate(void* /*ptr*/) {}
	usize allocatedSize(const void* /*ptr*/) { return SizeNotTracked; }
	usize totalAllocated() { return SizeNotTracked; }

private:
	Allocator& m_backing;

	u8* m_begin;
	u8* m_end;
	u8* m_current;

	usize m_chunkSize;

	u8 m_buffer[Size];
};

// NOTE(bill): Please use on the premade sizes for the TempAllocator<> to reduce
// unneeded template instantiation. If you need more than 4K, do _not_ use a
// TempAllocator! Try using the `defaultScratchAllocator()` or just the
// `defaultAllocator()`.
using TempAllocator64   = TempAllocator<64>;
using TempAllocator128  = TempAllocator<128>;
using TempAllocator256  = TempAllocator<256>;
using TempAllocator512  = TempAllocator<512>;
using TempAllocator1024 = TempAllocator<1024>;
using TempAllocator2048 = TempAllocator<2048>;
using TempAllocator4096 = TempAllocator<4096>;

////////////////////////////////////////
// TempAllocator<Size> Implementation //
////////////////////////////////////////

template <usize Size>
inline TempAllocator<Size>::TempAllocator(Allocator& backing)
: m_backing{backing}
, m_chunkSize{4 * 1024} // 4K
{
	m_current = m_begin = &m_buffer[0];
	m_end = m_begin + Size;
	*(void**)m_begin = nullptr; // Lovely casting
	m_current += sizeof(void*);
}

template <usize Size>
inline TempAllocator<Size>::~TempAllocator()
{
	void* ptr = *(void**)m_buffer;
	while (ptr)
	{
		void* nextPtr = *(void**)ptr;
		m_backing.deallocate(ptr);
		ptr = nextPtr;
	}
}

template <usize Size>
inline void* TempAllocator<Size>::allocate(usize size, usize align)
{
	m_current = (u8*)Memory::alignForward(m_current, align);

	if (size > (usize)(m_end - m_current))
	{
		usize toAllocate = size + align + sizeof(void*);

		if (toAllocate < m_chunkSize)
			toAllocate = m_chunkSize;

		m_chunkSize *= 2;

		void* ptr = m_backing.allocate(toAllocate);
		*(void**)m_begin = ptr;

		m_current = m_begin = (u8*)ptr;
		m_end = m_begin + toAllocate;

		*(void**)m_begin = nullptr; // Lovely casting

		m_current += sizeof(void*);
		m_current = (u8*)Memory::alignForward(m_current, align);
	}

	void* result = m_current;
	m_current += size;
	return result;
}

} // namespace Dunjun

#endif
