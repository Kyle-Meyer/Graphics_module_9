#ifndef __SCENE_LIGHT_NODE_HPP__
#define __SCENE_LIGHT_NODE_HPP__

#include "scene/scene_node.hpp"
#include "scene/color4.hpp"
#include "geometry/hpoint3.hpp"
#include "geometry/vector3.hpp"

namespace cg
{

/**
 * Light node - represents a light source in the scene graph.
 * When drawn, it sets the uniform variables for its light source.
 * STEP 5: Now supports spotlights.
 */
class LightNode : public SceneNode
{
  public:
    /**
     * Constructor for regular light (point or directional).
     * @param  light_index  Index of this light (0, 1, 2, etc.)
     * @param  position     Position/direction of the light (w=1 for point, w=0 for directional)
     * @param  ambient      Ambient color/intensity
     * @param  diffuse      Diffuse color/intensity
     * @param  specular     Specular color/intensity
     */
    LightNode(uint32_t       light_index,
              const HPoint3 &position,
              const Color4  &ambient,
              const Color4  &diffuse,
              const Color4  &specular);

    /**
     * STEP 5: Constructor for spotlight.
     * @param  light_index     Index of this light (0, 1, 2, etc.)
     * @param  position        Position of the spotlight (w should be 1)
     * @param  ambient         Ambient color/intensity
     * @param  diffuse         Diffuse color/intensity
     * @param  specular        Specular color/intensity
     * @param  spot_direction  Direction the spotlight is aimed
     * @param  spot_cutoff     Cutoff angle in degrees
     * @param  spot_exponent   Spotlight exponent (controls falloff)
     */
    LightNode(uint32_t        light_index,
              const HPoint3  &position,
              const Color4   &ambient,
              const Color4   &diffuse,
              const Color4   &specular,
              const Vector3  &spot_direction,
              float           spot_cutoff,
              float           spot_exponent);

    /**
     * Draw method - sets the light uniform variables in the shader.
     * @param  scene_state  Current scene state containing uniform locations.
     */
    void draw(SceneState &scene_state) override;

    /**
     * Set the position/direction of the light.
     * @param  position  Position/direction (w=1 for point, w=0 for directional)
     */
    void set_position(const HPoint3 &position);

    /**
     * Set the ambient color/intensity.
     * @param  ambient  Ambient color/intensity
     */
    void set_ambient(const Color4 &ambient);

    /**
     * Set the diffuse color/intensity.
     * @param  diffuse  Diffuse color/intensity
     */
    void set_diffuse(const Color4 &diffuse);

    /**
     * Set the specular color/intensity.
     * @param  specular  Specular color/intensity
     */
    void set_specular(const Color4 &specular);

    /**
     * Enable or disable this light.
     * @param  enabled  True to enable, false to disable
     */
    void set_enabled(bool enabled);

    /**
     * STEP 5: Set spotlight direction.
     * @param  direction  Direction the spotlight is aimed
     */
    void set_spot_direction(const Vector3 &direction);

    /**
     * STEP 5: Set spotlight cutoff angle.
     * @param  cutoff  Cutoff angle in degrees
     */
    void set_spot_cutoff(float cutoff);

    /**
     * STEP 5: Set spotlight exponent.
     * @param  exponent  Spotlight exponent (controls falloff)
     */
    void set_spot_exponent(float exponent);

    /**
     * Get the light index.
     * @return  Returns the index of this light source.
     */
    uint32_t get_light_index() const;

    /**
     * STEP 5: Check if this is a spotlight.
     * @return  Returns true if this is a spotlight.
     */
    bool is_spotlight() const;

  protected:
    uint32_t light_index_;      // Index of this light (0, 1, 2, etc.)
    bool     enabled_;          // Whether this light is enabled
    bool     is_spotlight_;     // STEP 5: Whether this is a spotlight
    HPoint3  position_;         // Position/direction of the light
    Color4   ambient_;          // Ambient color/intensity
    Color4   diffuse_;          // Diffuse color/intensity
    Color4   specular_;         // Specular color/intensity
    Vector3  spot_direction_;   // STEP 5: Spotlight direction
    float    spot_cutoff_;      // STEP 5: Spotlight cutoff angle (degrees)
    float    spot_exponent_;    // STEP 5: Spotlight exponent
};

} // namespace cg

#endif
