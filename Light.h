// Author: Tony Pan (panjiabe@grinnell.edu)

// A light source modeled as a sphere

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "Vector.h"

namespace Raytracer
{
  class Light 
  {
  public:
    Light(double x, double y, double z, double r, double g, double b,
          double radius)
    {
      position = Vector(x, y, z);
      color = Color(r, g, b);
      this->r = radius; // Cannot change to r = radius because a parameter
                        // is named r
    }
    Vector position; // Light position
    Color color; // Light color
    double r; // Light radius
  };
};

#endif
