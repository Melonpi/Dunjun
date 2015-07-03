#include <Dunjun/Memory/Memory.hpp>
#include <Dunjun/System/Container.hpp>

#include <Dunjun/System/Array.hpp>
#include <Dunjun/System/Queue.hpp>
#include <Dunjun/System/HashMap.hpp>

#include <cassert>

void allocatorTest();
{
	Memory::init();
	Allocator& a = defaultAllocator();

	void* p = a.allocate(10);
	defer(a.deallocate(p));

	assert(a.allocatedSize(p) >= 10);
	assert(a.totalAllocated() >= 10);

	void* q = a.allocate(10);
	defer(a.deallocate(q));

	assert(a.allocatedSize(q) >= 10);
	assert(a.totalAllocated() >= 20);

	Memory::shutdown();
}

void arrayTest()
{
	Memory::init();
	Allocator& a = defaultAllocator();

	Array<int> v1{a};
	assert(len(v1) == 0);
	append(v1, 7);
	assert(len(v1) == 1);
	assert(v1[0] == 7);

	Array<int> v2{v1};
	assert(v2[0] == 7);
	v2[0] = 2;
	assert(v1[0] == 7);
	assert(v2[0] == 2);
	v2 = v1;
	assert(v2[0] == 7);

	assert((usize)(end(v1) - begin(v1)) == len(v1));
	assert(front(v1) == 7);
	popBack(v1);
	assert(len(v1) == 0);

	for (int i{0}; i < 100; i++)
		append(v1, i);
	assert(len(v1) == 100);

	Memory::shutdown();
}

void arrayTest()
{
	Memory::init();
	Allocator& a = defaultAllocator();

	Queue<int> q{a};

	reserve(q, 10);
	assert(space(q) == 10);
	append(q, 1);
	prepend(q, 2);
	assert(len(q) == 2);
	assert(q[0] == 2);
	assert(q[1] == 1);
	clear(q);
	assert(len(q) == 0);
	int items[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	append(q, items, 10);
	assert(len(q) == 10);
	for (int i{0}; i < 10; i++)
		assert(q[i] == i + 1);
	clear(q);
	assert(len(q) == 0);

	Memory::shutdown();
}

void hashMapTest()
{
	Memory::init();
	Allocator& a = defaultAllocator();

	HashMap<int> h{a};
	assert(!has(h, 0));
	assert(get(h, 0, 1337) == 1337);
	remove(h, 0);
	set(h, 123, 7331);
	assert(get(h, 123, 0) == 7331);
	assert(get(h, 321, 666) == 666);

	for (int i{0}; i < 100; i++)
		set(h, i, i * i);
	for (int i{0}; i < 100; i++)
		assert(get(h, i, 0) == i * i);
	remove(h, 123);
	assert(!has(h, 123));
	remove(h, 321);
	assert(get(h, 123, 0) == 0);
	for (int i{0}; i < 100; i++)
		assert(get(h, i, 0) == i * i);
	clear(h);
	for (int i{0}; i < 100; i++)
		assert(!has(h, i));

	Memory::shutdown();
}

void multiHashMap()
{
	Memory::init();
	Allocator& a = defaultAllocator();

	HashMap<int> h{a};

	assert(Multi::count(h, 0) == 0);
	Multi::insert(h, 0, 1);
	Multi::insert(h, 0, 2);
	Multi::insert(h, 0, 3);
	assert(Multi::count(h, 0) == 3);

	Array<int> v{a};
	Multi::get(h, 0, v);
	assert(len(v) == 3);
	std::sort(begin(v), end(v));
	assert(v[0] == 1 && v[1] == 2 && v[2] == 3);

	Multi::remove(h, Multi::findFirst(h, 0));
	assert(Multi::count(h, 0) == 2);
	Multi::removeAll(h, 0);
	assert(Multi::count(h, 0) == 0);

	Memory::shutdown();
}

int main(int argc, char** argv)
{
	allocatorTest();
	arrayTest();
	queueTest();
	hashMapTest();
	multiHashMap();

	return 0;
}
