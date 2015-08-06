#version 120

attribute vec3 a_position;
attribute vec2 a_texCoord;

uniform vec2 u_scale;
uniform vec2 u_position;

varying vec2 v_texCoord;

void main()
{
	v_texCoord = a_texCoord;

	vec2 pos = (u_scale * a_position.xy) + u_position;
	gl_Position = vec4(pos.xy, a_position.z, 1.0);
}
