#ifndef DUNJUN_CORE_ARRAY_HPP
#define DUNJUN_CORE_ARRAY_HPP

#include <Dunjun/Core/ContainerTypes.hpp>

#include <cstring> // memcpy

namespace Dunjun
{
// Number of elements in array
template <typename T>
usize len(const Array<T>& a);

// Maximum number of items the array can hold
template <typename T>
usize capacity(const Array<T>& a);

// Appends an item to the array and returns the number of items in the array
// after the append
template <typename T>
usize append(Array<T>& a, const T& item);

// Appends items to the array and returns the number of items in the array after
// the array
template <typename T>
usize append(Array<T>& a, const T* items, usize count);

// Removes the last element from the array
template <typename T>
void popBack(Array<T>& a);

// Iterators
template <typename T>
T* begin(Array<T>& a);
template <typename T>
const T* begin(const Array<T>& a);

template <typename T>
T* end(Array<T>& a);
template <typename T>
const T* end(const Array<T>& a);

template <typename T>
T& front(Array<T>& a);
template <typename T>
const T& front(const Array<T>& a);

template <typename T>
T& back(Array<T>& a);
template <typename T>
const T& back(const Array<T>& a);

// Clears the content of the array
// Does not free memory nor call destructs (e.g. POD), it only zeroes the
// elements
template <typename T>
void clear(Array<T>& a);

// Shrink the array so that its capactity equals its length
template <typename T>
void shrinkToFit(Array<T>& a);

// Resizes the array to a given size
// Old items will be copied to the new array. If the new capacity is smaller
// than the previous one, the array will be shortened.
template <typename T>
void resize(Array<T>& a, usize length);

// Sets the capacity of the array
template <typename T>
void setCapacity(Array<T>& a, usize capacity);

// Reserves space in the array for at least capacity items
template <typename T>
void reserve(Array<T>& a, usize capacity);

// Grows the array to contain at least minCapacity items
template <typename T>
void grow(Array<T>& a, usize minCapacity = 0);

////////////////////////////////////////
//        Array Implementation        //
////////////////////////////////////////

template <typename T>
inline usize append(Array<T>& a, const T& item)
{
	if (a.length + 1 > a.capacity)
		grow(a);
	a.data[a.length++] = item;

	return a.length;
}

template <typename T>
inline void popBack(Array<T>& a)
{
	assert(a.length > 0);

	a.length--;
}

template <typename T>
inline usize append(Array<T>& a, const T* items, usize count)
{
	if (a.capacity <= a.length + count)
		grow(a, a.length + count);

	memcpy(&a.data[a.length], items, count * sizeof(T));
	a.length += count;

	return a.length;
}

template <typename T>
inline usize len(const Array<T>& a)
{
	return a.length;
}

template <typename T>
inline usize capacity(const Array<T>& a)
{
	return a.capacity;
}

template <typename T>
inline T* begin(Array<T>& a)
{
	return a.data;
}

template <typename T>
inline const T* begin(const Array<T>& a)
{
	return a.data;
}

template <typename T>
inline T* end(Array<T>& a)
{
	return a.data + a.length;
}

template <typename T>
inline const T* end(const Array<T>& a)
{
	return a.data + a.length;
}

template <typename T>
inline T& front(Array<T>& a)
{
	assert(a.length > 0);
	return a.data[0];
}

template <typename T>
inline const T& front(const Array<T>& a)
{
	assert(a.length > 0);
	return a.data[0];
}

template <typename T>
inline T& back(Array<T>& a)
{
	assert(a.length > 0);
	return a.data[a.length - 1];
}

template <typename T>
inline const T& back(const Array<T>& a)
{
	assert(a.length > 0);
	return a.data[a.length - 1];
}

template <typename T>
inline void clear(Array<T>& a)
{
	resize(a, 0);
}

template <typename T>
inline void shrinkToFit(Array<T>& a)
{
	setCapacity(a, a.length);
}

template <typename T>
inline void resize(Array<T>& a, usize length)
{
	if (length > a.capacity)
		grow(a, length);
	a.length = length;
}

template <typename T>
inline void setCapacity(Array<T>& a, usize capacity)
{
	if (capacity == a.capacity)
		return;

	if (capacity < a.length)
		resize(a, capacity);

	T* data = nullptr;
	if (capacity > 0)
	{
		data = (T*)a.allocator->allocate(capacity * sizeof(T), alignof(T));
		memcpy(data, a.data, a.length * sizeof(T));
	}
	a.allocator->deallocate(a.data);
	a.data     = data;
	a.capacity = capacity;
}

template <typename T>
inline void reserve(Array<T>& a, usize capacity)
{
	if (capacity > a.capacity)
		setCapacity(a, capacity);
}

template <typename T>
inline void grow(Array<T>& a, usize minCapacity)
{
	usize capacity = 2 * a.capacity + 2;
	if (capacity < minCapacity)
		capacity = minCapacity;
	setCapacity(a, capacity);
}

////////////////
// Array<T>:: //
////////////////

template <typename T>
inline Array<T>::Array(Allocator& a)
: allocator{&a}
, length{0}
, capacity{0}
, data{nullptr}
{
}

template <typename T>
inline Array<T>::~Array()
{
	allocator->deallocate(data);
}

template <typename T>
inline Array<T>::Array(const Array& other)
: allocator{other.allocator}
, length{0}
, capacity{0}
, data{nullptr}
{
	const usize num = other.length;
	setCapacity(*this, num);
	memcpy(data, other.data, num * sizeof(T));
	length = num;
}

template <typename T>
inline Array<T>& Array<T>::operator=(const Array& other)
{
	const usize num = other.length;
	resize(*this, num);
	memcpy(data, other.data, num * sizeof(T));

	return *this;
}

template <typename T>
inline T& Array<T>::operator[](usize index)
{
	return data[index];
}

template <typename T>
inline const T& Array<T>::operator[](usize index) const
{
	return data[index];
}
} // namespace Dunjun

#endif
