#include "Module9/light_node.hpp"

namespace cg
{

// Constructor for regular light (point or directional)
LightNode::LightNode(uint32_t       light_index,
                     const HPoint3 &position,
                     const Color4  &ambient,
                     const Color4  &diffuse,
                     const Color4  &specular)
    : light_index_(light_index), enabled_(true), is_spotlight_(false), position_(position),
      ambient_(ambient), diffuse_(diffuse), specular_(specular),
      spot_direction_(0.0f, 0.0f, -1.0f), spot_cutoff_(180.0f), spot_exponent_(0.0f)
{
    // Regular light - not a spotlight
}

LightNode::LightNode(uint32_t        light_index,
                     const HPoint3  &position,
                     const Color4   &ambient,
                     const Color4   &diffuse,
                     const Color4   &specular,
                     const Vector3  &spot_direction,
                     float           spot_cutoff,
                     float           spot_exponent)
    : light_index_(light_index), enabled_(true), is_spotlight_(true), position_(position),
      ambient_(ambient), diffuse_(diffuse), specular_(specular),
      spot_direction_(spot_direction), spot_cutoff_(spot_cutoff), spot_exponent_(spot_exponent)
{
    // Spotlight constructor
}

void LightNode::draw(SceneState &scene_state)
{
    // Set the uniform variables for this light source
    // Get the light uniforms for this light index from the scene state
    if(light_index_ < 3) // STEP 5: Now support up to 3 lights
    {
        const LightUniforms &light_uniforms = scene_state.lights[light_index_];

        // Set enabled flag
        glUniform1i(light_uniforms.enabled, enabled_ ? 1 : 0);

        // STEP 5: Set spotlight flag
        glUniform1i(light_uniforms.spotlight, is_spotlight_ ? 1 : 0);

        // Set light position/direction
        glUniform4fv(light_uniforms.position, 1, &position_.x);

        // Set light colors
        glUniform4fv(light_uniforms.ambient, 1, &ambient_.r);
        glUniform4fv(light_uniforms.diffuse, 1, &diffuse_.r);
        glUniform4fv(light_uniforms.specular, 1, &specular_.r);

        if(is_spotlight_)
        {
            glUniform3fv(light_uniforms.spot_direction, 1, &spot_direction_.x);
            glUniform1f(light_uniforms.spot_cutoff, spot_cutoff_);
            glUniform1f(light_uniforms.spot_exponent, spot_exponent_);

        }
    }

    // Draw children (if any)
    SceneNode::draw(scene_state);
}

void LightNode::set_position(const HPoint3 &position) { position_ = position; }

void LightNode::set_ambient(const Color4 &ambient) { ambient_ = ambient; }

void LightNode::set_diffuse(const Color4 &diffuse) { diffuse_ = diffuse; }

void LightNode::set_specular(const Color4 &specular) { specular_ = specular; }

void LightNode::set_enabled(bool enabled) { enabled_ = enabled; }

void LightNode::set_spot_direction(const Vector3 &direction) 
{ 
    spot_direction_ = direction;
}

void LightNode::set_spot_cutoff(float cutoff) 
{ 
    spot_cutoff_ = cutoff; 
}

void LightNode::set_spot_exponent(float exponent) 
{ 
    spot_exponent_ = exponent; 
}

uint32_t LightNode::get_light_index() const { return light_index_; }

bool LightNode::is_spotlight() const { return is_spotlight_; }

} // namespace cg
