// Author: Tony Pan (panjiabe@grinnell.edu)

// A demonstraction of the raytracer

#include <iostream>
#include <stdlib.h>
#include <GLUT/glut.h>

#include "Plane.h"
#include "Sphere.h"
#include "Scene.h"

using namespace std;
using namespace Raytracer;

// Image buffer
float *pic;

// Constants
const int winW = 800; // Window dimensions
const int winH = 600;
const double viewW = 4.0; // View plane dimensions
const double viewH = 3.0;
const double eye_x = 0; // Camera position
const double eye_y = 0;
const double eye_z = 20;
const double bg_r = 1; // Background color
const double bg_g = 1;
const double bg_b = 1;
const double ambient_r = 0; // Global ambient light
const double ambient_g = 0;
const double ambient_b = 0;

// Prototypes
void init_display();
void display();
void cleanup();

// Display the image buffer
void display(void)
{
  int error;

  // Clear the frame buffer to begin a new frame
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw graph
  glDrawPixels(winW, winH, GL_RGB, GL_FLOAT, pic);

  // Frame buffer is complete, so move it to "front" for screen display
  glutSwapBuffers();

  // Detect rendering errors (note that the call to glGetError() clears
  // the error condition too, so call it once and save the error code).
  error = glGetError();
  if (error != GL_NO_ERROR)
  {
    printf("OpenGL Error: %s\n", gluErrorString(error));
  }
}

// Initialize display
void init_display()
{
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(winW, winH);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Raytracing");

  // Register callback functions for glut
  glutDisplayFunc(display);
  atexit(cleanup);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);

  if (argc != 3)
  {
    cout << "usage   : " << argv[0] << " MIN_Y MAX_Y" << endl;
    cout << "purpose : Render the part of image from MIN_Y to MAX_Y" << endl;
    cout << "          0 <= MIN_Y <= MAX_Y < " << winH << endl;
    cout << "          This will generate a \"result_MIN_Y_MAX_Y.raw\" file."
         << endl;
    cout << "note    : When all parts of image are finished, use cat to"
         << endl;
    cout << "          combine .raw files in the order of increasing Y."
         << endl;
    cout << "          e.g. cat result_0_100.raw result_101_200.raw > all.ppm"
         << endl;
    exit(0);
  }

  // Create scene
  Scene scene(winW, winH, viewW, viewH, eye_x, eye_y, eye_z,
              bg_r, bg_g, bg_b, ambient_r, ambient_g, ambient_b, 4, 6, 1000,
              atoi(argv[1]), atoi(argv[2])); // 4, 6, 1000

  // Create spheres
  // Parameters from:
  // www.opengl.org/resources/code/samples/sig99/advanced99/notes/node153.html
  // Textures from:
  // planetpixelemporium.com and cgtextures.com
  Sphere ball1(0.25, 0.20725, 0.20725, 0.829, 0.829, 0.922,
               0.296648, 0.296648, 0.922, 100, 0.1, 0, 0, "earth.ppm",
               "earth_bump.ppm",
               -0.7, -0.5, -10, 1);

  Sphere ball2(0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828,
               0.256777, 0.137622, 0.086014, 100, 0.2, 0.8, 1.755, "", "",
               -0.4, -1, -3, 0.5);

  Sphere ball3(0.25, 0.20725, 0.20725, 0.829, 0.829, 0.922,
               0.296648, 0.296648, 0.922, 100, 0, 0.5, 0, "venus.ppm",
               "venus_bump.ppm",
               0.3, -1.1, -1, 0.4);

  Sphere ball4(0.25, 0.20725, 0.20725, 0.829, 0.829, 0.922,
               0.296648, 0.296648, 0.922, 100, 0.9, 0.1, 1.3, "", "",
               0.5, -0.8, -6, 0.7);

  // Create planes
  Plane plane1(0.25, 0.20725, 0.20725, 0.829, 0.829, 0.922,
               0.296648, 0.296648, 0.922, 20, 0.6, 0, 0, "ground.ppm", "",
               0, -1.5, 0, 0, 1, 0);

  Plane plane2(0.25, 0.20725, 0.20725, 0.829, 0.829, 0.922,
               0.296648, 0.296648, 0.922, 20, 0, 0, 0, "", "",
               0, 10, 0, 0, -1, 0);

  // Creat lights
  Light light1(0, 1, 1, 1, 1, 1, 0.3);

  Light light2(1, 1, -2, 1, 1, 1, 0.5);

  // Add objects and lights to the scene
  scene.add_object(&ball1);
  scene.add_object(&ball2);
  scene.add_object(&ball3);
  scene.add_object(&ball4);
  scene.add_object(&plane1);
  scene.add_object(&plane2);
  scene.add_light(&light1);
  scene.add_light(&light2);

  // Render scene
  pic = scene.render(pic);
  char filename[255];
  strcpy(filename, "result_");
  strcat(filename, argv[1]);
  strcat(filename, "_");
  strcat(filename, argv[2]);
  strcat(filename, ".raw");
  scene.save_picture(filename, pic);
  init_display();
  glutMainLoop(); // Infinite event-driven loop (managed by glut)
  return 0;
}

// Release image buffer
void cleanup()
{
  free(pic);
}
