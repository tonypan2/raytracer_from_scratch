// Author: Tony Pan (panjiabe@grinnell.edu)

// A sphere

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Object.h"
#include "Raytracer.h"

namespace Raytracer
{
  class Sphere : public Object
  {
  public:
    Sphere(double ka_r, double ka_g, double ka_b,
           double kd_r, double kd_g, double kd_b,
           double ks_r, double ks_g, double ks_b, double ns,
           double k_reflect, double k_refract, double refract_index,
           char* texture_file, char* bump_map,
           double x, double y, double z, double r);
    double intersect(Ray ray);
    Vector normal(Vector position, bool apply_bump);
    Color texture_at(Vector position);
    Vector map_to_texture(Vector position);

  private:
    double r; // Radius

    Vector center; // Center
    Vector top; // The vector pointing from the center to the north pole
    Vector side; // A vector pointing from the center to the equator
  };
};

#endif
