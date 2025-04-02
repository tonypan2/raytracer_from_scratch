// Author: Tony Pan (panjiabe@grinnell.edu)

// A 2D texture

#include "Texture.h"

using namespace std;

namespace Raytracer
{
  // Create a texture object by loading from a PPM file
  Texture::Texture(char *filename)
  {
    int i;
    unsigned char* raw_data;
    if (filename[0]==0)
    {
      is_empty = true;
      return;
    }
    is_empty = false;

    // Read the file and handle exceptions
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
      is_empty = true;
      cout << "Error: Cannot read file " << filename << endl;
      exit(EXIT_FAILURE);
    }

    // Discard "P6\n", record dimensions, and discard color-depth
    // (since it is assumed to be 255)
    getc(file); getc(file); getc(file);
    fscanf(file, "%d %d", &w, &h);
    fscanf(file, "%d\n", &i);

    // Dynamically allocate memory according to dimensions and 
    // handle exceptions
    raw_data = (unsigned char*)malloc(w*h*3);
    if (raw_data == NULL)
    {
      cout << "Error: Out of memory." << endl;
      free(raw_data);
      fclose(file);
      exit(EXIT_FAILURE);
    }
    fread(raw_data, w*h*3, 1, file);
    fclose(file);
    pixels = (double*)malloc(w*h*3*sizeof(double));
    for (i=0; i<w*h*3; i++)
    {
      pixels[i] = raw_data[i]/255.0;
    }
    free(raw_data);
  }

  // Return the texture color at the specified location
  // with or without bilinear filter.
  Color Texture::acquire_color(double x, double y, bool filter)
  {
    if (filter)
    {
      double a = floor(x)+1-x; double b = floor(y)+1-y;
      double c = x-floor(x); double d = y-floor(y);
      double w1 = a*b; double w2 = c*b; double w3 = a*d; double w4 = c*d;
      int cx = (int)x; int dx = (cx<w-1)?cx+1:cx;
      int cy = (int)y; int dy = (cy<h-1)?cy+1:cy;
      int offset1 = 3*(cy*w+cx); int offset2 = 3*(dy*w+cx);
      int offset3 = 3*(cy*w+dx); int offset4 = 3*(dy*w+dx);
      return Color(pixels[offset1]*w1+pixels[offset2]*w2+
                   pixels[offset3]*w3+pixels[offset4]*w4,
                   pixels[offset1+1]*w1+pixels[offset2+1]*w2+
                   pixels[offset3+1]*w3+pixels[offset4+1]*w4,
                   pixels[offset1+2]*w1+pixels[offset2+2]*w2+
                   pixels[offset3+2]*w3+pixels[offset4+2]*w4);
    }
    else
    {
      int cx = (int)x; if (cx>=w) cx = w-1; if (cx<0) cx=0;
      int cy = (int)y; if (cy>=h) cy = h-1; if (cy<0) cy=0;
      int offset = 3*(cy*w+cx);
      return Color(pixels[offset], pixels[offset+1], pixels[offset+2]);
    }
  }

  // Apply bump mapping. Algorithm from
  // http://www.gamedev.net/reference/articles/article1903.asp
  Vector Texture::apply_bump(double x, double y, Vector normal)
  {
    double h1 = acquire_color(x+1, y, true).x;
    double h2 = acquire_color(x-1, y, true).x;
    double h3 = acquire_color(x, y+1, true).x;
    double h4 = acquire_color(x, y-1, true).x;
    Vector u = Vector(1, 0, (h1-h2)*255);
    Vector v = Vector(0, 1, (h3-h4)*255);
    return (normal+(u.cross(v).normalize())).normalize();
  }

  // Free memory used by texture
  Texture::~Texture()
  {
    if (!is_empty) free(pixels);
  }
};
