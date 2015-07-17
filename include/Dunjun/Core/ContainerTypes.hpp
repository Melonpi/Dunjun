#ifndef DUNJUN_CORE_CONTAINERTYPES_HPP
#define DUNJUN_CORE_CONTAINERTYPES_HPP

#include <Dunjun/Types.hpp>
#include <Dunjun/Core/Allocator.hpp>

namespace Dunjun
{
// Array<T> is a dynamic array for POD types
// NOTE(bill): ctors & dtors are not called. This allows for
// the use of memmove
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

// Queue<T> is a double ended queue/ring buffer for POD types
// NOTE(bill): ctors & dtors are not called. This allows for
// the use of memmove
// TODO(bill): Create a container that will call ctor & dtor
template <typename T>
struct Queue
{
	Array<T> data;
	usize length;
	usize offset;

	Queue(Allocator& a);
	~Queue() = default;

	T& operator[](usize index);
	const T& operator[](usize index) const;
};

// HashMap<T> is an unordered hash map/table for POD types
// The key is a u64 so a hash function must return as a u64
// The HashMap can be a regular HashMap or a MultiHashMap through
// Multi:: namespaced functions
// TODO(bill): Create a container that will call ctor & dtor
template <typename T>
struct HashMap
{
	struct Entry
	{
		u64 key;
		usize next;
		T value;
	};

	Array<usize> hashes;
	Array<Entry> data;

	HashMap(Allocator& a);
	~HashMap() = default;
};
} // namespace Dunjun

#endif
