#ifndef DUNJUN_TRANSFORM_HPP
#define DUNJUN_TRANSFORM_HPP

#include <Dunjun/Math/Types.hpp>

namespace Dunjun
{
struct Transform
{
	Vector3 position       = Vector3::Zero;
	Quaternion orientation = Quaternion::Identity;
	Vector3 scale          = Vector3{1, 1, 1};
};

// World = Parent * Local
Transform operator*(const Transform& ps, const Transform& ls);
Transform& operator*=(Transform& ps, const Transform& ls);
// Local = World / Parent
Transform operator/(const Transform& ws, const Transform& ps);
Transform& operator/=(Transform& ws, const Transform& ps);

Vector3 transformPoint(const Transform& transform, const Vector3& point);

Transform inverse(const Transform& t);
Matrix4 transformMatrix4(const Transform& t);
} // namespace Dunjun
#endif
