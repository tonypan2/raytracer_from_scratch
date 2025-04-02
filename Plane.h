// Author: Tony Pan (panjiabe@grinnell.edu)

// A plane

#ifndef RAYTRACER_PLANE_H
#define RAYTRACER_PLANE_H

#include "Object.h"

namespace Raytracer
{
  class Plane : public Object
  {
  public:
    Plane(double ka_r, double ka_g, double ka_b,
          double kd_r, double kd_g, double kd_b,
          double ks_r, double ks_g, double ks_b, double ns, double k_reflect,
          double k_refract, double refract_index, char *texture_file,
          char *bump_map,
          double px, double py, double pz, double nx, double ny, double nz);
    double intersect(Ray ray);
    Vector normal(Vector position, bool apply_bump);
    Color texture_at(Vector position);
    Vector map_to_texture(Vector position);

  private:
    Vector point; // A point on the plane
    Vector norm;  // The normal vector
    Vector u, v;  // U-V coordinate system of the texture
  };
};

#endif
