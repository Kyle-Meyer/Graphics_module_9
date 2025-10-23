#ifndef __MODULE8_SHADER_SRC_HPP__
#define __MODULE8_SHADER_SRC_HPP__

const char *vertex_lighting_vert =
    R"(
#version 410 core
layout (location = 0) in vec3 vtx_position;
layout (location = 1) in vec3 vtx_normal;
layout (location = 0) smooth out vec4 color;

uniform vec4   material_ambient;
uniform vec4   material_diffuse;
uniform vec4   material_specular;
uniform vec4   material_emission;
uniform float  material_shininess;
uniform vec4  global_light_ambient;
uniform vec3  camera_position;
uniform mat4 pvm_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform int num_lights;

const int MAX_LIGHTS = 2; 
struct LightSource
{
  int  enabled;
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
uniform LightSource lights[MAX_LIGHTS]; 

void main()
{
  vec3 N = normalize(vec3(normal_matrix * vec4(vtx_normal, 0.0)));
  vec3 v = vec3((model_matrix * vec4(vtx_position, 1.0)));
  vec3 V = normalize(camera_position - v);

  vec4 ambient  = vec4(0.0);
  vec4 diffuse  = vec4(0.0);
  vec4 specular = vec4(0.0);

  for (int i = 0; i < num_lights; i++)
  {
    if (lights[i].enabled == 1)
    {
      vec3 L;
      if (lights[i].position.w == 0.0) L = normalize(lights[i].position.xyz);
      else L = normalize(lights[i].position.xyz - v);

      ambient += lights[i].ambient;

      float nDotL = dot(N, L);
      if (nDotL > 0.0)
      {
        diffuse  += lights[i].diffuse  * nDotL;
        vec3 H = normalize(L + V);

        float nDotH = dot(N, H);
        if (nDotH > 0.0) specular += lights[i].specular * pow(nDotH, material_shininess);
      }
    }
  }

  color = material_emission + global_light_ambient * material_ambient +
          (ambient  * material_ambient) + (diffuse  * material_diffuse) + (specular * material_specular);
  gl_Position = pvm_matrix * vec4(vtx_position, 1.0);
}
)";

const char *vertex_lighting_frag =
    R"(
#version 410 core
layout (location = 0) smooth in vec4 color;
layout (location = 0) out vec4 fragColor;
void main() 
{
    fragColor = color;
}
)";

#endif
