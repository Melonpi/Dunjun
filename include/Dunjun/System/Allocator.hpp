#ifndef DUNJUN_MEMORY_ALLOCATOR_HPP
#define DUNJUN_MEMORY_ALLOCATOR_HPP

#include <Dunjun/Types.hpp>

#include <new>
#include <utility>

namespace Dunjun
{
class Allocator
{
public:
	GLOBAL const usize DefaultAlign{4};
	GLOBAL const usize SizeNotTracked{(usize)(-1)};

	Allocator() {}
	virtual ~Allocator() {}

	virtual void* allocate(usize size, usize align = DefaultAlign) = 0;
	virtual void deallocate(void* ptr) = 0;
	virtual usize allocatedSize(void* ptr) = 0;
	virtual usize totalAllocated() = 0;

	template <typename T, typename... Args>
	T* makeNew(Args&&... args)
	{
		return new (allocate(sizeof(T), alignof(T))) T{std::forward<Args>(args)...};
	}

	template <typename T>
	void makeDelete(T* ptr)
	{
		if (ptr)
		{
			ptr->~T();
			deallocate(ptr);
		}
	}

private:
	Allocator(const Allocator&) = delete;
	Allocator& operator=(const Allocator&) = delete;
};
} // namespace Dunjun

#endif
