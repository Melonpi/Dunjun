#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Core/Hashmap.hpp>

#include <cstring>

namespace Dunjun
{
SceneGraph::SceneGraph(Allocator& a)
: allocator{a}
, data{}
, map{allocator}
{
	allocate(16);
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
	newData.global      = (Transform*)(newData.local + capacity);
	newData.parent      = (NodeId*)(newData.global + capacity);
	newData.firstChild  = (NodeId*)(newData.parent + capacity);
	newData.prevSibling = (NodeId*)(newData.firstChild + capacity);
	newData.nextSibling = (NodeId*)(newData.prevSibling + capacity);

	memcpy(newData.entityId, data.entityId, data.length * sizeof(EntityId));
	memcpy(newData.local, data.local, data.length * sizeof(Transform));
	memcpy(newData.global, data.global, data.length * sizeof(Transform));
	memcpy(newData.parent, data.parent, data.length * sizeof(NodeId));
	memcpy(newData.firstChild, //
	       data.firstChild,
	       data.length * sizeof(NodeId));
	memcpy(newData.prevSibling, data.prevSibling, data.length * sizeof(NodeId));
	memcpy(newData.nextSibling, data.nextSibling, data.length * sizeof(NodeId));

	allocator.deallocate(data.buffer);
	data = newData;
}

SceneGraph::NodeId SceneGraph::addNode(EntityId id, const Transform& t)
{
	if (data.capacity == data.length || data.capacity == 0) // grow
		allocate(2 * data.length + 1);

	const NodeId last = data.length;

	data.entityId[last]    = id;
	data.local[last]       = t;
	data.global[last]      = t;
	data.parent[last]      = EmptyNodeId;
	data.firstChild[last]  = EmptyNodeId;
	data.prevSibling[last] = EmptyNodeId;
	data.nextSibling[last] = EmptyNodeId;

	set(map, id, last);

	data.length++;

	return last;
}

void SceneGraph::removeNode(NodeId id)
{
	const NodeId last         = data.length - 1;
	const EntityId entity     = data.entityId[id];
	const EntityId lastEntity = data.entityId[last];

	data.entityId[id]    = data.entityId[last];
	data.local[id]       = data.local[last];
	data.global[id]      = data.global[last];
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

void SceneGraph::linkNodes(NodeId parentId, NodeId childId)
{
	unlinkNode(childId);

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

	const Transform parentTransform = data.global[parentId];
	const Transform childTransform  = data.global[childId];

	// TODO(bill): check to see if this is correct
	const Transform localTransform = parentTransform / childTransform;

	data.local[childId]  = localTransform;
	data.parent[childId] = parentId;

	transformChild(childId, parentTransform);
}

void SceneGraph::unlinkNode(NodeId childId)
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

void SceneGraph::transformChild(NodeId id, const Transform& parentTransform)
{
	data.global[id] = parentTransform * data.local[id];

	NodeId child = data.firstChild[id];
	while (isValid(child))
	{
		transformChild(child, data.global[id]);
		child = data.nextSibling[child];
	}
}

void SceneGraph::updateLocal(NodeId id)
{
	NodeId parentId           = data.parent[id];
	Transform parentTransform = {};
	if (isValid(parentId))
		parentTransform = data.global[parentId];
	transformChild(id, parentTransform);
}

void SceneGraph::updateGlobal(NodeId id)
{
	NodeId parentId           = data.parent[id];
	Transform parentTransform = {};
	if (isValid(parentId))
		parentTransform = data.global[parentId];
	data.local[id] = data.global[id] / parentTransform;
	transformChild(id, parentTransform);
}

////////////////

Transform SceneGraph::getLocalTransform(NodeId id) const
{
	return data.local[id];
}

Transform SceneGraph::getGlobalTransform(NodeId id) const
{
	return data.global[id];
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

Vector3 SceneGraph::getGlobalPosition(NodeId id) const
{
	return getGlobalTransform(id).position;
}

Quaternion SceneGraph::getGlobalOrientation(NodeId id) const
{
	return getGlobalTransform(id).orientation;
}

Vector3 SceneGraph::getGlobalScale(NodeId id) const
{
	return getGlobalTransform(id).scale;
}

////////////////

void SceneGraph::setLocalTransform(NodeId id, const Transform& t)
{
	data.local[id] = t;
	updateLocal(id);
}

void SceneGraph::setGlobalTransform(NodeId id, const Transform& t)
{
	data.global[id] = t;
	updateGlobal(id);
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

void SceneGraph::setGlobalPosition(NodeId id, const Vector3& position)
{
	data.global[id].position = position;
	updateGlobal(id);
}

void SceneGraph::setGlobalOrientation(NodeId id, const Quaternion& orientation)
{
	data.global[id].orientation = orientation;
	updateGlobal(id);
}

void SceneGraph::setGlobalScale(NodeId id, const Vector3& scale)
{
	data.global[id].scale = scale;
	updateGlobal(id);
}

} // namespace Dunjun
