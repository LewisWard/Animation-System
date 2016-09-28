// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 18/04/2015
// © 2015-2016 Lewis Ward. All rights reserved.

#version 330 ///< shader version

uniform sampler2D texture; ///< texture
uniform vec4 ambient; ///< ambient
uniform vec4 diffuse; ///< diffuse
uniform vec4 specular; ///< specular
uniform float textureRepeat; ///< specular

out vec4 output_colour; ///< output colour

in vec3 ps_light; // light direction
in vec3 ps_eye; // eye direction
in vec3 ps_normal; // normal vector
in vec2 ps_uv; // uv

void main()
{
	// make sure the vectors are normalized
	vec3 L = normalize(ps_light);
	vec3 E = normalize(ps_eye);
	vec3 N = normalize(ps_normal);

	// compute the dot product between the normal and light
	float dotNL = max(dot(N, L), 0.0);

	// compute ambient term
	vec4 termAmbient = ambient;

	// compute specular term
	vec4 termSpecular = specular;

	// compute diffuse term (texture can be repeated)
	vec4 termDiffuse = texture2D(texture, ps_uv * textureRepeat) * diffuse * dotNL;

	// output colour
	output_colour = termDiffuse; +termAmbient + termSpecular;

}