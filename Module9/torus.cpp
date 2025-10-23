#include "torus.hpp"

#include "geometry/geometry.hpp"

#include <cmath>

namespace cg
{
   TorusSurface::TorusSurface() {}
   TorusSurface::TorusSurface(float ring_radius,
                              float tube_radius,
                              uint32_t num_ring_sides,
                              uint32_t num_tube_sides,
                              int32_t position_loc,
                              int32_t normal_loc)
   {
      if(ring_radius <= 0.0f || tube_radius <= 0.0f) return;
      
      //basically rotate circle around the z axis
      num_rows_ = num_tube_sides + 1;
      num_cols_ = num_ring_sides + 1;

      // Angular increments
      float d_theta = (2.0f * PI) / static_cast<float>(num_ring_sides);  // Around the ring
      float d_phi = (2.0f * PI) / static_cast<float>(num_tube_sides);    // Around the tube

      VertexAndNormal vtx;

      // Generate vertices by iterating around the ring (theta) and tube (phi)
      for(uint32_t i = 0; i < num_ring_sides; i++)
      {
          float theta = i * d_theta;
          float cos_theta = std::cos(theta);
          float sin_theta = std::sin(theta);

          for(uint32_t j = 0; j <= num_tube_sides; j++)
          {
              float phi = j * d_phi;
              float cos_phi = std::cos(phi);
              float sin_phi = std::sin(phi);

              // Position: point on the tube circle, then rotated around the ring
              // x = (R + r*cos(phi)) * cos(theta)
              // y = (R + r*cos(phi)) * sin(theta)
              // z = r * sin(phi)
              float radius_at_phi = ring_radius + tube_radius * cos_phi;
              vtx.vertex.x = radius_at_phi * cos_theta;
              vtx.vertex.y = radius_at_phi * sin_theta;
              vtx.vertex.z = tube_radius * sin_phi;

              // Normal: points from the center of the tube circle to the vertex
              // The center of the tube circle at this theta is at (R*cos(theta), R*sin(theta), 0)
              // Normal direction is (r*cos(phi)*cos(theta), r*cos(phi)*sin(theta), r*sin(phi))
              // After normalization, it's just (cos(phi)*cos(theta), cos(phi)*sin(theta), sin(phi))
              vtx.normal.x = cos_phi * cos_theta;
              vtx.normal.y = cos_phi * sin_theta;
              vtx.normal.z = sin_phi;
              vtx.normal.normalize();

              vertices_.push_back(vtx);
          }
      }

      // Copy the first column of vertices to close the ring
      for(uint32_t j = 0; j < num_rows_; j++)
      {
          vertices_.push_back(vertices_[j]);
      }

      // Construct the face list using the row-column pattern
      // Rows are around the tube, columns are around the ring
      construct_row_col_face_list(num_cols_, num_rows_);
      create_vertex_buffers(position_loc, normal_loc);
   }
}
