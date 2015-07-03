#include <Dunjun/Scene/FaceCamera.hpp>

#include <Dunjun/Scene/SceneNode.hpp>

#include <Dunjun/Math.hpp>

namespace Dunjun
{
void FaceCamera::update(Time dt)
{
	const Vector3 pos    = parent->getGlobalTransform().position;
	const Vector3 camPos = camera->transform.position;

	parent->transform.orientation =
	    conjugate(Math::lookAt<Quaternion>(camPos, pos, up));
}
} // namespace Dunjun
