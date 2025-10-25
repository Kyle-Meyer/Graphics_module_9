#version 410 core

// PHONG SHADING: Incoming interpolated position and normal from vertex shader
layout (location = 0) smooth in vec3 frag_position;  // World space position
layout (location = 1) smooth in vec3 frag_normal;    // World space normal

// Output fragment color
layout (location = 0) out vec4 frag_color;

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

// Uniform to constrain the number of lights the application uses
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

// Fragment shader for Phong (per-pixel) lighting
// Performs lighting calculations for each fragment
void main()
{
  // Normalize the interpolated normal (interpolation can change length)
  vec3 N = normalize(frag_normal);
  
  // Construct a unit length vector from the fragment to the camera
  vec3 V = normalize(camera_position - frag_position);

  // Iterate through all lights to determine the illumination at this fragment
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
        // Construct a vector from the fragment to the light source. Normalize that vector (L)
        L = normalize(lights[i].position.xyz - frag_position);
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
        // Alternatively, use the reflection vector!
        vec3 R = reflect(-L, N);
        
        // Add specular contribution (if R dot V > 0)
        float rDotV = dot(R, V);
        if (rDotV > 0.0) specular += lights[i].specular * pow(rDotV, material_shininess); 
        */
      }
    }
  }

  // Compute color. Emission + global ambient contribution + light sources ambient, diffuse,
  // and specular contributions
  frag_color = material_emission + global_light_ambient * material_ambient +
               (ambient  * material_ambient) + (diffuse  * material_diffuse) + (specular * material_specular);
}
