#ifndef COMMON_HEAD_GLSL
#define COMMON_HEAD_GLSL

struct Transform
{
	vec3 position;
	vec4 orientation;
	vec3 scale;
};

vec3 quaternionRotate(in vec4 q, in vec3 v)
{
	vec3 t = 2.0 * cross(q.xyz, v);
	return (v + q.w * t + cross(q.xyz, t));
}

float square(in float x) { return x * x; }

vec3 calculatePositionFromDepth(in vec2 st, in float w, in float depth, in mat4 inverseMatrix)
{
	float x_ss = 2.0 * st.x - 1.0;
	float y_ss = 2.0 * st.y - 1.0;
	float z_ss = 2.0 * depth - 1.0;

	vec4 position_ss = vec4(x_ss, y_ss, z_ss, 1.0) / w;

	vec4 position_ws = inverseMatrix * position_ss;

	return position_ws.xyz / position_ws.w;
}

#endif
