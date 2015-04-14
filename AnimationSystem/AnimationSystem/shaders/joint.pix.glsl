// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 04/04/2015

#version 330 ///< shader version

out vec4 output_colour; ///< output colour

in vec3 ps_light; // light direction
in vec3 ps_eye; // eye direction
in vec3 ps_normal; // normal vector

void main()
{
  // make sure the vectors are normalized
  vec3 L = normalize(ps_light);
  vec3 E = normalize(ps_eye);
  vec3 N = normalize(ps_normal);

  // compute the dot product between the normal and light
  float dotNL = max(dot(N, L), 0.0);

  // output colour
  output_colour = vec4(0.0, 1.0, 1.0, 1.0) * dotNL;
}