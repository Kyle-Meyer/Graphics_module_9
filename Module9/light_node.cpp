#include "Module9/light_node.hpp"

namespace cg
{

LightNode::LightNode(uint32_t       light_index,
                     const HPoint3 &position,
                     const Color4  &ambient,
                     const Color4  &diffuse,
                     const Color4  &specular)
    : light_index_(light_index), enabled_(true), position_(position), ambient_(ambient),
      diffuse_(diffuse), specular_(specular)
{
    // No additional initialization needed
}

void LightNode::draw(SceneState &scene_state)
{
    // Set the uniform variables for this light source
    // Get the light uniforms for this light index from the scene state
    if(light_index_ < 2) // Only support 2 lights for now
    {
        const LightUniforms &light_uniforms = scene_state.lights[light_index_];

        // Set enabled flag
        glUniform1i(light_uniforms.enabled, enabled_ ? 1 : 0);

        // Set light position/direction
        glUniform4fv(light_uniforms.position, 1, &position_.x);

        // Set light colors
        glUniform4fv(light_uniforms.ambient, 1, &ambient_.r);
        glUniform4fv(light_uniforms.diffuse, 1, &diffuse_.r);
        glUniform4fv(light_uniforms.specular, 1, &specular_.r);
    }

    // Draw children (if any)
    SceneNode::draw(scene_state);
}

void LightNode::set_position(const HPoint3 &position) { position_ = position; }

void LightNode::set_ambient(const Color4 &ambient) { ambient_ = ambient; }

void LightNode::set_diffuse(const Color4 &diffuse) { diffuse_ = diffuse; }

void LightNode::set_specular(const Color4 &specular) { specular_ = specular; }

void LightNode::set_enabled(bool enabled) { enabled_ = enabled; }

uint32_t LightNode::get_light_index() const { return light_index_; }
}
