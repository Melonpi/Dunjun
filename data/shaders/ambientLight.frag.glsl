#version 120

#include "Lighting"

uniform BaseLight u_light;

void main()
{
	gl_FragColor = vec4(u_light.intensities, 0.0);
}
