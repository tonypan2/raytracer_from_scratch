#------------------------------------
# File:		Makefile
# Author:	M. Coahran
# Purpose:	3D OpenGL example, with raytrace
# Date: 	12/27/2008
#------------------------------------

CC=g++
#  -c (don't link, just create object file)
#  -g (add debugging information)
#  -O (optimize)

FLAGS=-c -gstabs -I/usr/include

LINKTO=-o raytrace

# This specifies where the libraries (listed below) are located. To specify
# more than one directory, simply add another '-L/...' to the line.
LIBDIRS = -L/usr/X11R6/lib

# This specifies what libraries to link to. The actual library filenames
# have the form 'libGLU.so' or 'libGLU.a', and they are located as above.
LIBS = -lGLU -lGL -lglut -lXmu -lXext -lX11 -lXi -lm

#------------------------------------
OBJECTS = Main.o Plane.o Scene.o Sphere.o Texture.o

raytrace: $(OBJECTS)
	$(CC) $(LINKTO) $(OBJECTS) $(LIBDIRS) $(LIBS)

Main.o: Main.cpp Light.h Scene.h Sphere.h Plane.h
	$(CC) $(FLAGS) Main.cpp

Plane.o: Plane.cpp Plane.h
	$(CC) $(FLAGS) Plane.cpp

Scene.o: Scene.cpp Scene.h
	$(CC) $(FLAGS) Scene.cpp

Sphere.o: Sphere.cpp Sphere.h
	$(CC) $(FLAGS) Sphere.cpp
	
Texture.o: Texture.cpp Texture.h
	$(CC) $(FLAGS) Texture.cpp
#------------------------------------
clean:
	rm -f *.o *~ core