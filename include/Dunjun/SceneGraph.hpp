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
		usize length   = 0;
		usize capacity = 0;
		void* buffer   = nullptr;

		EntityId* entityId  = nullptr;
		Transform* local    = nullptr;
		Transform* world    = nullptr;
		NodeId* parent      = nullptr;
		NodeId* firstChild  = nullptr;
		NodeId* prevSibling = nullptr;
		NodeId* nextSibling = nullptr;
	};

	Allocator& allocator;
	InstanceData data;
	Hashmap<NodeId> map;

	SceneGraph(Allocator& a);
	~SceneGraph();

	void allocate(usize capacity);

	void create(EntityId id, const Transform& t);
	void destroy(NodeId id);

	NodeId getTransformId(EntityId id) const;

	bool isValid(NodeId id) const;

	void link(NodeId parent, NodeId child);
	void unlink(NodeId child);

	void transformChild(NodeId child, const Transform& t);


} // namespace Dunjun

#endif
