// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 18/04/2015

#version 330 ///< shader version

layout(location = 0) in vec4 position; ///< the vertex co-odinate from VBO
layout(location = 1) in vec2 uv; ///< the UV co-odinate from VBO

out vec2 ps_uv; ///< send uv to fragment shader

void main()
{
	// send uv to fragment shader
	ps_uv = uv;

	// as is a flat image, do not need to worry about a matrix mul here!
	gl_Position = position;
}