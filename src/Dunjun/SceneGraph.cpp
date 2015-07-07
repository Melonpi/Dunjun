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

SceneGraph::~SceneGraph() { allocator.deallocate(data.buffer); }

void SceneGraph::allocate(u32 capacity)
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

SceneGraph::NodeId SceneGraph::create(EntityId id, const Transform& t)
{
	if (data.capacity == data.length) // grow
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

	return last;
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
	set(map, entity, EmptyNodeId);

	data.length--;
}

SceneGraph::NodeId SceneGraph::getNodeId(EntityId id) const
{
	return get(map, id, EmptyNodeId);
}

bool SceneGraph::isValid(NodeId id) const { return id != EmptyNodeId; }

u32 SceneGraph::nodeCount() const { return data.length; }

void SceneGraph::link(NodeId parentId, NodeId childId)
{
	unlink(childId);

	if (!isValid(data.firstChild[parentId]))
	{
		data.firstChild[parentId] = childId;
		data.parent[parentId]     = parentId;
	}
	else
	{
		NodeId prev = EmptyNodeId;
		NodeId curr = data.firstChild[parentId];
		while (isValid(curr))
		{
			prev = curr;
			curr = data.nextSibling[curr];
		}

		data.nextSibling[prev] = childId;

		data.firstChild[childId]  = EmptyNodeId;
		data.nextSibling[childId] = EmptyNodeId;
		data.prevSibling[childId] = prev;
	}

	const Transform parentTransform = data.world[parentId];
	const Transform childTransform  = data.world[childId];

	// TODO(bill): check to see if this is correct
	const Transform localTransform = parentTransform / childTransform;

	data.local[childId]  = localTransform;
	data.parent[childId] = parentId;

	transformChild(childId, parentTransform);
}

void SceneGraph::unlink(NodeId childId)
{
	if (!isValid(data.parent[childId]))
		return;

	if (!isValid(data.prevSibling[childId]))
		data.firstChild[data.parent[childId]] = data.nextSibling[childId];
	else
		data.nextSibling[data.prevSibling[childId]] = data.nextSibling[childId];

	if (isValid(data.nextSibling[childId]))
		data.prevSibling[data.nextSibling[childId]] = data.prevSibling[childId];

	data.parent[childId]      = EmptyNodeId;
	data.nextSibling[childId] = EmptyNodeId;
	data.prevSibling[childId] = EmptyNodeId;
}

void SceneGraph::transformChild(NodeId id, const Transform& t)
{
	data.world[id] = data.local[id] * t;

	NodeId child = data.firstChild[id];
	while (isValid(child))
	{
		transformChild(child, data.world[id]);
		child = data.nextSibling[child];
	}
}

void SceneGraph::updateLocal(NodeId id)
{
	NodeId parentId           = data.parent[id];
	Transform parentTransform = {};
	if (isValid(parentId))
		parentTransform = data.world[parentId];
	transformChild(id, parentTransform);
}

void SceneGraph::updateWorld(NodeId id)
{
	NodeId parentId           = data.parent[id];
	Transform parentTransform = {};
	if (isValid(parentId))
		parentTransform = data.world[parentId];
	data.local[id] = data.world[id] / parentTransform;
	transformChild(id, parentTransform);
}

////////////////

Transform SceneGraph::getLocalTransform(NodeId id) const
{
	return data.local[id];
}

Transform SceneGraph::getWorldTransform(NodeId id) const
{
	return data.world[id];
}

////////////////

Vector3 SceneGraph::getLocalPosition(NodeId id) const
{
	return getLocalTransform(id).position;
}

Quaternion SceneGraph::getLocalOrientation(NodeId id) const
{
	return getLocalTransform(id).orientation;
}

Vector3 SceneGraph::getLocalScale(NodeId id) const
{
	return getLocalTransform(id).scale;
}

Vector3 SceneGraph::getWorldPosition(NodeId id) const
{
	return getWorldTransform(id).position;
}

Quaternion SceneGraph::getWorldOrientation(NodeId id) const
{
	return getWorldTransform(id).orientation;
}

Vector3 SceneGraph::getWorldScale(NodeId id) const
{
	return getWorldTransform(id).scale;
}

////////////////

void SceneGraph::setLocalTransform(NodeId id, const Transform& t)
{
	data.local[id] = t;
	updateLocal(id);
}

void SceneGraph::setWorldTransform(NodeId id, const Transform& t)
{
	data.world[id] = t;
	updateWorld(id);
}

void SceneGraph::setLocalPosition(NodeId id, const Vector3& position)
{
	data.local[id].position = position;
	updateLocal(id);
}

void SceneGraph::setLocalOrientation(NodeId id, const Quaternion& orientation)
{
	data.local[id].orientation = orientation;
	updateLocal(id);
}

void SceneGraph::setLocalScale(NodeId id, const Vector3& scale)
{
	data.local[id].scale = scale;
	updateLocal(id);
}

void SceneGraph::setWorldPosition(NodeId id, const Vector3& position)
{
	data.world[id].position = position;
	updateWorld(id);
}

void SceneGraph::setWorldOrientation(NodeId id, const Quaternion& orientation)
{
	data.world[id].orientation = orientation;
	updateWorld(id);
}

void SceneGraph::setWorldScale(NodeId id, const Vector3& scale)
{
	data.world[id].scale = scale;
	updateWorld(id);
}

} // namespace Dunjun
