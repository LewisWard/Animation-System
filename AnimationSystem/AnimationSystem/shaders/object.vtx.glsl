// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 18/04/2015
// � 2015-2016 Lewis Ward. All rights reserved.

#version 330 ///< shader version
layout(location = 0) in vec4 position; ///< the vertex co-odinate from VBO
layout(location = 1) in vec3 normal; ///< the normal from VBO
layout(location = 2) in vec2 uv; ///< the UV co-odinate from VBO

uniform mat4 mvp; ///< Model-View-Projection
uniform mat4 mv;  ///< Model-View

out vec3 ps_light; // light direction
out vec3 ps_eye; // eye direction (for the moment will be the same as the light!)
out vec3 ps_normal; // normal vector
out vec2 ps_uv; // uv

void main()
{
	// transform the normal into viewspace
	ps_normal = (mv * vec4(normal, 0.0)).xyz;

	// send uv to fragment shader
	ps_uv = uv;

	// compute vector from light to position
	vec4 L = vec4(0, 0, 0, 0) - mv * normalize(position);

	// pass to fragment shader
	ps_light = L.xyz;
	ps_eye = L.xyz; ///< same as light for the moment!

	// position output
	gl_Position = mvp * position;
}