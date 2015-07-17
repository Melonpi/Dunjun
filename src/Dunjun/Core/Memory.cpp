#include <Dunjun/Core/Memory.hpp>

#include <Dunjun/Common.hpp>

#include <mutex>

#include <cstdlib>

namespace Dunjun
{
namespace Memory
{
namespace
{
struct Header
{
	usize size;

	GLOBAL const usize PadValue = (usize)(-1);
};

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

// An allocator that uses the default system malloc(). Allocations are padded
// so that we can store the size of each allocation and align them to the
// desired aligment.
//
// NOTE(bill): An OS-specific heap allocator can take track of the size of an
// allocation and will do it much more efficiently.
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

	virtual void* allocate(usize size, usize align = DefaultAlign)
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		const usize total = size + align + sizeof(Header);

		Header* header = (Header*)malloc(total);
		header->size   = total;

		void* ptr = alignForward(header + 1, align);

		pad(header, ptr);

		m_totalAllocated += total;
		m_allocationCount++;

		return ptr;
	}

	virtual void deallocate(void* ptr)
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

	virtual usize allocatedSize(const void* ptr)
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		return header(ptr)->size;
	}

	virtual usize totalAllocated()
	{
		m_mutex.lock();
		defer(m_mutex.unlock());

		return m_totalAllocated;
	}

private:
	std::mutex m_mutex;
	usize m_totalAllocated;
	usize m_allocationCount;
};

// An allocator used to allocate temporary (AKA scratch) memory.
// ScratchAllocator
// uses a fixed size ring buffer.
//
// NOTE(bill): If the ring buffer is exhausted, the scratch allocator will use
// its backing allocator to allocate memory instead.
class ScratchAllocator : public Allocator
{
public:
	ScratchAllocator(Allocator& backing, usize size)
	: m_backing{backing}
	{
		m_begin    = (u8*)m_backing.allocate(size);
		m_end      = m_begin + size;
		m_allocate = m_begin;
		m_free     = m_begin;
	}

	~ScratchAllocator()
	{
		assert(m_free == m_allocate);
		m_backing.deallocate(m_begin);
	}

	bool inUse(const void* ptr)
	{
		if (m_allocate == m_allocate)
			return false;
		if (m_allocate > m_free)
			return ptr >= m_free && ptr < m_allocate;

		return ptr >= m_free || ptr < m_allocate;
	}

	virtual void* allocate(usize size, usize align = DefaultAlign)
	{
		assert(align % 4 == 0);      // NOTE(bill): Align should be 4 or 8
		size = ((size + 3) / 4) * 4; // Move size to next alignment block

		u8* ptr   = m_allocate;
		Header* h = (Header*)ptr;
		u8* data  = (u8*)alignForward(h + 1, align);
		ptr       = data + size;

		// NOTE(bill): Reached the end of the buffer, wrap around to the
		// beginning
		if (ptr > m_end)
		{
			h->size = (m_end - (u8*)h) | ((usize)(-1));

			ptr  = m_begin;
			h    = (Header*)ptr;
			data = (u8*)alignForward(h + 1, align);
			ptr  = data + size;
		}

		// NOTE(bill): If the buffer is exhausted use the backing allocator
		if (inUse(ptr))
			return m_backing.allocate(size, align);

		h->size = ptr - (u8*)h;
		pad(h, data);

		m_allocate = ptr;

		return data;
	}

	virtual void deallocate(void* ptr)
	{
		if (!ptr)
			return;

		if (ptr < m_begin || ptr >= m_end)
		{
			m_backing.deallocate(ptr);
			return;
		}

		// NOTE(bill): Set slot to be free
		Header* h = header(ptr);
		assert((ssize)h->size >= 0);
		h->size = h->size | ((usize)(-1));

		// Move the free pointer past all the free slots
		while (m_free != m_allocate)
		{
			Header* h = (Header*)m_free;
			if ((ssize)(h->size) < 0)
				break;

			// Loop back
			m_free += h->size;
			if (m_free == m_end)
				m_free = m_begin;
		}
	}

	virtual usize allocatedSize(const void* ptr)
	{
		const Header* h = header(ptr);
		return h->size - ((const u8*)ptr - (const u8*)h);
	}

	virtual usize totalAllocated() { return m_end - m_begin; }

private:
	Allocator& m_backing;

	u8* m_begin;
	u8* m_end;

	u8* m_allocate;
	u8* m_free;
};

struct MemoryGlobals
{
	GLOBAL const usize AllocatorMemory =
	    sizeof(HeapAllocator) + sizeof(ScratchAllocator);
	u8 buffer[AllocatorMemory];

	HeapAllocator* defaultAllocator           = nullptr;
	ScratchAllocator* defaultScratchAllocator = nullptr;
};

MemoryGlobals g_memoryGlobals;
} // namespace (anonymous)

void init()
{
	u8* ptr = g_memoryGlobals.buffer;

	g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator;
	ptr += sizeof(HeapAllocator); // Move to next allocator
	// NOTE(bill): set with 16K bytes of temporary memory
	g_memoryGlobals.defaultScratchAllocator = new (ptr)
	    ScratchAllocator(*g_memoryGlobals.defaultAllocator, 16 * 1024);
}

void shutdown()
{
	// NOTE(bill): destruct the scratch allocator first as its backing is the
	// heap allocator
	g_memoryGlobals.defaultScratchAllocator->~ScratchAllocator();
	g_memoryGlobals.defaultAllocator->~HeapAllocator();
	// Set everything to null
	g_memoryGlobals = MemoryGlobals{};
}
} // namespace Memory

Allocator& defaultAllocator()
{
	return *Memory::g_memoryGlobals.defaultAllocator;
}

Allocator& defaultScratchAllocator()
{
	return *Memory::g_memoryGlobals.defaultScratchAllocator;
}
} // namespace Dunjun
