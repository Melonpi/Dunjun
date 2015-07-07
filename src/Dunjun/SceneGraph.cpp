#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/System/Hashmap.hpp>

#include <cstring>

namespace Dunjun
{
SceneGraph::SceneGraph(Allocator& a)
: allocator{a}
, data{}
, map{allocator}
{
}

SceneGraph::~SceneGraph()
{
	allocator.deallocate(data.buffer);
	map.~Array();
}

void SceneGraph::allocate(usize capacity)
{
	if (capacity <= data.length)
		return;

	const usize bytes = capacity * (sizeof(EntityId) + 2 * sizeof(Transform) +
	                                4 * sizeof(NodeId));

	InstanceData newData;
	newData.length   = data.length;
	newData.capacity = capacity;
	newData.buffer   = allocator.allocate(bytes);

	newData.entityId    = (EntityId*)(newData.buffer);
	newData.local       = (Transform*)(newData.entityId + capacity);
	newData.world       = (Transform*)(newData.local + capacity);
	newData.parent      = (NodeId*)(newData.world + capacity);
	newData.firstChild  = (NodeId*)(newData.parent + capacity);
	newData.prevSibling = (NodeId*)(newData.firstChild + capacity);
	newData.nextSibling = (NodeId*)(newData.prevSibling + capacity);

	memcpy(newData.entityId, data.entityId, data.length * sizeof(EntityId));
	memcpy(newData.local, data.local, data.length * sizeof(Transform));
	memcpy(newData.world, data.world, data.length * sizeof(Transform));
	memcpy(newData.parent, data.parent, data.length * sizeof(NodeId));
	memcpy(newData.firstChild, //
	       data.firstChild,
	       data.length * sizeof(NodeId));
	memcpy(newData.prevSibling, data.prevSibling, data.length * sizeof(NodeId));
	memcpy(newData.nextSibling, data.nextSibling, data.length * sizeof(NodeId));

	allocator.deallocate(data.buffer);
	data = newData;
}

void SceneGraph::create(EntityId id, const Transform& t)
{
	if (data.capacity = data.length) // grow
		allocate(2 * data.length + 1);

	const NodeId last = data.length;

	data.entityId[last]    = id;
	data.local[last]       = t;
	data.world[last]       = t;
	data.parent[last]      = EmptyNodeId;
	data.firstChild[last]  = EmptyNodeId;
	data.prevSibling[last] = EmptyNodeId;
	data.nextSibling[last] = EmptyNodeId;

	set(map, id, last);

	data.length++;
}

void SceneGraph::destroy(NodeId id)
{
	const NodeId last         = data.length - 1;
	const EntityId entity     = data.entityId[id];
	const EntityId lastEntity = data.entityId[last];

	data.entityId[id]    = data.entityId[last];
	data.local[id]       = data.local[last];
	data.world[id]       = data.world[last];
	data.parent[id]      = data.parent[last];
	data.firstChild[id]  = data.firstChild[last];
	data.prevSibling[id] = data.prevSibling[last];
	data.nextSibling[id] = data.nextSibling[last];

	set(map, lastEntity, id);
	set(map, entitym EmptyNodeId);

	data.length--;
}

NodeId SceneGraph::getNodeId(EntityId id) const
{
	return get(map, id, EmptyNodeId);
}

bool SceneGraph::isValid(NodeId id) const
{
	return id != EmptyNodeId;
}

void SceneGraph::link(NodeId parent, NodeId child)
{
	unlink(child);

	if (!isValid(data.firstChild[parent]))
	{
		data.firstChild[parent] = child;
		data.parent[child] = parent;
	}
	else
	{
		NodeId prev = EmptyNodeId;
		NodeId curr = data.firstChild[parent];
		while (isValid(curr))
		{
			prev = curr;
			curr = data.nextSibling[curr];
		}

		data.nextSibling[prev] = child;

		data.firstChild[child] = EmptyNodeId;
		data.nextSibling[child] = EmptyNodeId;
		data.prevSibling[child] = prev;
	}

	const Transform parentTransform = data.world[parent];
	const Transform childTransform = data.world[child];

	// TODO(bill): check to see if this is correct
	const Transform localTransform = parentTransform / childTransform;

	data.local[child] = localTransform;
	data.parent[child] = parent;

	transformChild(child, parentTransform);
}

void SceneGraph::unlink(NodeId child)
{
	// TODO(bill):
}
} // namespace Dunjun
