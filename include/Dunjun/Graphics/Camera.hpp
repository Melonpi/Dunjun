#ifndef DUNJUN_GRAPHICS_CAMERA_HPP
#define DUNJUN_GRAPHICS_CAMERA_HPP

#include <Dunjun/Math/Types.hpp>
#include <Dunjun/Math/Angle.hpp>
#include <Dunjun/Graphics/Transform.hpp>

namespace Dunjun
{
enum class ProjectionType
{
	Perspective,
	InfinitePerspective,
	Orthographic,
};

struct Camera
{
	Transform transform;
	Radian fieldOfView;
	f32 orthoScale;
	f32 nearPlane;
	f32 farPlane;
	f32 viewportAspectRatio;
	ProjectionType projectionType;
};

void cameraLookAt(Camera& camera,
                  const Vector3& position,
                  const Vector3& up = {0, 1, 0});

// NOTE(bill): +ve == ccw => +ve yaw == face up, +ve pitch == face left
// Using Right-Handed Coordinate System
void offsetOrientation(Quaternion& orientation,
                       const Radian& yaw,
                       const Radian& pitch);

Vector3 forwardVector(const Quaternion& orientation);
Vector3 backwardVector(const Quaternion& orientation);

Vector3 rightVector(const Quaternion& orientation);
Vector3 leftVector(const Quaternion& orientation);

Vector3 upVector(const Quaternion& orientation);
Vector3 downVector(const Quaternion& orientation);

Matrix4 cameraMatrix(const Camera& camera);
Matrix4 cameraProjection(const Camera& camera);
Matrix4 cameraView(const Camera& camera);
} // namespace Dunjun

#endif
