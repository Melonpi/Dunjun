#include <Dunjun/System/Memory.hpp>

#include <Dunjun/Common.hpp>

#include <mutex>

#include <cstdlib>

namespace Dunjun
{
namespace Memory
{
namespace
{
class HeapAllocator : public Allocator
{
public:
	HeapAllocator()
	: m_totalAllocated{0}
	, m_allocationCount{0}
	{
	}

	~HeapAllocator()
	{
		// Check that all memory has been freed to prevent memory leaks
		assert(m_allocationCount == 0 && totalAllocated() == 0);
	}

	virtual void* allocate(usize size, usize align = DefaultAlign) override
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		const usize total = size + align + sizeof(Header);

		Header* header = (Header*)malloc(total);
		header->size = total;

		void* ptr = alignForward(header + 1, align);

		pad(header, ptr);

		m_totalAllocated += total;
		m_allocationCount++;

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
		m_allocationCount--;

		free(h);
	}

	virtual usize allocatedSize(const void* ptr) override
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
	struct Header
	{
		usize size;

		GLOBAL const usize PadValue = (usize)(-1);
	};

	std::mutex m_mutex;
	usize m_totalAllocated;
	usize m_allocationCount;

	inline void pad(Header* header, void* data)
	{
		usize* ptr = (usize*)(header + 1);

		while (ptr != data)
			*ptr++ = Header::PadValue;
	}

	inline Header* header(const void* data)
	{
		const usize* ptr = (usize*)data;
		ptr--;

		while (*ptr == Header::PadValue)
			ptr--;

		return (Header*)ptr;
	}
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

MemoryGlobals g_memoryGlobals;
} // namespace (anonymous)

void init()
{
	u8* ptr = g_memoryGlobals.buffer;

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
