// Author: Tony Pan (panjiabe@grinnell.edu)

// A ray of light

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Vector.h"

namespace Raytracer
{
  class Ray
  {
  public:
    Ray()
    {
    }

    Ray(const Vector start, const Vector direction)
    {
      this->start = start;
      this->direction = direction;
    }

    Vector start, direction; // The starting point and the direction
                             // of the ray
  };
};

#endif
