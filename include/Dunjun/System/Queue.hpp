#ifndef DUNJUN_SYSTEM_QUEUE_HPP
#define DUNJUN_SYSTEM_QUEUE_HPP

#include <Dunjun/System/Containers.hpp>

#include <Dunjun/System/Array.hpp>

#include <cstring>
#include <cassert>

namespace Dunjun
{
// Number of elements in queue
template <typename T>
usize len(const Queue<T>& q);

// Number of items the queue can hold before a resize is needed
template <typename T>
usize space(const Queue<T>& q);

// Appends an item to the queue and returns the number of items in the queue
// after the append
template <typename T>
usize append(Queue<T>& q, const T& item);

// Removes the last element from the queue
template <typename T>
void popBack(Queue<T>& q);

// Appends an item to the front of the queue and returns the number of items in
// the queue after the append
template <typename T>
usize prepend(Queue<T>& q, const T& item);

// Removes the first element from the queue
template <typename T>
void popFront(Queue<T>& q);

// Appends items to the queue and returns the number of items in the queue after
// the queue
template <typename T>
usize append(Queue<T>& q, const T* items, usize count);

// Removes a number of elements
template <typename T>
void pop(Queue<T>& q, usize count);

// Iterators
// NOTE(bill): These iterators only go from the Queue::offset to the
// end(Queue::data). end(q) - begin(q) may not equal len(q) because it is a ring
// buffer. If you need to iterate over all the elements, either use
// Queue::operator[](usize index) or iterate over Queue::data
template <typename T>
T* begin(Queue<T>& q);

template <typename T>
const T* begin(const Queue<T>& q);

template <typename T>
T* end(Queue<T>& q);

template <typename T>
const T* end(const Queue<T>& q);

// Same as q[0]
template <typename T>
T& front(Queue<T>& q);

template <typename T>
const T& front(const Queue<T>& q);

// same as q[len(q) - 1]
template <typename T>
T& back(Queue<T>& q);

template <typename T>
const T& back(const Queue<T>& q);

// Clears the content of the queue
// Does not free memory nor call destructs (e.g. POD)
template <typename T>
void clear(Queue<T>& q);

// Sets the capacity of the queue
template <typename T>
void setCapacity(Queue<T>& q, usize capacity);

// Reserves space in the queue for at least capacity items
template <typename T>
void reserve(Queue<T>& q, usize capacity);

// Grows the queue to contain at least minCapacity items
template <typename T>
void grow(Queue<T>& q, usize minCapacity = 0);

////////////////////////////////////////
//        Queue Implementation        //
////////////////////////////////////////

template <typename T>
inline usize append(Queue<T>& q, const T& item)
{
	if (space(q) == 0)
		grow(q);

	q[q.length++] = item;

	return q.length;
}

template <typename T>
inline void popBack(Queue<T>& q)
{
	assert(q.length > 0);

	q.length--;
}

template <typename T>
inline usize prepend(Queue<T>& q, const T& item)
{
	if (space(q) == 0)
		grow(q);

	q.offset = (q.offset - 1 + len(q.data)) % len(q.data);

	q[0] = item;

	q.length++;

	return q.length;
}

template <typename T>
inline void popFront(Queue<T>& q)
{
	assert(q.length > 0);

	q.offset = (q.offset + 1) % len(q.data);
	q.length--;
}

template <typename T>
inline usize append(Queue<T>& q, const T* items, usize count)
{
	if (space(q) < count)
		grow(q, q.length + count);

	const usize length = len(q.data);
	const usize insert = (q.offset + q.length) % length;

	usize toInsert = count;
	if (insert + toInsert > length)
		toInsert = length - insert;

	memcpy(begin(q.data) + insert, items, toInsert * sizeof(T));

	q.length += toInsert;
	items += toInsert;
	count -= toInsert;

	memcpy(begin(q.data), items, count * sizeof(T));

	q.length += count;

	return q.length;
}

template <typename T>
inline void pop(Queue<T>& q, usize count)
{
	assert(q.length > 0);

	q.offset = (q.offset + count) % len(q.data);
	q.length -= count;
}

template <typename T>
inline usize len(const Queue<T>& q)
{
	return q.length;
}

template <typename T>
inline usize space(const Queue<T>& q)
{
	return len(q.data) - q.length;
}

template <typename T>
inline T* begin(Queue<T>& q)
{
	return begin(q.data) + q.offset;
}

template <typename T>
inline const T* begin(const Queue<T>& q)
{
	return begin(q.data) + q.offset;
}

template <typename T>
inline T* end(Queue<T>& q)
{
	const usize endPos = q.offset + q.length;

	if (endPos >= len(q.data))
		return end(q.data);
	return begin(q.data) + endPos;
}

template <typename T>
inline const T* end(const Queue<T>& q)
{
	const usize endPos = q.offset + q.length;

	if (endPos >= len(q.data))
		return end(q.data);
	return begin(q.data) + endPos;
}

template <typename T>
inline T& front(Queue<T>& q)
{
	assert(q.length > 0);
	return q[0];
}

template <typename T>
inline const T& front(const Queue<T>& q)
{
	assert(q.length > 0);
	return q[0];
}

template <typename T>
inline T& back(Queue<T>& q)
{
	assert(q.length > 0);
	return q[q.length - 1];
}

template <typename T>
inline const T& back(const Queue<T>& q)
{
	assert(q.length > 0);
	return q[q.length - 1];
}

template <typename T>
inline void clear(Queue<T>& q)
{
	q.offset = 0;
	q.length = 0;
}

template <typename T>
inline void setCapacity(Queue<T>& q, usize capacity)
{
	const usize oldLength = len(q.data);

	resize(q.data, capacity);

	if (q.offset + q.length > oldLength)
	{
		memmove(begin(q.data) + capacity - (oldLength - q.offset),
		        begin(q.data) + q.offset,
		        (oldLength - q.offset) * sizeof(T));
		q.offset += capacity - oldLength;
	}
}

// Reserves space in the Queue for at least capacity items.
template <typename T>
inline void reserve(Queue<T>& q, usize capacity)
{
	if (capacity > q.length)
		setCapacity(q, capacity);
}

template <typename T>
inline void grow(Queue<T>& q, usize minCapacity)
{
	usize newCapacity = 2 * len(q.data) + 2;
	if (newCapacity < minCapacity)
		newCapacity = minCapacity;

	setCapacity(q, newCapacity);
}

////////////////
// Queue<T>:: //
////////////////

template <typename T>
inline Queue<T>::Queue(Allocator& a)
: data{a}
, length{0}
, offset{0}
{
}

template <typename T>
inline T& Queue<T>::operator[](usize index)
{
	return data[(offset + index) % len(data)];
}

template <typename T>
inline const T& Queue<T>::operator[](usize index) const
{
	return data[(offset + index) % len(data)];
}
} // namespace Dunjun

#endif
