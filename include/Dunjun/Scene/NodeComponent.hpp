#ifndef DUNJUN_SCENE_NODECOMPONENT_HPP
#define DUNJUN_SCENE_NODECOMPONENT_HPP

#include <Dunjun/Common.hpp>
#include <Dunjun/Graphics/Transform.hpp>
#include <Dunjun/Scene/SceneRenderer.hpp>
#include <Dunjun/Core/Time.hpp>
#include <Dunjun/Types.hpp>
#include <Dunjun/Window/Event.hpp>

namespace Dunjun
{
class SceneNode;
class NodeComponent
{
public:
	using UPtr = std::unique_ptr<NodeComponent>;

	SceneNode* parent = nullptr;

	NodeComponent() = default;
	virtual ~NodeComponent() {}

	virtual void init() {}
	virtual void update(Time dt) {}
	virtual void handleEvent(const Event& event) {}
	virtual void draw(SceneRenderer& renderer, Transform t) const {}
private:
	NodeComponent(const NodeComponent&) = delete;
	NodeComponent& operator=(const NodeComponent&) = delete;
};
} // namespace Dunjun

#endif
