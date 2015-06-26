#ifndef DUNJUN_SYSTEM_ARRAY_HPP
#define DUNJUN_SYSTEM_ARRAY_HPP

#include <cstring>

namespace Dunjun
{
// TODO(bill): Document Array<T> functions
template <typename T>
void pushBack(Array<T>& a, const T& item);
template <typename T>
void popBack(Array<T>& a);

// TODO(bill): emplaceBack???


// Number of elements in array
template <typename T>
usize len(const Array<T>& a);
// Is array empty?
template <typename T>
bool empty(const Array<T>& a);

// Iterators
template <typename T>
T* begin(const Array<T>& a);
template <typename T>
const T* begin(const Array<T>& a);
template <typename T>
T* end(const Array<T>& a);
template <typename T>
const T* end(const Array<T>& a);

template <typename T>
T& front(const Array<T>& a);
template <typename T>
const T& front(const Array<T>& a);
template <typename T>
T& back(const Array<T>& a);
template <typename T>
const T& back(const Array<T>& a);

template <typename T>
void clear(Array<T>& a);

template <typename T>
void resize(Array<T>& a, usize length);
template <typename T>
void setCapacity(Array<T>& a, usize capacity);
template <typename T>
void reserve(Array<T>& a, usize capacity);

template <typename T>
void grow(Array<T>& a, usize minCapacity = 0);


////////////////////////////////////////

template <typename T>
inline void pushBack(Array<T>& a, const T& item)
{
	if (a.length > a.capacity - 1)
		grow(a);
	a.data[a.length++] = item;
}
template <typename T>
inline void popBack(Array<T>& a)
{
	a.length--;
}


// Number of elements in array
template <typename T>
inline usize len(const Array<T>& a)
{
	return a.length;
}
// Is array empty?
template <typename T>
inline bool empty(const Array<T>& a)
{
	return a.length != 0;
}

// Iterators
template <typename T>
inline T* begin(const Array<T>& a)
{
	return a.data;
}
template <typename T>
inline const T* begin(const Array<T>& a)
{
	return a.data;
}
template <typename T>
inline T* end(const Array<T>& a)
{
	return a.data + a.length;
}
template <typename T>
inline const T* end(const Array<T>& a)
{
	return a.data + a.length;
}

template <typename T>
inline T& front(const Array<T>& a)
{
	return a.data[0];
}
template <typename T>
inline const T& front(const Array<T>& a)
{
	return a.data[0];
}
template <typename T>
inline T& back(const Array<T>& a)
{
	return a.data[a.length - 1];
}
template <typename T>
inline const T& back(const Array<T>& a)
{
	return a.data[a.length - 1];
}

template <typename T>
inline void clear(Array<T>& a)
{
	resize(a, 0);
}

template <typename T>
void resize(Array<T>& a, usize length)
{
	if (length > a.capacity)
		grow(a, length);
	a.length = length;
}
template <typename T>
void setCapacity(Array<T>& a, usize capacity)
{
	if (capacity == a.capacity)
		return;

	if (capacity < a.length)
		resize(a, capacity);

	T* data{nullptr};
	if (capacity > 0)
	{
		data = (T*)a.allocator->allocate(capacity * sizeof(T), alignof(T));
		std::memcpy(data, a.data, a.length * sizeof(T));
	}
	a.allocator->deallocate(a.data);
	a.data = data;
	a.capacity = capacity;
}
template <typename T>
void reserve(Array<T>& a, usize capacity)
{
	if (capacity > a.capacity)
		setCapacity(a, capacity);
}

template <typename T>
void grow(Array<T>& a, usize minCapacity)
{
	usize capacity{2*a.capacity + 2};
	if (capacity < minCapacity)
		capacity = minCapacity;
	setCapacity(a, capacity);
}

////////////////////////////////////////

/*
template <typename T>
inline Array::Array(Allocator& a)
: allocator(&a)
, length{0}
, capacity{0}
, data{nullptr}
{
}

template <typename T>
inline Array::~Array()
{
	allocator->deallocate(data);
}

template <typename T>
Array::Array(const Array& other)
: allocator(other.allocator)
, length{0}
, capacity{0}
, data{nullptr}
{
	const usize num{other.length};
	setCapacity(*this, num);
	std::memcpy(data, other.data, num * sizeof(T));
	length = num;
}

template <typename T>
Array& Array::operator=(const Array& other)
{
	const usize num{other.length};
	resize(*this, num);
	std::memcpy(data, other.data, num * sizeof(T));

	return *this;
}

template <typename T>
inline T& Array::operator[](usize index)
{
	return data[index];
}
template <typename T>
inline const T& Array::operator[](usize index) const
{
	return data[index];
}
*/
} // namespace Dunjun

#endif
