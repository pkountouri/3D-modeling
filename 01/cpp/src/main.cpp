// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Gmap.h"
#include <unordered_map>

//Function to sort the edges
bool compareByID(const Edge &a, const Edge &b)
{
    return a.id_edge <= b.id_edge;
}

int main(int argc, const char * argv[]) {
    std::string file_in = "torus.obj";


    // ## Read OBJ file ##

//Read vertices and faces from OBJ file

    // Creation of vectors
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;
    std::vector<Dart> darts;
    std::vector<std::pair<int, int>> ao;
    std::vector<std::pair<int, int>> a1;
    std::vector<std::pair<int, int>> a2;

    //Output faces in CSV file (header)
    std::ofstream myFile4;
    myFile4.open("torus_faces.csv");
    myFile4 << "id " << ";" << " dart " << std::endl;


    //Define and initialize ID value for vertices,edges, darts and faces
    int id_vert = 1;
    int id_edge = 1;
    int id_dart = 1;
    int id_face = 1;

    std::ifstream stream_in;
    stream_in.open(file_in);
    if (stream_in.is_open()) {
        std::string line;
        while (getline(stream_in, line)) {
            std::istringstream iss(line);
            std::string word;
            iss >> word;
            if (word == "v") {
                std::vector<float> coordinates;
                while (iss >> word) coordinates.push_back(std::stof(word));
                if (coordinates.size() == 3) {
                    Vertex vertex = Vertex(coordinates[0], coordinates[1], coordinates[2]);
                    vertex.id_vert = id_vert;
                    id_vert++;
                    vertices.emplace_back(vertex);
                }
                else vertices.emplace_back();
            }
            if (word == "f") {
                std::vector<int> index;
                while (iss >> word)
                    index.push_back(std::stoi(word));
                if (index.size() == 4) {
                    faces.emplace_back(Face{index[0], index[1], index[2], index[3]});


                    // ## Construct generalised map using the structures from Gmap.h ##

                    //Create the first edge of a face/////////////////////////////////
                    Edge edge_1 = Edge{id_edge, index[0], index[1]};
                    int k = 0;
                    //Give same ID in same edges but opposite start-end point
                    for (auto &edge: edges) {
                        if ((edge.start == edge_1.end) && (edge_1.start == edge.end)) {
                            edge_1.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the first edge:
                    Dart dart_1 = Dart{id_dart, index[0], edge_1.id_edge, id_face};
                    darts.push_back(dart_1);

                    //Update the id dart for the first vertex of the corresponding face
                    (vertices[index[0]-1]).id_dart = id_dart;  // -1: OBJ uses 1-based indexes while C++ 0-based indexes

                    //Update the id dart for the first edge of the corresponding face
                    edge_1.id_dart = id_dart;

                    id_dart = id_dart + 1; //increase the id darts

                    Dart dart_2 = Dart{id_dart, index[1], edge_1.id_edge, id_face};
                    darts.push_back(dart_2);
                    id_dart = id_dart + 1;

                    //Increase the ID only if the duplicate edge is not found
                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_1);


                    //Create the second edge of a face//////////////////////////////
                    Edge edge_2 = Edge{id_edge, index[1], index[2]};
                    k = 0;
                    //Give same ID in same edges but opposite start-end point
                    for (auto &edge: edges) {
                        if ((edge.start == edge_2.end) && (edge_2.start == edge.end)) {
                            edge_2.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the second edge:
                    Dart dart_3 = Dart{id_dart, index[1], edge_2.id_edge, id_face};
                    darts.push_back(dart_3);

                    //Update the id dart for the second vertex of the corresponding face
                    vertices[index[1]-1].id_dart = id_dart;

                    //Update the id dart for the second edge of the corresponding face
                    edge_2.id_dart = id_dart;

                    id_dart = id_dart + 1; //increase the id darts

                    Dart dart_4 = Dart{id_dart, index[2], edge_2.id_edge, id_face};
                    darts.push_back(dart_4);
                    id_dart = id_dart + 1;

                    //Increase the ID only if the duplicate edge is not found
                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_2);


                    //Create the third edge of a face/////////////////////////////////
                    Edge edge_3 = Edge{id_edge, index[2], index[3]};
                    k = 0;
                    //Give same ID in same edges but opposite start-end point
                    for (auto &edge: edges) {
                        if ((edge.start == edge_3.end) && (edge_3.start == edge.end)) {
                            edge_3.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the third edge:
                    Dart dart_5 = Dart{id_dart, index[2], edge_3.id_edge, id_face};
                    darts.push_back(dart_5);

                    //Update the id dart for the third vertex of the corresponding face
                    vertices[index[2]-1].id_dart = id_dart;

                    //Update the id dart for the third edge of the corresponding face
                    edge_3.id_dart = id_dart;

                    id_dart = id_dart + 1; //increase the id darts

                    Dart dart_6 = Dart{id_dart, index[3], edge_3.id_edge, id_face};
                    darts.push_back(dart_6);
                    id_dart = id_dart + 1;

                    //Increase the ID only if the duplicate edge is not found
                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_3);


                    //Create the forth edge of a face/////////////////////////////
                    Edge edge_4 = Edge{id_edge, index[3], index[0]};
                    k = 0;
                    //Give same ID in same edges but opposite start-end point
                    for (auto &edge: edges) {
                        if ((edge.start == edge_4.end) && (edge_4.start == edge.end)) {
                            edge_4.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the forth edge:
                    Dart dart_7 = Dart{id_dart, index[3], edge_4.id_edge, id_face};
                    darts.push_back(dart_7);

                    //Update the id dart for the forth vertex of the corresponding face
                    vertices[index[3]-1].id_dart = id_dart;

                    //Update the id dart for the forth edge of the corresponding face
                    edge_4.id_dart = id_dart;

                    id_dart = id_dart + 1;

                    Dart dart_8 = Dart{id_dart, index[0], edge_4.id_edge, id_face};
                    darts.push_back(dart_8);

                    //Output face CSV file
                    myFile4  << id_face  << " ; " << id_dart << std::endl;

                    id_face = id_face + 1; // increase the id for the face
                    id_dart = id_dart + 1; // increase the id darts for the next face

                    //Increase the ID only if the duplicate edge is not found
                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_4);


                    //Ao involution in ascending order of the darts' id:////////////
                    //Ao involution (1st edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_1.id_dart, dart_2.id_dart);
                    ao.emplace_back(dart_2.id_dart, dart_1.id_dart);

                    //Ao involution (2nd edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_3.id_dart, dart_4.id_dart);
                    ao.emplace_back(dart_4.id_dart, dart_3.id_dart);

                    //Ao involution (3rd edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_5.id_dart, dart_6.id_dart);
                    ao.emplace_back(dart_6.id_dart, dart_5.id_dart);

                    //Ao involution (4rth edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_7.id_dart, dart_8.id_dart);
                    ao.emplace_back(dart_8.id_dart, dart_7.id_dart);


                    //A1 involution in ascending order of the darts' id:////////////
                    // A1 involution (4 & 1 edge) -> same vertex & face, change edge
                    a1.emplace_back(dart_1.id_dart, dart_8.id_dart);

                    //A1 involution(1 & 2 edge) -> same vertex & face, change edge
                    a1.emplace_back(dart_2.id_dart, dart_3.id_dart);
                    a1.emplace_back(dart_3.id_dart, dart_2.id_dart);

                    //A1 involution(2 & 3 edge) -> same vertex & face, change edge
                    a1.emplace_back(dart_4.id_dart, dart_5.id_dart);
                    a1.emplace_back(dart_5.id_dart, dart_4.id_dart);

                    //A1 involution(3 & 4 edge) -> same vertex & face, change edge
                    a1.emplace_back(dart_6.id_dart, dart_7.id_dart);
                    a1.emplace_back(dart_7.id_dart, dart_6.id_dart);

                    //A1 involution(4 & 1 edge) -> same vertex & face, change edge
                    a1.emplace_back(dart_8.id_dart, dart_1.id_dart);
                }
            }
        }
    }

    //Sort and erase duplicates edges inside vector edges
    std::sort(edges.begin(), edges.end(), compareByID);
    for (int i=1; i<edges.size(); i++){
        edges.erase(edges.begin()+i);
    }

    //A2 involution -> same vertex & edge, change face
    for (auto &dart: darts) {
        for (auto &dart2: darts) {
            if ((dart.id_vert == dart2.id_vert) && (dart.id_edge == dart2.id_edge) && (dart.id_face != dart2.id_face)) {
                a2.emplace_back(dart.id_dart, dart2.id_dart);
            }
        }
    }


    // ## Output generalised map to CSV ##

    //Output darts in CSV file:
    std::ofstream myFile1;
    myFile1.open("torus_darts.csv");
    //Header construction
    myFile1 << "Dart ID" << ";" << " ao " << ";" << " a1 " << ";" << " a2 " << ";" << " a3 " << ";" << " v "
           << ";" << " e " << ";" << " f " << std::endl;

    for (int i = 0; i < darts.size(); i++) {
        int a3 = 0;
        myFile1 << "d" << darts[i].id_dart << ";" << ao[i].second << ";" << a1[i].second << ";" << a2[i].second
        << ";" << a3 << ";" << darts[i].id_vert << ";" << darts[i].id_edge << ";" << darts[i].id_face
        << std::endl;
    }

    //Output vertices in CSV file
    std::ofstream myFile2;
    myFile2.open("torus_vertices.csv");
    //Header construction
    myFile2 << "id" << ";" << " dart " << ";" << " x " << ";" << " y " << ";" << " z " << std::endl;

    for (Vertex vert: vertices) {
        myFile2  << vert.id_vert  << " ; " << vert.id_dart << " ; " <<  vert.point.x  << " ; " << vert.point.y  << " ; " << vert.point.z << std::endl;
    }

    //Output edges in CSV file
    std::ofstream myFile3;
    myFile3.open("torus_edges.csv");
    //Header construction
    myFile3 << "id" << ";" << " dart " << std::endl;

    for (Edge edge : edges){
        myFile3 << edge.id_edge << " ; " << edge.id_dart << std::endl;
    }

    //Output volume in CSV file
    std::ofstream myFile5;
    myFile5.open("torus_volume.csv");
    //Header construction
    myFile5 << "id" << ";" << " dart " << std::endl;
    myFile5 << 1 << " ; " << darts[0].id_dart << std::endl;



  // ## Create triangles from the darts ##




  // ## Write triangles to obj ##





  // ## Useful prints ##

// // Print edges
//    for (Edge edge : edges){
//        std::cout << edge.id_edge << " " << edge.start << " " << edge.end << " " << edge.id_dart << std::endl;
//    }

//  // Print vertices
//    for (Vertex vert: vertices) {
//        std::cout << "vert_id : " << vert.id_vert << " id_dart : " << vert.id_dart << " point: " << vert.point << std::endl;
//    }

//  // Print faces
//    for (Face face:faces) {
//        std::cout<< face.a << face.b << face.c << face.d << std::endl;
//    }

//    for (int i =0; i < edges.size(); i++)  {
//        std::cout << Edge {edges.start[i]}  <<std::endl;
//    }

//    //Print the first and second dart_id of the ao involution
//    for (int i=0; i<ao.size();i++){
//        std::cout<< a2[i].first<<"  "<<a2[i].second<<std::endl;
//}

    //Print the darts.csv
//    for (int i=0; i<darts.size();i++){
//        int a3 = 0;
//        std::cout<< darts[i].id_dart << " " << ao[i].second << " " << a1[i].second << " " << a2[i].second << " " << a3 << " " << darts[i].id_vert << " " << darts[i].id_edge << " " << darts[i].id_face <<std::endl;
//    }

//    //Print the vertices
//    for (int i=1; i<vertices.size()+1;i++){
//         std::cout << i << vertices[i-1].point.x << std::endl;
//    }

  return 0;
}