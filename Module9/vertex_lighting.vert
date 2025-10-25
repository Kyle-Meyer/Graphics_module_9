#version 410 core

// Incoming vertex and normal attributes
// Vertex position attribute
layout (location = 0) in vec3 vtx_position;
// Vertex normal attribute
layout (location = 1) in vec3 vtx_normal;

// PHONG SHADING: Pass interpolated position and normal to fragment shader
layout (location = 0) smooth out vec3 frag_position;  // World space position
layout (location = 1) smooth out vec3 frag_normal;    // World space normal

// Uniforms for matrices
uniform mat4 pvm_matrix;     // Composite projection, view, model matrix
uniform mat4 model_matrix;   // Modeling matrix
uniform mat4 normal_matrix;  // Normal transformation matrix

// Vertex shader for Phong (per-pixel) lighting
// Transforms position and normal to world space and passes them to fragment shader
void main()
{
  // Transform normal to world coords and pass to fragment shader
  frag_normal = normalize(vec3(normal_matrix * vec4(vtx_normal, 0.0)));
  
  // Transform position to world coords and pass to fragment shader
  frag_position = vec3(model_matrix * vec4(vtx_position, 1.0));

  // Convert position to clip coordinates and pass along
  gl_Position = pvm_matrix * vec4(vtx_position, 1.0);
}
