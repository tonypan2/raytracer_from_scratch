// Author: Tony Pan (panjiabe@grinnell.edu)

// The scene contains objects, lights, and the rendering engine

#include "Scene.h"

using namespace std;

namespace Raytracer
{
  // Add an object to the scene
  void Scene::add_object(Object* object)
  {
    objects.push_back(object);
  }

  // Add a light source to the scene
  void Scene::add_light(Light* light)
  {
    lights.push_back(light);
  }

  // Render the scene
  float* Scene::render(float* scene)
  {
    double x, y, r_x, r_y, d_x, d_y;
    long offset = 0;
    int i, j, p, q;
    Color result_color(0, 0, 0);
    scene = (float*)calloc(pixel_w*pixel_h*3, sizeof(float));
    d_x = view_w/pixel_w/anti_alias;
    d_y = view_h/pixel_h/anti_alias;

    // Render each pixel
    for (j=min_h; j<=max_h; j++)
    {
      y = (double)j/pixel_h*view_h-view_h/2;    
      for (i=0; i<pixel_w; i++)
      {
        x = (double)i/pixel_w*view_w-view_w/2;

        // Supersample randomly around the center of each subpixel
        for (p=1; p<=anti_alias; p++)
        {
          for (q=1; q<=anti_alias; q++)
          {
            r_x = x+(p+(0.05*rand()/RAND_MAX-0.1)-0.5)*d_x;
            r_y = y+(q+(0.05*rand()/RAND_MAX-0.1)-0.5)*d_y;
            result_color = result_color+ray_trace(Ray(Vector(r_x, r_y, 0),
                                                  Vector(r_x, r_y, 0)-eye),
                                                  0, false);
          }
        }

        // Average the samples
        result_color = result_color/(anti_alias*anti_alias);

        // Record result
        scene[offset++] = (result_color.x>1)?1:(float)result_color.x;
        scene[offset++] = (result_color.y>1)?1:(float)result_color.y;
        scene[offset++] = (result_color.z>1)?1:(float)result_color.z;
        cout << "i=" << i << ", j=" << j << endl; // Show percentage
      }
    }
    return scene;
  }

  // Trace a ray
  Color Scene::ray_trace(Ray ray, int depth, bool in_object)
  {
    if (depth>max_depth)
    {
      return Color(0, 0, 0);
    }
    double t_min, temp;
    Object* target;
    Vector hit_point, adjusted_hit_point;
    t_min = -1;

    // Find the closest (positive) intersection
    for (iterator i=objects.begin(); i!=objects.end(); i++)
    {
      temp = (*i)->intersect(ray);
      if ((temp>0) && ((temp<t_min) || (t_min==-1)))
      {
        t_min = temp;
        target = *i;
      }
    }
    if (t_min==-1) // No intersections
    {
      return background;
    }
    else
    {
      //return Color(0, 0, 0); 
      // Uncomment the above line to show only the object signature image
      hit_point = ray.start+t_min*ray.direction;
      adjusted_hit_point = hit_point-0.0001*ray.direction;
      Color local_light(0, 0, 0);
      Vector N = target->normal(hit_point, true);
      if (!in_object)
      {
        // Start by calculating the ambient light composition
        local_light = target->k_ambient*ambient;
  
        // Calculate light contribution from the light sources
        if (target->k_reflect+target->k_refract != 1)
        {
          // Apply the illumination model for each light source
          for (l_iterator i=lights.begin(); i!=lights.end(); i++)
          {
            local_light = local_light+
                          apply_light(*i, hit_point, target,
                                      adjusted_hit_point, N);
          }
        }
      }

      // Calculate light contribution from reflected rays
      Color reflect_light(0, 0, 0);
      Vector R;
      Vector V = ray.direction.normalize();
      double d = V.dot(N);
      if ((target->k_reflect>0) && (!in_object))
      {
        R = V-(2*N)*d;
        reflect_light = ray_trace(Ray(adjusted_hit_point, R),
                                  depth+1, in_object);
      }

      // Calculate light contribution from refraction
      Color refract_light(0, 0, 0);
      double n, p;
      if (target->k_refract>0)
      {
        n = in_object?target->refract_index:1.0/target->refract_index;
        p = n*n*(1-d*d);
        if (p>0)
        {
          if (p>1)
          {
            p=1;
          }
          R = n*V-(n*d+sqrt(1-p))*N*(in_object?-1:1);
          refract_light = ray_trace(Ray(hit_point+0.0001*ray.direction, R),
                                    depth+1, !in_object);
        }
      }

      // Combine colors
      return  ((1.0-target->k_reflect-target->k_refract)*local_light+
              target->k_reflect*reflect_light+
              target->k_refract*refract_light)*target->texture_at(hit_point);
    }
  }

  // Apply the illumination model
  Color Scene::apply_light(Light *light, Vector point, Object *obj, 
                           Vector new_point, Vector N)
  {
    double t, u, theta, k;
    Vector position;
    Ray ray_to_light;
    Vector L, H;
    Vector V = (eye-point).normalize();
    int total_hit = 0;
    bool hit;
    double i_r, i_g, i_b;

    // Shoot multiple rays to random points on the spherical light source
    // Alogrithm of obtaining random points from
    // http://demonstrations.wolfram.com/RandomPointsOnASphere/
    for (int j=0; j<show_shadow_rays; j++)
    {
      u = 2.0*rand()/RAND_MAX-1;
      theta = 2*PI*rand()/RAND_MAX;
      k = sqrt(1-u*u);
      position = light->position+Vector(light->r*k*cos(theta), 
                                        light->r*k*sin(theta),
                                        light->r*u);
      ray_to_light = Ray(new_point, position-new_point);
      hit = true;
      for (iterator i=objects.begin(); i!=objects.end(); i++)
      {
        t = (*i)->intersect(ray_to_light);
        if ((0<=t) && (t<=1))
        {
          hit = false;
          break;
        }
      }
      if (hit) total_hit++;
    }

    if (total_hit>0)
    {
      L = (light->position-point).normalize();
      H = (L+V).normalize();
    
      // Calculate diffuse and specular compositions for R, G, and B
      i_r = diffuse_and_specular(light->color.x, obj->k_diffuse.x, 
                                 obj->k_specular.x, obj->specular_coeff,
                                 N.dot(L), N.dot(H));
      i_g = diffuse_and_specular(light->color.y, obj->k_diffuse.y, 
                                 obj->k_specular.y, obj->specular_coeff,
                                 N.dot(L), N.dot(H));
      i_b = diffuse_and_specular(light->color.z, obj->k_diffuse.z, 
                                 obj->k_specular.z, obj->specular_coeff,
                                 N.dot(L), N.dot(H));
      return Color(i_r, i_g, i_b)*((double)total_hit/show_shadow_rays);
    }
    else
    {
      return Color(0, 0, 0);
    }
  }

  // Calculate diffuse and specular compositions
  double Scene::diffuse_and_specular(double intensity, double kd, double ks,
                                     double ns, double NL, double NH)
  {
    return intensity*((NL<0?0:kd*NL)+
                      (NH<0?0:(ks*pow(NH, ns))));
  }

  // Save output image
  void Scene::save_picture(char* filename, float* scene)
  {
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
      cout << "Error: Cannot create file " << filename << endl;
      fclose(file);
      exit(EXIT_FAILURE);
    }

    // Construct the header of a PPM file only if processing the
    // first segment of the entire image
    if (min_h==0)
    {
      fprintf(file, "%c%c\n", 'P', '6');
      fprintf(file, "%d %d\n", pixel_w, pixel_h);
      fprintf(file, "255\n");
    }

    // Write data to file
    for (int i=0; i < 3*pixel_w*(max_h-min_h+1); i++)
    {
      putc((int)(scene[i]*255), file);
    }
    fclose(file);  
  }
};
