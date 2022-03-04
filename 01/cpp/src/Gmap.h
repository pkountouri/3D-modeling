#pragma once

#include "Point.h"
#include <algorithm>

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;


/*
Below you find the basic elements that you need to build the generalised map.
The main thing you need to fill out are the links between the elements:
  * the involutions and cells on the Dart
  * the darts on the cells

One way to do this is by using pointers. eg. define a member on the dart struct like

  Struct Dart {
    // involutions:
    Dart* a0 = nullptr;
    // ...

    // cells:
    // ...
  
  };

Then you could create and link Darts like:
  
  Dart* dart_a = new Dart();
  Dart* dart_b = new Dart();

  dart_a->a0 = dart_b;
*/


// constructing the dart with the input arguments id_dart, id_vert, etc.
struct Dart {
    int id_dart;
    int id_vert;
    int id_edge;
    int id_face;
    int a0;
    int a1;
    int a2;
    int a3;

    Dart (const int &id_dart, const int &id_vert, const int &id_edge, const int &id_face) {
        this-> id_dart = id_dart;
        this-> id_vert = id_vert;
        this-> id_edge = id_edge;
        this-> id_face = id_face;
        this-> a0 = a0;
        this-> a1 = a1;
        this-> a2 = a2;
        this-> a3 = a3;
    }

    void operator = (const Dart& next) {
        id_dart=next.id_dart;
        id_vert=next.id_vert;
        id_edge=next.id_edge;
        id_face = next.id_face;
        a0 = next.a0;
        a1 = next.a1;
        a2 = next.a2;
        a3 = next.a3;
    }

};


// constructing the vertex with input arguments id_vert, id_dart
struct Vertex {
    int id_vert;
    int id_dart;
    float x;
    float y;
    float z;
    Point point;

    Vertex (const int &id_vert, const int &id_dart, const float &x, const float &y, const float &z) {
        this-> id_vert = id_vert;
        this-> id_dart = id_dart;
        this-> x = x;
        this-> y = y;
        this-> z = z;
    }

};


// constructing the edge with input arguments id_edge, id_dart, start, end
struct Edge {
    int id_edge;
    int id_dart;
    int start;
    int end;

    Edge (const int &id_edge, const int &id_dart) {
        this-> id_edge = id_edge;
        this-> id_dart = id_dart;
    }

};


// constructing the face with input arguments ______ ?
struct Face {
//    int a,b,c,d;
    int id_face;
    int id_dart;
    std::vector<int> vertices;
//    Point v1,v2,v3,v4;
//  std::vector<Vertex> faces;
//  int id_face;


    Face (const int &id_face, const int &id_dart) {
        this-> id_face = id_face;
        this-> id_dart = id_dart;
    }

};


// constructing the volume with input arguments id_vol, id_dart (dart incident to the volume)
struct Volume {
    int id_vol;
    int id_dart;

    Volume(const int &id_vol, const int &id_dart) {
        this-> id_vol = id_vol;
        this-> id_dart = id_dart;
    }

};