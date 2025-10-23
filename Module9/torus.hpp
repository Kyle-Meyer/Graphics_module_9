#ifndef __SCENE_TORUS_HPP__
#define __SCENE_TORUS_HPP__

#include "scene/tri_surface.hpp"

namespace cg
{

/**
 * Torus surface geometry node.
 */
class TorusSurface : public TriSurface
{
  public:
    /**
     * Creates a torus surface by rotating a circle (tube) around an axis
     * at a specified distance (ring radius) from the center.
     * @param   ring_radius    Distance from center of torus to center of tube
     * @param   tube_radius    Radius of the tube
     * @param   num_ring_sides Number of subdivisions around the ring (main circle)
     * @param   num_tube_sides Number of subdivisions around the tube
     * @param   position_loc   Location of position attribute in shader
     * @param   normal_loc     Location of normal attribute in shader
     */
    TorusSurface(float    ring_radius,
                 float    tube_radius,
                 uint32_t num_ring_sides,
                 uint32_t num_tube_sides,
                 int32_t  position_loc,
                 int32_t  normal_loc);

  private:
    uint32_t num_rows_;
    uint32_t num_cols_;

    // Make default constructor private to force use of the constructor
    // with parameters.
    TorusSurface();
};

} // namespace cg

#endif
