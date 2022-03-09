#pragma once

#include "Point.h"

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;

//Below you find the basic elements that you need to build the generalised map.

struct Dart {
 // constructor of the dart
 int id_dart, id_vert, id_edge, id_face;
};

struct Vertex {
  // the coordinates of this vertex:

    int id_vert;
    int id_dart;
    Point point;

  // constructor without arguments
  Vertex() : point(Point()) 
  {}

  // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
  Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z))
  {}
};

struct Edge {
   // constructor of the edge
   int id_edge;
   int start;
   int end;
   int id_dart;
   int id_bar_edge;
};

struct Face {
   //std::vector<Vertex> faces;
  // constructor of face
    int a,b,c,d;
    int id_face;
    int id_bar_face;
};


struct Triangle {
    int a,b,c;
};


