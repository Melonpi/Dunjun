#ifndef DUNJUN_SYSTEM_CONTAINERS_HPP
#define DUNJUN_SYSTEM_CONTAINERS_HPP

#include <Dunjun/System/Types.hpp>
#include <Dunjun/Memory/Allocator.hpp>

namespace Dunjun
{
// Array<T> is a dynamic array for POD types
// NOTE(bill): This will not construct and destruct elements
// TODO(bill): Create a container that will call ctor & dtor
template <typename T>
struct Array
{
	Allocator* allocator;
	usize length;
	usize capacity;
	T* data;

	Array(Allocator& a);
	~Array();

	Array(const Array& other);
	Array& operator=(const Array& other);

	T& operator[](usize index);
	const T& operator[](usize index) const;
};

// NOTE(bill): Queue<T> is a double ended queue/ring buffer
template <typename T>
struct Queue
{
	Array<T> data;
	usize length;
	usize offset;

	Queue(Allocator& a);

	T& operator[](usize index);
	const T& operator[](usize index) const;
};

// TODO(bill): Implement Hash Table/Map whatever
} // namespace Dunjun

#endif
