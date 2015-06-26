#include <Dunjun/Memory/Memory.hpp>

#include <cstdlib>

namespace Dunjun
{

struct Header
{
	GLOBAL const Allocator::SizeType PadValue{(Allocator::SizeType)(-1)};

	Allocator::SizeType size;
};

INTERNAL inline void fill(Header* header, void* data, Allocator::SizeType size)
{
	header->size = size;
	Allocator::SizeType* ptr = (Allocator::SizeType*)(header + 1);
	while (ptr < data)
		*ptr++ = Header::PadValue;
}

INTERNAL inline Header* header(void* data)
{
	Allocator::SizeType* v = (Allocator::SizeType*)data;
	while (*(v - 1) == Header::PadValue)
		v--;

	return (Header*)v - 1;
}

// NOTE(bill): Similar to how malloc is implemented internall with C
class HeapAllocator : public Allocator
{
public:
	HeapAllocator()
	: m_totalAllocated{0}
	{
	}

	~HeapAllocator() {}

	virtual void* allocate(SizeType size, SizeType align = DefaultAlign)
	{
		const SizeType total = size + align + sizeof(Header);
		Header* header = (Header*)malloc(total);
		void* ptr{header + 1};
		ptr = Memory::alignForward(ptr, align);
		fill(header, ptr, total);
		return ptr;
	}

	virtual void deallocate(void* ptr)
	{
		if (!ptr)
			return;

		Header* h = header(ptr);
		m_totalAllocated -= h->size;
		free(h);
	}

	virtual SizeType allocatedSize(void* ptr)
	{
		return header(ptr)->size;
	}

private:
	SizeType m_totalAllocated;
};

struct MemoryGlobals
{
	GLOBAL const usize AllocatorMemory = sizeof(HeapAllocator);
	u8 buffer[AllocatorMemory];

	HeapAllocator* defaultAllocator;

	MemoryGlobals()
	: defaultAllocator{nullptr}
	{
	}
};

namespace
{
MemoryGlobals g_memoryGlobals;
} // namespace (anonymous)


Allocator& defaultAllocator()
{
	return *g_memoryGlobals.defaultAllocator;
}

namespace Memory
{
void init()
{
	u8* ptr{g_memoryGlobals.buffer};
	g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator{};
}

void shutdown()
{
	g_memoryGlobals.defaultAllocator->~HeapAllocator();
	g_memoryGlobals = MemoryGlobals{};
}
} // namespace Memory
} // namespace Dunjun
