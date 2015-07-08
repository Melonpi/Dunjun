#ifndef DUNJUN_SCENE_LIGHTING_HPP
#define DUNJUN_SCENE_LIGHTING_HPP

#include <Dunjun/Math/Types.hpp>
#include <Dunjun/Math/Functions.hpp>
#include <Dunjun/Graphics/Color.hpp>

namespace Dunjun
{
struct Attenuation
{
	f32 constant  = 1.0f;
	f32 linear    = 0.0f;
	f32 quadratic = 1.0f;
};

struct BaseLight
{
	Color color   = Color{255, 255, 255, 255};
	f32 intensity = 1.0f;
};

struct DirectionalLight : BaseLight
{
	Vector3 direction;
};

struct PointLight : BaseLight
{
	Vector3 position        = Vector3{0, 0, 0};
	Attenuation attenuation = {};
	mutable f32 range       = 16.0f;
};

inline f32 calculateLightRange(const PointLight& l)
{
	f32 i =
	    l.intensity * (f32)std::max(l.color.r, std::max(l.color.g, l.color.b));
	f32 r = -l.attenuation.linear +
	        Math::sqrt(l.attenuation.linear * l.attenuation.linear -
	                   4.0f * l.attenuation.quadratic *
	                       (l.attenuation.constant - i));
	r /= 2.0f * l.attenuation.quadratic;

	return r;
}

struct SpotLight : PointLight
{
	Radian coneAngle  = Degree{20.0f};
	Vector3 direction = Vector3{0, 0, -1};
};
} // namespace Dunjun

#endif
