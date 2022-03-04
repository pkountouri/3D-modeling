#pragma once

#include "Point.h"
#include <algorithm>

struct vertex_Coords;
struct face_Points;

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;

// vertex_Coords stores the three coordinates of each vertex from the input OBJ file, and can then return them.
// the ostream object 'os' is where the characters are inserted.
struct vertex_Coords {
    Point point;
    double x{};
    double y{};
    double z{};

    vertex_Coords(): point(Point()) {
    }

    vertex_Coords (const double &x, const double &y, const double &z): point(Point(x, y, z)) {
        this-> x = x;
        this-> y = y;
        this-> z = z;
    }

    friend std::ostream& operator<<(std::ostream &os, const vertex_Coords& one) {
        os << "Vertex = " << one.point;
        return os;
    }

};

// face_Points stores the four points that make up each face, using vertex_Coords' vectors (the temp_vertices),
// and can then return them (ostream writes sequences of characters).
struct face_Points {
    std::vector<vertex_Coords> temp_vertices;

    face_Points (const std::vector<vertex_Coords>& temp_vertices) {
        this-> temp_vertices = temp_vertices;
    }

    friend std::ostream& operator<< (std::ostream& os, const face_Points&a) {
        for (auto& e : a.temp_vertices) os << e.point;
        return os;
    }

    template<typename _InputIterator>
    _InputIterator begin() {
        return nullptr;
    }
};

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

    Dart (const int &id_dart, const int &id_vert, const int &id_edge, const int &id_face, const int &a0, const int &a1, const int &a2, const int &a3) {
        this -> id_dart = id_dart;
        this -> id_vert = id_vert;
        this -> id_edge = id_edge;
        this -> id_face = id_face;
        this -> a0 = a0;
        this -> a1 = a1;
        this -> a2 = a2;
        this -> a3 = a3;
    }

    void operator = (const Dart &next) {
        id_dart=next.id_dart;
        id_vert=next.id_vert;
        id_edge=next.id_edge;
        id_face = next.id_face;
        a0 = next.a0;
        a1 = next.a1;
        a2 = next.a2;
        a3 = next.a3;
    }

    friend std::ostream& operator<<(std::ostream &os, const Dart& one) {
        os << "id = " << one.id_dart << "vertex = " << one.id_vert << "edge = " << one.id_edge << "face = " << one.id_face << "a0 = " << one.a0 << "a1 = " << one.a1 << "a2 = " << one.a2 << "a3 = " << one.a3;
        return os;
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

    Vertex (const int &id_vert, const int&id_dart, const float&x, const float &y, const float &z) {
        this -> id_vert = id_vert;
        this -> id_dart = id_dart;
        this -> x = x;
        this -> y = y;
        this -> z = z;
    }

    friend std::ostream & operator<<(std::ostream &os, const Vertex&one) {
        os << "id = " << one.id_vert << "dart = " << one.id_dart << "x = " << one.x << "y = " << one.y << "z = " << one.z;
        return os;
    }

};


// constructing the edge with input arguments id_edge, id_dart, start, end
struct Edge {
    int id_edge;
    int id_dart;
    int start;
    int end;

    Edge (const int &id_edge, const int&id_dart) {
        this -> id_edge = id_edge;
        this -> id_dart = id_dart;
    }

    friend std::ostream & operator<<(std::ostream &os, const Edge&one) {
        os << "id = " << one.id_edge << "dart = " << one.id_dart;
        return os;
    }

};


// constructing the face with input arguments ______ ?
struct Face {
    int a,b,c,d; // a bit confused about what a, b, c, d are? Is a the dart?
    int id_face;
    Point v1,v2,v3,v4;
//  std::vector<Vertex> faces;
//  int id_face;


    Face (const int &id_face, const int&a) {
        this -> id_face = id_face;
        this -> a = a;
    }

    friend std::ostream & operator<<(std::ostream &os, const Face&one) {
        os << "id = " << one.id_face << "dart = " << one.a;
        return os;
    }

};


// constructing the volume with input arguments id_vol, id_dart (dart incident to the volume)
struct Volume {
    int id_vol;
    int id_dart;

    Volume(const int &id_vol, const int&id_dart) {
        this -> id_vol = id_vol;
        this -> id_dart = id_dart;
    }

    friend std::ostream & operator<<(std::ostream &os, const Volume&one) {
        os << "id = " << one.id_vol << "dart = " << one.id_dart;
        return os;
    }

};