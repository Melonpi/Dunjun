#ifndef DUNJUN_SCENEGRAPH_HPP
#define DUNJUN_SCENEGRAPH_HPP

#include <Dunjun/Entity.hpp>
#include <Dunjun/Graphics/Transform.hpp>
#include <Dunjun/System/Allocator.hpp>
#include <Dunjun/System/Containers.hpp>

namespace Dunjun
{
struct SceneGraph
{
	using NodeId = u32;

	GLOBAL const NodeId EmptyNodeId = (NodeId)(-1);

	struct InstanceData
	{
		u32 length   = 0;
		u32 capacity = 0;
		void* buffer = nullptr;

		EntityId* entityId  = nullptr;
		Transform* local    = nullptr;
		Transform* global   = nullptr;
		NodeId* parent      = nullptr;
		NodeId* firstChild  = nullptr;
		NodeId* prevSibling = nullptr;
		NodeId* nextSibling = nullptr;
	};

	Allocator& allocator;
	InstanceData data;
	HashMap<NodeId> map;

	// Code

	SceneGraph(Allocator& a);
	~SceneGraph();

	void allocate(u32 capacity);

	NodeId create(EntityId id, const Transform& t);
	void destroy(NodeId id);

	NodeId getNodeId(EntityId id) const;
	bool isValid(NodeId id) const;
	u32 nodeCount() const;

	void link(NodeId parentId, NodeId childId);
	void unlink(NodeId childId);

	void transformChild(NodeId childId, const Transform& t);

	// Helper Functions

	void updateLocal(NodeId id);
	void updateGlobal(NodeId id);

	Transform getLocalTransform(NodeId id) const;
	Transform getGlobalTransform(NodeId id) const;

	Vector3 getLocalPosition(NodeId id) const;
	Quaternion getLocalOrientation(NodeId id) const;
	Vector3 getLocalScale(NodeId id) const;

	Vector3 getGlobalPosition(NodeId id) const;
	Quaternion getGlobalOrientation(NodeId id) const;
	Vector3 getGlobalScale(NodeId id) const;

	void setLocalTransform(NodeId id, const Transform& t);
	void setGlobalTransform(NodeId id, const Transform& t);

	void setLocalPosition(NodeId id, const Vector3& position);
	void setLocalOrientation(NodeId id, const Quaternion& orientation);
	void setLocalScale(NodeId id, const Vector3& scale);

	void setGlobalPosition(NodeId id, const Vector3& position);
	void setGlobalOrientation(NodeId id, const Quaternion& orientation);
	void setGlobalScale(NodeId id, const Vector3& scale);
};
} // namespace Dunjun

#endif
