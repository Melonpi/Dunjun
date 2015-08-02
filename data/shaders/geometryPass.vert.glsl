#version 120

#include "Common"

attribute vec3 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_color;
attribute vec3 a_normal;
attribute vec3 a_tangent;


varying vec3 v_position_ws;

varying vec3 v_position;
varying vec2 v_texCoord;
varying vec3 v_color;
varying mat3 v_tbnMatrixInv;

uniform mat4 u_camera;
uniform Transform u_transform;

void main()
{
	v_position = a_position;
	v_texCoord = a_texCoord.st;
	v_color = a_color;


	vec3 n = normalize(quaternionRotate(u_transform.orientation, a_normal));
	vec3 t = normalize(quaternionRotate(u_transform.orientation, a_tangent));
	t = normalize(t - dot(t, n) * n);
	vec3 b = cross(t, n);
	v_tbnMatrixInv = transpose(mat3(t, b, n));


	// v' = proj * view * transform * v;
	vec3 pos = a_position;
	pos = u_transform.position + quaternionRotate(u_transform.orientation, u_transform.scale * pos);
	v_position_ws = pos;

	gl_Position = u_camera * vec4(pos, 1.0);
}
