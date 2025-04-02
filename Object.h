// Author: Tony Pan (panjiabe@grinnell.edu)

// A generic object

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include <cmath>
#include "Vector.h"
#include "Ray.h"
#include "Texture.h"

namespace Raytracer
{
  class Object
  {
  public:

    // Create an object according to the parameters
    Object(double ka_r, double ka_g, double ka_b,
           double kd_r, double kd_g, double kd_b,
           double ks_r, double ks_g, double ks_b, double ns,
           double k_reflect, double k_refract, double refract_index,
           char* texture_file, char* bump_map)
    {
      k_ambient = Vector(ka_r, ka_g, ka_b);
      k_diffuse = Vector(kd_r, kd_g, kd_b);
      k_specular = Vector(ks_r, ks_g, ks_b);
      specular_coeff = ns;
      this->k_reflect = k_reflect;
      this->k_refract = k_refract;
      this->refract_index = refract_index;
      texture = new Texture(texture_file);
      this->bump_map = new Texture(bump_map);
    }

    // Free memory used by texture and bump map
    ~Object()
    {
      delete texture;
      delete bump_map;
    }
    
    virtual double intersect(Ray ray) = 0;
    virtual Vector normal(Vector position, bool apply_bump) = 0;
    virtual Color texture_at(Vector position) = 0;
    virtual Vector map_to_texture(Vector position) = 0;
    Vector k_ambient, k_diffuse, k_specular;
    double specular_coeff, k_reflect, k_refract, refract_index;
  
  protected:
    char* obj_type; // A string indicating the type of the object
    Texture *texture, *bump_map; // Texture and bump map
  };
};

#endif
