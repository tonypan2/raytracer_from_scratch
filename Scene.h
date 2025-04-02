// Author: Tony Pan (panjiabe@grinnell.edu)

// The scene contains objects, lights, and the rendering engine

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <time.h>
#include "Light.h"
#include "Vector.h"
#include "Ray.h"
#include "Object.h"
#include "Raytracer.h"

using namespace std;

namespace Raytracer
{
  class Scene
  {
    typedef vector<Object *>::iterator iterator;
    typedef vector<Light *>::iterator l_iterator;

  public:
    // Create the scene according to the parameters
    Scene(int width, int height, double view_width, double view_height,
          double eye_x, double eye_y, double eye_z,
          double bg_r, double bg_g, double bg_b, double ambient_r,
          double ambient_g, double ambient_b, int anti_alias, int max_depth,
          double show_shadow_rays, int min_h, int max_h)
    {
      pixel_w = width;
      pixel_h = height; // Dimensions of output image
      view_w = view_width;
      view_h = view_height;                             // Dimensions of view plane
      eye = Vector(eye_x, eye_y, eye_z);                // Eye position
      background = Color(bg_r, bg_g, bg_b);             // Background color
      ambient = Color(ambient_r, ambient_g, ambient_b); // Ambient light
      srand((unsigned)time(NULL));                      // Seed the random number generator
      this->anti_alias = anti_alias;                    // Degree of supersampling
      this->max_depth = max_depth;                      // Maximum depth of recursion
      this->show_shadow_rays = show_shadow_rays;        // Number of shadow rays
      this->min_h = min_h;
      this->max_h = max_h; // Part of the output image
                           // to process; the rest of
                           // the image will be black
    };

    void add_object(Object *obj);
    void add_light(Light *light);
    float *render(float *scene);
    void save_picture(char *filename, float *scene);

  private:
    Color background, ambient;
    vector<Object *> objects; // Objects in the scene
    vector<Light *> lights;   // Lights in the scene
    Vector eye;
    int anti_alias, pixel_w, pixel_h, max_depth, min_h, max_h;
    double view_w, view_h, show_shadow_rays;
    Color ray_trace(Ray ray, int depth, bool in_object);
    Color apply_light(Light *light, Vector point, Object *obj,
                      Vector new_point, Vector N);
    double diffuse_and_specular(double intensity, double kd, double ks,
                                double ns, double NL, double NH);
  };
};

#endif
