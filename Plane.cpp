// Author: Tony Pan (panjiabe@grinnell.edu)

// A plane

#include "Plane.h"

using namespace std;

namespace Raytracer
{
  // Create a plane according to the parameters
  Plane::Plane(double ka_r, double ka_g, double ka_b,
               double kd_r, double kd_g, double kd_b,
               double ks_r, double ks_g, double ks_b, double ns,
               double k_reflect, double k_refract, double refract_index,
               char* texture_file, char* bump_map,
               double px, double py, double pz, 
               double nx, double ny, double nz):
  Object(ka_r, ka_g, ka_b, kd_r, kd_g, kd_b, ks_r, ks_g, ks_b, ns, 
         k_reflect, k_refract, refract_index, texture_file, bump_map)
  {
    point = Vector(px, py, pz);
    norm = Vector(nx, ny, nz).normalize();

    // Build U-V coordinate on the plane
    u = Vector(norm.y, norm.z, -norm.x).normalize();
    v = u.cross(norm).normalize();
    obj_type = "Plane";
  }

  // Calculate intersection
  double Plane::intersect(Ray ray)
  {
    double ND = ray.direction.dot(norm);
    if (ND==0)
    {
      return -1;
    }
    else
    {
      return norm.dot(point-ray.start)/ND;
    }
  }

  // Calculate normal vector
  Vector Plane::normal(Vector position, bool apply_bump)
  {
    if ((!apply_bump) || (bump_map->is_empty))
    {
      return norm;
    }
    Vector texture_pos = map_to_texture(position);
    return bump_map->apply_bump(texture_pos.x*bump_map->w, 
                                texture_pos.y*bump_map->w,
                                norm);
  }

  // Map texture to the plane
  Color Plane::texture_at(Vector position)
  {
    if (texture->is_empty)
    {
      return Color(1, 1, 1);
    }
    Vector texture_pos = map_to_texture(position);
    return texture->acquire_color(texture_pos.x*texture->w, 
                                  texture_pos.y*texture->w, true);
  }

  // Convert plane coordinates to texture coordinates (scaled down w times)
  // i.e. A point on the plane mapping to (w, h) on the texture will return
  // (1, h/w)
  Vector Plane::map_to_texture(Vector position)
  {
    double x, y, int_part, k, ratio;
    x = (position-point).dot(u)/5.0;
    y = (position-point).dot(v)/5.0;
    if (bump_map->is_empty)
    {
      ratio = (double)texture->h/texture->w;
    }
    else
    {
      ratio = (double)bump_map->h/bump_map->w;
    }
    x = modf(x, &int_part);
    if (x<0)
    {
      x++;
    }
    k = floor(abs(y)/ratio)*ratio;
    if (y>0)
    {
      y -= k;
    }
    else
    {
      y = ratio+y+k;
    }
    return Vector(x, y, 0);
  }
};
