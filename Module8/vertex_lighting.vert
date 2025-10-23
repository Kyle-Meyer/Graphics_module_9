#version 410 core

// Incoming vertex and normal attributes
// Vertex position attribute
layout (location = 0) in vec3 vtx_position;
// Vertex normal attribute
layout (location = 1) in vec3 vtx_normal;

// Per vertex lighting - vertex shader. Outputs a varying (intepolated) color to the 
// fragment shader
layout (location = 0) smooth out vec4 color;

// Uniforms for material properties
uniform vec4   material_ambient;
uniform vec4   material_diffuse;
uniform vec4   material_specular;
uniform vec4   material_emission;
uniform float  material_shininess;

// Global lighting environment ambient intensity
uniform vec4  global_light_ambient;

// Global camera position
uniform vec3  camera_position;

// Uniforms for matrices
uniform mat4 pvm_matrix;		// Composite projection, view, model matrix
uniform mat4 model_matrix;	// Modeling  matrix
uniform mat4 normal_matrix;	// Normal transformation matrix

// Uniform to constrian the number of lights the application uses
uniform int num_lights;

// Structure for a light source. Allow up to 8 lights. No attenuation or spotlight
// support yet.
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

// Simple shader for per vertex lighting.
void main()
{
  // Transform normal and position to world coords. 
  vec3 N = normalize(vec3(normal_matrix * vec4(vtx_normal, 0.0)));
  vec3 v = vec3((model_matrix * vec4(vtx_position, 1.0)));

  // Construct a unit length vector from the vertex to the camera 
  vec3 V = normalize(camera_position - v);

  // Iterate through all lights to determine the illumination striking this vertex
  vec4 ambient  = vec4(0.0);
  vec4 diffuse  = vec4(0.0);
  vec4 specular = vec4(0.0);

  for (int i = 0; i < num_lights; i++)
  {
    if (lights[i].enabled == 1)
    {
      vec3 L;
      if (lights[i].position.w == 0.0) {
        // Directional Light
        L = normalize(lights[i].position.xyz);
      } else {
        // Construct a vector from the vertex to the light source. Normalize that vector (L)
        L = normalize(lights[i].position.xyz - v);
      }

      // Add the light source ambient contribution
      ambient += lights[i].ambient;

      // Determine dot product of normal with L. If < 0 the light is not 
      // incident on the front face of the surface.
      float nDotL = dot(N, L);
      if (nDotL > 0.0)
      {
        // Add diffuse contribution of this light source
        diffuse  += lights[i].diffuse  * nDotL;

        // Construct the halfway vector
        vec3 H = normalize(L + V);

        // Add specular contribution (if N dot H > 0)
        float nDotH = dot(N, H);
        if (nDotH > 0.0)
            specular += lights[i].specular * pow(nDotH, material_shininess);

        /*
        // Lets use the reflection vector!
        vec3 R = reflect(-L, N);
        
        // Add specular contribution (if R dot V > 0)
        float rDotV = dot(R, V);
        if (rDotV > 0.0) specular += lights[i].specular * pow(rDotV, material_shininess); 
        */

      }
    }
  }

  // Compute color. Emmission + global ambient contribution + light sources ambient, diffuse,
  // and specular contributions
  color = material_emission + global_light_ambient * material_ambient +
          (ambient  * material_ambient) + (diffuse  * material_diffuse) + (specular * material_specular);

  // Convert position to clip coordinates and pass along
  gl_Position = pvm_matrix * vec4(vtx_position, 1.0);
}
