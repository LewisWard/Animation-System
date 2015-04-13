#version 330 ///< shader version
layout(location = 0) in vec4 position; ///< the vertex co-odinate from VBO
layout(location = 1) in vec4 normal; ///< the UV co-odinate from VBO

uniform mat4 mvp; ///< Model-View-Projection

void main()
{
	// position output
	gl_Position =  mvp * position;
}