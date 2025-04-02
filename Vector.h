// Author: Tony Pan (panjiabe@grinnell.edu)

// A ordered group of 3 numbers.
// Can be used to represent a vector or a position.
// For clarity, use the alias type "Color" to represent colors.

#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <cmath>

namespace Raytracer
{
  class Vector
  {
  public:
    Vector()
    {
    }

    Vector(double x, double y, double z)
    {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    // Negation
    Vector operator-()
    {
      return Vector(this->x, this->y, this->z);
    }

    // Addition
    Vector operator+(const Vector &v)
    {
      return Vector(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    // Substraction
    Vector operator-(const Vector &v)
    {
      return Vector(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    // Normalize
    Vector normalize()
    {
      return (*this) / this->magnitude();
    }

    // Dot product
    double dot(const Vector &v)
    {
      return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    // Cross product
    Vector cross(const Vector &v)
    {
      return Vector(this->y * v.z - this->z * v.y,
                    this->z * v.x - this->x * v.z,
                    this->x * v.y - this->y * v.x);
    }

    // Scalar multiplication
    friend Vector operator*(double d, const Vector &v)
    {
      return Vector(v.x * d, v.y * d, v.z * d);
    }

    // Scalar multiplication
    friend Vector operator*(const Vector &v, double d)
    {
      return Vector(v.x * d, v.y * d, v.z * d);
    }

    // Scalar division
    friend Vector operator/(const Vector &v, double d)
    {
      return Vector(v.x / d, v.y / d, v.z / d);
    }

    // Component-wise multiplication
    friend Vector operator*(const Vector &v1, const Vector &v2)
    {
      return Vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    }

    // Magnitude
    double magnitude()
    {
      return sqrt(this->dot(*this));
    }

    double x, y, z;
  };

  typedef Vector Color;
};

#endif
