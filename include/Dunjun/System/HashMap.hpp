#ifndef DUNJUN_SYSTEM_HASHMAP_HPP
#define DUNJUN_SYSTEM_HASHMAP_HPP

#include <Dunjun/System/Containers.hpp>

#include <cstring>

namespace Dunjun
{
template <typename T>
bool has(const HashMap<T>& h, u64 key);
template <typename T>
const T& get(const HashMap<T>& h, u64 key);
template <typename T>
const T& get(const HashMap<T>& h, u64 key, const T& defaultValue);
template <typename T>
void set(HashMap<T>& h, u64 key, const T& value);
template <typename T>
void remove(HashMap<T>& h, u64 key);
template <typename T>
void reserve(HashMap<T>& h, usize capacity);
template <typename T>
void clear(HashMap<T>& h);

template <typename T>
const typename HashMap<T>::Entry* begin(const HashMap<T>& h);
template <typename T>
const typename HashMap<T>::Entry* end(const HashMap<T>& h);


template <typename T>
void insert(HashMap<T>& h, u64 key, const T& value);


////////////////////////////////////////
//       HashMap Implementation       //
////////////////////////////////////////


GLOBAL const usize EndOfList{(usize)(-1)};


struct FindResult
{
	usize hashIndex;
	usize dataPrev;
	usize dataIndex;
};


// addEntry
// erase
// insert
// find
// findAndErase
// make

template <typename T>
FindResult find(const HashMap<T>& h, u64 key)
{
	FindResult fr;
	fr.hashIndex = EndOfList;
	fr.dataPrev  = EndOfList;
	fr.dataIndex = EndOfList;

	if (len(h.hashes) == 0)
		return fr;


	fr.hashIndex = key % len(h.hashes);
	fr.dataIndex = h.hahes[fr.hashIndex];
	while (fr.dataIndex != EndOfList)
	{
		if (h.data[fr.dataIndex].key == key)
			return fr;
		fr.dataPrev = fr.dataIndex;
		fr.dataIndex = h.data[fr.dataIndex].next;
	}

	return fr;
}

template <typename T>
FindResult find(const HashMap<T>& h, const typename HashMap<T>::Entry* e)
{
	FindResult fr;
	fr.hashIndex = EndOfList;
	fr.dataPrev  = EndOfList;
	fr.dataIndex = EndOfList;

	if (len(h.hashes) == 0 || !e)
		return fr;


	fr.hashIndex = e->key % len(h.hashes);
	fr.dataIndex = h.hahes[fr.hashIndex];
	while (fr.dataIndex != EndOfList)
	{
		if (&h.data[fr.dataIndex] == e)
			return fr;
		fr.dataPrev = fr.dataIndex;
		fr.dataIndex = h.data[fr.dataIndex].next;
	}

	return fr;
}



template <typename T>
usize findOrFail(const HashMap<T>& h, u64 key)
{
	return find(h, key).dataIndex;
}


template <typename T>
usize findOrMake(const HashMap<T>& h, u64 key)
{
	const FindResult fr{find(h, key)};
	if (fr.dataIndex != EndOfList)
		return fr.dataIndex;

	// TODO(bill): add a new entry

	return (usize)-1;
}




template <typename T>
void rehash(HashMap<T>& h, usize newCapacity)
{
	HashMap<T>& nh{*h.hashes.a};
	resize(nh.hashes, newCapacity);
	const usize oldLength{len(h.data)};
	reserve(nh.data, oldLength);

	for (usize i{0}; i < newCapacity; i++)
		nh.hashes[i] = EndOfList;

	for (usize i{0}; i < oldLength; i++)
	{
		auto& e = h.data[i];
		insert(nh, e.key, e.value);
	}


	HashMap<T> empty{*h.hashes.Allocator};
	h.~HashMap<T>();

	memcpy(&h, &nh, sizeof(HashMap<T>));
	memcpy(&nh, &empty, sizeof(HashMap<T>));
}

template <typename T>
void grow(HashMap<T>& h)
{
	const usize newCapacity{2 * len(h.data) + 2};
	rehash(h, newCapacity);
}

template <typename T>
bool full(HashMap<T>& h)
{
	LOCAL_PERSIST const f32 maximumLoadCoefficient{0.75f};
	return len(h.data) >= maximumLoadCoefficient * len(h.hashes);
}

template <typename T>
bool has(const HashMap<T>& h, u64 key);

template <typename T>
const T& get(const HashMap<T>& h, u64 key);

template <typename T>
const T& get(const HashMap<T>& h, u64 key, const T& defaultValue);

template <typename T>
void set(HashMap<T>& h, u64 key, const T& value);

template <typename T>
void remove(HashMap<T>& h, u64 key);

template <typename T>
void reserve(HashMap<T>& h, usize capacity);

template <typename T>
void clear(HashMap<T>& h)
{
	clear(h.hashes);
	clear(h.data);
}

template <typename T>
const typename HashMap<T>::Entry* begin(const HashMap<T>& h)
{
	return begin(h.data);
}

template <typename T>
const typename HashMap<T>::Entry* end(const HashMap<T>& h)
{
	return end(h.data);
}



template <typename T>
void insert(HashMap<T>& h, u64 key, const T& value)
{
	if (len(h.hashes) == 0)
		grow(h);

	const usize next{make(h, key)};
	h.data[next].value = value;

	if (full(h))
		grow(h);
}



template <typename T>
HashMap<T>::HashMap(Allocator& a)
: hashes{a}
, data{a}
{
}

} // namespace Dunjun

#endif
