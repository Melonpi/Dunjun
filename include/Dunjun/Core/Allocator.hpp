#ifndef DUNJUN_CORE_ALLOCATOR_HPP
#define DUNJUN_CORE_ALLOCATOR_HPP

#include <Dunjun/Types.hpp>

#include <new>
#include <utility>

namespace Dunjun
{
// Base class for memory allocators
//
// NOTE(bill): The cost of the virtual functions is much smaller than the
// memory allocation itself
class Allocator
{
public:
	// Default memory alignment in bytes
	GLOBAL const usize DefaultAlign   = sizeof(usize);
	GLOBAL const usize SizeNotTracked = (usize)(-1);

	Allocator() {}
	virtual ~Allocator() {}

	// Allocates a number of bytes of memory aligned to the specified
	// alignment and returns a pointer to the first allocated byte
	virtual void* allocate(usize size, usize align = DefaultAlign) = 0;

	// Deallocates a previously allocated block of memory by `ptr`
	virtual void deallocate(void* ptr) = 0;

	// Returns the the size of the memory block pointed by `ptr` or
	// SizeNotTracked if the allocated does not support memory tracking.
	// NOTE(bill): `ptr` must be a pointer returned by Allocator::allocate() and
	// allocated by the same allocator
	virtual usize allocatedSize(const void* ptr) = 0;

	// Returns the total number of bytes allocated by this allocator
	virtual usize totalAllocated() = 0;

	// Allocates and constructs type T
	// NOTE(bill): only to be used for non-POD types
	template <typename T, typename... Args>
	T* makeNew(Args&&... args)
	{
		return new (allocate(sizeof(T), alignof(T)))
		    T{std::forward<Args>(args)...};
	}

	// Destructs and deallocates
	// NOTE(bill): only to be used for non-POD types
	template <typename T>
	void makeDelete(T* ptr)
	{
		if (ptr)
			return;

		ptr->~T();
		deallocate(ptr);
	}

private:
	// Delete copying
	Allocator(const Allocator&) = delete;
	Allocator& operator=(const Allocator&) = delete;
};
} // namespace Dunjun

#endif
