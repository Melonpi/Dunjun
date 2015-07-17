#ifndef DUNJUN_CORE_HASHMAP_HPP
#define DUNJUN_CORE_HASHMAP_HPP

#include <Dunjun/Core/ContainerTypes.hpp>

#include <Dunjun/Core/Array.hpp>

#include <cstring>

namespace Dunjun
{
// If key exists in hash map
template <typename T>
bool has(const HashMap<T>& h, u64 key);

// Get a stored value for the corresponding key or default value if key
// does not exist with this hash map
template <typename T>
const T& get(const HashMap<T>& h, u64 key, const T& defaultValue);

// Sets the value for the specified key
template <typename T>
void set(HashMap<T>& h, u64 key, const T& value);

// Removes the key from the hash map if it exists
template <typename T>
void remove(HashMap<T>& h, u64 key);

// Reserve space for the hash lookup table
template <typename T>
void reserve(HashMap<T>& h, usize capacity);

// Remove all entries and hashes from the hash map
template <typename T>
void clear(HashMap<T>& h);

// Iterators (in random order)
template <typename T>
const typename HashMap<T>::Entry* begin(const HashMap<T>& h);
template <typename T>
const typename HashMap<T>::Entry* end(const HashMap<T>& h);

// Functions for Multi Hash Maps e.g. keys with mutliple values
namespace MultiHashMap
{
// Get all the entries with the corresponding key
template <typename T>
void get(const HashMap<T>& h, u64 key, Array<T>& items);

// Get the number of entries with that key
template <typename T>
usize count(const HashMap<T>& h, u64 key);

// Get the first entry with that key
template <typename T>
const typename HashMap<T>::Entry* findFirst(const HashMap<T>& h, u64 key);

// Get a pointer to next entry with the same key and if not found, returns
// a nullptr
template <typename T>
const typename HashMap<T>::Entry* findNext(const HashMap<T>& h,
                                           const typename HashMap<T>::Entry* e);

// Inserts the value as another value for the set key
template <typename T>
void insert(HashMap<T>& h, u64 key, const T& value);

// Removes the entry from the hash map iff it exists
template <typename T>
void remove(HashMap<T>& h, const typename HashMap<T>::Entry* e);

// Removes all the entries with the key
template <typename T>
void removeAll(HashMap<T>& h, u64 key);

} // namespace MultiHashMap

////////////////////////////////////////
//       HashMap Implementation       //
////////////////////////////////////////

namespace Impl
{
GLOBAL const usize EndOfList = (usize)(-1);

// Type for Result with holds the indexes of the hash; data; previous data
struct FindResult
{
	usize hashIndex;
	usize dataPrev;
	usize dataIndex;
};

template <typename T>
usize addEntry(HashMap<T>& h, u64 key)
{
	typename HashMap<T>::Entry e;
	e.key        = key;
	e.next       = Impl::EndOfList;
	usize eIndex = len(h.data);
	append(h.data, e);

	return eIndex;
}

template <typename T>
void erase(HashMap<T>& h, const FindResult& fr)
{
	if (fr.dataPrev == Impl::EndOfList)
		h.hashes[fr.hashIndex] = h.data[fr.dataIndex].next;
	else
		h.data[fr.dataPrev].next = h.data[fr.dataIndex].next;

	popBack(h.data); // updates length

	if (fr.dataIndex == len(h.data))
		return;

	h.data[fr.dataIndex] = h.data[len(h.data)];

	FindResult last = find(h, h.data[fr.dataIndex].key);

	if (last.dataPrev == Impl::EndOfList)
		h.hashes[last.hashIndex] = fr.dataIndex;
	else
		h.data[last.dataIndex].next = fr.dataIndex;
}

template <typename T>
FindResult find(const HashMap<T>& h, u64 key)
{
	FindResult fr;
	fr.hashIndex = Impl::EndOfList;
	fr.dataPrev  = Impl::EndOfList;
	fr.dataIndex = Impl::EndOfList;

	if (len(h.hashes) == 0)
		return fr;

	fr.hashIndex = key % len(h.hashes);
	fr.dataIndex = h.hashes[fr.hashIndex];
	while (fr.dataIndex != Impl::EndOfList)
	{
		if (h.data[fr.dataIndex].key == key)
			return fr;
		fr.dataPrev  = fr.dataIndex;
		fr.dataIndex = h.data[fr.dataIndex].next;
	}

	return fr;
}

template <typename T>
FindResult find(const HashMap<T>& h, const typename HashMap<T>::Entry* e)
{
	FindResult fr;
	fr.hashIndex = Impl::EndOfList;
	fr.dataPrev  = Impl::EndOfList;
	fr.dataIndex = Impl::EndOfList;

	if (len(h.hashes) == 0 || !e)
		return fr;

	fr.hashIndex = e->key % len(h.hashes);
	fr.dataIndex = h.hashes[fr.hashIndex];
	while (fr.dataIndex != Impl::EndOfList)
	{
		if (&h.data[fr.dataIndex] == e)
			return fr;
		fr.dataPrev  = fr.dataIndex;
		fr.dataIndex = h.data[fr.dataIndex].next;
	}

	return fr;
}

template <typename T>
usize make(HashMap<T>& h, u64 key)
{
	const FindResult fr = Impl::find(h, key);
	const usize index   = Impl::addEntry(h, key);

	if (fr.dataPrev == Impl::EndOfList)
		h.hashes[fr.hashIndex] = index;
	else
		h.data[fr.dataPrev].next = index;

	h.data[index].next = fr.dataIndex;

	return index;
}

template <typename T>
void findAndErase(HashMap<T>& h, u64 key)
{
	const FindResult fr = Impl::find(h, key);
	if (fr.dataIndex != Impl::EndOfList)
		erase(h, fr);
}

template <typename T>
usize findOrFail(const HashMap<T>& h, u64 key)
{
	return find(h, key).dataIndex;
}

template <typename T>
usize findOrMake(HashMap<T>& h, u64 key)
{
	const FindResult fr = find(h, key);
	if (fr.dataIndex != Impl::EndOfList)
		return fr.dataIndex;

	usize index = addEntry(h, key);
	if (fr.dataPrev == Impl::EndOfList)
		h.hashes[fr.hashIndex] = index;
	else
		h.data[fr.dataPrev].next = index;

	return index;
}

template <typename T>
void rehash(HashMap<T>& h, usize newCapacity)
{
	HashMap<T> nh = *h.hashes.allocator;
	resize(nh.hashes, newCapacity);
	const usize oldLength = len(h.data);
	reserve(nh.data, oldLength);

	for (usize i = 0; i < newCapacity; i++)
		nh.hashes[i] = Impl::EndOfList;

	for (usize i = 0; i < oldLength; i++)
	{
		auto& e = h.data[i];
		MultiHashMap::insert(nh, e.key, e.value);
	}

	HashMap<T> empty = *h.hashes.allocator;
	h.~HashMap<T>();

	memcpy(&h, &nh, sizeof(HashMap<T>));
	memcpy(&nh, &empty, sizeof(HashMap<T>));
}

template <typename T>
void grow(HashMap<T>& h)
{
	const usize newCapacity = 2 * len(h.data) + 2;
	rehash(h, newCapacity);
}

template <typename T>
bool full(HashMap<T>& h)
{
	// Make sure that there is enough space
	const f32 maximumLoadCoefficient = 0.75f;
	return len(h.data) >= maximumLoadCoefficient * len(h.hashes);
}
} // namespace Impl

////////////////

template <typename T>
inline bool has(const HashMap<T>& h, u64 key)
{
	return Impl::findOrFail(h, key) != Impl::EndOfList;
}

template <typename T>
inline const T& get(const HashMap<T>& h, u64 key, const T& defaultValue)
{
	const usize index = Impl::findOrFail(h, key);

	if (index == Impl::EndOfList)
		return defaultValue;
	return h.data[index].value;
}

template <typename T>
inline void set(HashMap<T>& h, u64 key, const T& value)
{
	if (len(h.hashes) == 0)
		Impl::grow(h);

	const usize index   = Impl::findOrMake(h, key);
	h.data[index].value = value;
	if (Impl::full(h))
		Impl::grow(h);
}

template <typename T>
inline void remove(HashMap<T>& h, u64 key)
{
	Impl::findAndErase(h, key);
}

template <typename T>
inline void reserve(HashMap<T>& h, usize capacity)
{
	Impl::rehash(h, capacity);
}

template <typename T>
inline void clear(HashMap<T>& h)
{
	clear(h.hashes);
	clear(h.data);
}

template <typename T>
inline const typename HashMap<T>::Entry* begin(const HashMap<T>& h)
{
	return begin(h.data);
}

template <typename T>
inline const typename HashMap<T>::Entry* end(const HashMap<T>& h)
{
	return end(h.data);
}

namespace MultiHashMap
{
template <typename T>
inline void get(const HashMap<T>& h, u64 key, Array<T>& items)
{
	auto e = findFirst(h, key);

	while (e)
	{
		append(items, e->value);
		e = findNext(h, e);
	}
}

template <typename T>
inline usize count(const HashMap<T>& h, u64 key)
{
	usize c = 0;
	auto e = findFirst(h, key);
	while (e)
	{
		c++;
		e = findNext(h, e);
	}

	return c;
}

template <typename T>
inline void insert(HashMap<T>& h, u64 key, const T& value)
{
	if (len(h.hashes) == 0)
		Impl::grow(h);

	const usize next   = Impl::make(h, key);
	h.data[next].value = value;

	if (Impl::full(h))
		Impl::grow(h);
}

template <typename T>
inline const typename HashMap<T>::Entry* findFirst(const HashMap<T>& h, u64 key)
{
	const usize index = Impl::findOrFail(h, key);
	if (index == Impl::EndOfList)
		return nullptr;
	return &h.data[index];
}

template <typename T>
inline const typename HashMap<T>::Entry*
    findNext(const HashMap<T>& h, const typename HashMap<T>::Entry* e)
{
	if (!e)
		return nullptr;

	usize index = e->next;
	while (index != Impl::EndOfList)
	{
		if (h.data[index].key == e->key)
			return &h.data[index];
		index = h.data[index].next;
	}

	return nullptr;
}

template <typename T>
inline void remove(HashMap<T>& h, const typename HashMap<T>::Entry* e)
{
	const Impl::FindResult fr = Impl::find(h, e);
	if (fr.dataIndex != Impl::EndOfList)
		Impl::erase(h, fr);
}

template <typename T>
inline void removeAll(HashMap<T>& h, u64 key)
{
	while (has(h, key))
		remove(h, key);
}

} // namespace MultiHashMap

//////////////////
// HashMap<T>:: //
//////////////////

template <typename T>
inline HashMap<T>::HashMap(Allocator& a)
: hashes{a}
, data{a}
{
}
} // namespace Dunjun

#endif
