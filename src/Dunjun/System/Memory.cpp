#include <Dunjun/System/Memory.hpp>

#include <Dunjun/Common.hpp>

#include <mutex>

#include <cstdlib>

namespace Dunjun
{
namespace Memory
{
struct Header
{
	usize size;

	GLOBAL const usize PadValue{(usize)(-1)};
};

INTERNAL inline void fill(Header* header, void* data, usize size)
{
	header->size = size;
	usize* ptr = (usize*)(header + 1);
	while (ptr < data)
		*ptr++ = Header::PadValue;
}

INTERNAL inline Header* header(void* data)
{
	usize* v = (usize*)data;
	while (*(v - 1) == Header::PadValue)
		v--;

	return (Header*)v - 1;
}

class HeapAllocator : public Allocator
{
public:
	HeapAllocator()
	: m_totalAllocated{0}
	{
	}

	~HeapAllocator() override
	{
		// Check that all memory has been destroyed to prevent memory leaks
		assert(m_totalAllocated == 0);
	}

	virtual void* allocate(usize size, usize align = DefaultAlign) override
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		const usize total = size + align + sizeof(Header);
		Header* header = (Header*)malloc(total);
		void* ptr{header + 1};
		ptr = Memory::alignForward(ptr, align);
		fill(header, ptr, total);
		m_totalAllocated += total;
		return ptr;
	}

	virtual void deallocate(void* ptr) override
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		if (!ptr)
			return;

		Header* h = header(ptr);
		m_totalAllocated -= h->size;
		free(h);
	}

	virtual usize allocatedSize(void* ptr) override
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		return header(ptr)->size;
	}

	virtual usize totalAllocated() override
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		return m_totalAllocated;
	}

private:
	std::mutex m_mutex;
	usize m_totalAllocated;
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

void init()
{
	u8* ptr{g_memoryGlobals.buffer};
	g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator;
}

void shutdown()
{
	g_memoryGlobals.defaultAllocator->~HeapAllocator();
	g_memoryGlobals = MemoryGlobals{};
}
} // namespace Memory


Allocator& defaultAllocator()
{
	return *Memory::g_memoryGlobals.defaultAllocator;
}
} // namespace Dunjun
