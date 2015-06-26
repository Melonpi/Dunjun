#ifndef DUNJUN_SYSTEM_CONTAINERS_HPP
#define DUNJUN_SYSTEM_CONTAINERS_HPP

#include <Dunjun/System/Types.hpp>
#include <Dunjun/Memory/Allocator.hpp>

namespace Dunjun
{
// NOTE(bill): Array<T> is a dynamic array for POD types
template <typename T>
struct Array
{
	Allocator* allocator;
	usize length;
	usize capacity;
	T* data;
	/*
		Array(Allocator& a);
		~Array();

		Array(const Array& other);
		Array& operator=(const Array& other);

		T& operator[](usize index);
		const T& operator[](usize index) const;*/
	Array(Allocator& a)
	: allocator{&a}
	, length{0}
	, capacity{0}
	, data{nullptr}
	{
	}

	~Array()
	{
		allocator->deallocate(data);
	}

	Array(const Array& other)
	: allocator{other.allocator}
	, length{0}
	, capacity{0}
	, data{nullptr}
	{
		const usize num{other.length};
		setCapacity(*this, num);
		std::memcpy(data, other.data, num * sizeof(T));
		length = num;
	}

	Array& operator=(const Array& other)
	{
		const usize num{other.length};
		resize(*this, num);
		std::memcpy(data, other.data, num * sizeof(T));

		return *this;
	}

	inline T& operator[](usize index)
	{
		return data[index];
	}

	inline const T& operator[](usize index) const
	{
		return data[index];
	}
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
