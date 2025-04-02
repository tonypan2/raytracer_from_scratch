// Author: Tony Pan (panjiabe@grinnell.edu)

// A sphere

#include "Sphere.h"

using namespace std;

namespace Raytracer
{
  // Construct a sphere according to the parameters
  Sphere::Sphere(double ka_r, double ka_g, double ka_b,
                 double kd_r, double kd_g, double kd_b,
                 double ks_r, double ks_g, double ks_b, double ns,
                 double k_reflect, double k_refract, double refract_index,
                 char *texture_file, char *bump_map,
                 double x, double y, double z, double r) : Object(ka_r, ka_g, ka_b, kd_r, kd_g, kd_b, ks_r, ks_g, ks_b, ns, k_reflect,
                                                                  k_refract, refract_index, texture_file, bump_map)
  {
    top = Vector(0, 1, 0);
    side = Vector(1, 0, 0);
    this->center = Vector(x, y, z);
    this->r = r;
    obj_type = "Sphere";
  }

  // Calculate intersection with a ray
  double Sphere::intersect(Ray ray)
  {
    double t1, t2, smaller;
    double a = ray.direction.dot(ray.direction);
    double b = 2 * (ray.direction.dot(ray.start - center));
    double c = center.dot(center) + ray.start.dot(ray.start) -
               2 * center.dot(ray.start) - r * r;
    double discriminant = b * b - 4 * a * c;

    // Find out the intersection, if any
    if (discriminant >= 0)
    {
      t1 = -(b + sqrt(discriminant)) / (2 * a);
      t2 = -(b - sqrt(discriminant)) / (2 * a);
      smaller = t1 < t2 ? t1 : t2;
      if (smaller >= 0)
      {
        return smaller;
      }
      else if (t1 < 0)
      {
        return t2 >= 0 ? t2 : -1;
      }
      else if (t2 < 0)
      {
        return t1 >= 0 ? t1 : -1;
      }
    }
    else
    {
      return -1;
    }
  }

  // Calculate normal vector at a given position with or
  // without bump mapping.
  Vector Sphere::normal(Vector position, bool apply_bump)
  {
    Vector normal = (position - center).normalize();
    if ((!apply_bump) || (bump_map->is_empty))
    {
      return normal;
    }
    Vector texture_pos = map_to_texture(position);
    return bump_map->apply_bump(texture_pos.x * bump_map->w,
                                texture_pos.y * bump_map->h,
                                normal);
  }

  // Map texture to the plane
  Color Sphere::texture_at(Vector position)
  {
    if (texture->is_empty)
      return Color(1, 1, 1);
    Vector texture_pos = map_to_texture(position);
    return texture->acquire_color(texture_pos.x * texture->w,
                                  texture_pos.y * texture->h, true);
  }

  // Convert plane coordinates to texture coordinates
  // scaled down to a unit squre. A point on the plane mapping
  // to (w, h) on the texture will return (1, 1)
  Vector Sphere::map_to_texture(Vector position)
  {
    Vector p = normal(position, false);
    double v = acos(top.dot(p)) / PI;
    p = p * Vector(1, 0, 1); // Projection of p on the equator plane
    double u;
    if (p.magnitude() == 0)
    {
      u = 0;
    }
    else
    {
      p = p.normalize();
      u = (side.cross(p).dot(top) > 0 ? acos(side.dot(p)) : 2 * PI - acos(side.dot(p))) / (2 * PI);
    }
    return Vector(u, v, 0);
  }
};
