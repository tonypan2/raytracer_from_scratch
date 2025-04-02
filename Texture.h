// Author: Tony Pan (panjiabe@grinnell.edu)

// A 2D texture

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H

#include <iostream>
#include "Vector.h"

namespace Raytracer
{
  class Texture
  {
  public:
    Texture(char* filename);
    Color acquire_color(double x, double y, bool filter);
    Vector apply_bump(double x, double y, Vector normal);
    ~Texture();
    double* pixels; // Texture data
    int w, h; // Dimensions of the texture
    bool is_empty; // Empty flag
  };
};

#endif
