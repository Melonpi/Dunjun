#ifndef DUNJUN_MEMORY_MEMORY_HPP
#define DUNJUN_MEMORY_MEMORY_HPP

#include <Dunjun/Core/Allocator.hpp>

#include <cassert>

namespace Dunjun
{
Allocator& defaultAllocator();
Allocator& defaultScratchAllocator();


namespace Memory
{
// Initialize the memory system
void init();
// Shutdown the memory system
void shutdown();

// `alignForward` returns the pointer `ptr` aligned to the desited `align` byte
inline void* alignForward(void* ptr, usize align)
{
	assert((align != 0) && !(align & (align - 1))); // Check if power of 2

	uintptr p = (uintptr)ptr;

	const usize modulo = p % align;
	if (modulo)
		p += (uintptr)(align - modulo);

	return (void*)p;
}
} // namespace Memory
} // namespace Dunjun

#endif
