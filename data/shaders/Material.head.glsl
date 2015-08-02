#ifndef MATERIAL_HEAD_GLSL
#define MATERIAL_HEAD_GLSL

struct Material
{
	sampler2D diffuseMap;
	vec4 diffuseColor;

	sampler2D normalMap;

	vec4 specularColor;
	float specularExponent;
};

#endif
