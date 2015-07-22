#include <Dunjun/Graphics/Camera.hpp>

#include <Dunjun/Math.hpp>

namespace Dunjun
{
void cameraLookAt(Camera& c, const Vector3& position, const Vector3& up)
{
	c.transform.orientation =
	    conjugate(Math::lookAtQuaternion(c.transform.position, position, up));
}

void offsetOrientation(Quaternion& orientation,
                       const Radian& yaw,
                       const Radian& pitch)
{
	// NOTE(bill): absolute up
	const Quaternion yawRot = angleAxis(yaw, {0, 1, 0});
	// NOTE(bill): relative right
	const Quaternion pitchRot = angleAxis(pitch, rightVector(orientation));

	orientation = yawRot * pitchRot * orientation;
}

Vector3 forwardVector(const Quaternion& orientation)
{
	return orientation * Vector3{0, 0, -1};
}

Vector3 backwardVector(const Quaternion& orientation)
{
	return orientation * Vector3{0, 0, +1};
}

Vector3 rightVector(const Quaternion& orientation)
{
	return orientation * Vector3{+1, 0, 0};
}

Vector3 leftVector(const Quaternion& orientation)
{
	return orientation * Vector3{-1, 0, 0};
}

Vector3 upVector(const Quaternion& orientation)
{
	return orientation * Vector3{0, +1, 0};
}

Vector3 downVector(const Quaternion& orientation)
{
	return orientation * Vector3{0, -1, 0};
}

Matrix4 cameraMatrix(const Camera& c)
{
	return cameraProjection(c) * cameraView(c);
}

Matrix4 cameraProjection(const Camera& c)
{

	switch (c.projectionType)
	{
	case ProjectionType::Perspective:
	{
		return Math::perspective(
		    c.fieldOfView, c.viewportAspectRatio, c.nearPlane, c.farPlane);
	}
	case ProjectionType::InfinitePerspective:
	{
		return Math::infinitePerspective(
		    c.fieldOfView, c.viewportAspectRatio, c.nearPlane);
	}
	case ProjectionType::Orthographic:
	{
		const f32 distance = 0.5f * (c.farPlane - c.nearPlane);

		return Math::ortho(-c.orthoScale * c.viewportAspectRatio,
		                   c.orthoScale * c.viewportAspectRatio,
		                   -c.orthoScale,
		                   c.orthoScale,
		                   -distance,
		                   distance);
	}
	}

	return Matrix4::Identity;
}

Matrix4 cameraView(const Camera& c)
{
	return Math::scale(Vector3{1, 1, 1} / c.transform.scale) *
	       quaternionToMatrix4(conjugate(c.transform.orientation)) *
	       Math::translate(-c.transform.position);
}
} // namespace Dunjun
