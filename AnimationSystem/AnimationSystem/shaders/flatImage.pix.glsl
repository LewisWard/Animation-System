// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 18/04/2015
// © 2015-2016 Lewis Ward. All rights reserved.

#version 330 ///< shader version

uniform sampler2D texture; ///< texture

in vec2 ps_uv; ///< uv from vertex shader

out vec4 ps_colour; ///< output colour

void main()
{
	// output texture
	ps_colour = texture2D(texture, ps_uv);
}