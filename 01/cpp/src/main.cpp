// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include <unordered_map>

#include "Gmap.h"

int main(int argc, const char * argv[]) {
    std::string file_in = "cube.obj";
    std::string file_out_obj = "/home/ravi/git/geo1004.2022/hw/01/data/torus_triangulated.obj";
    std::string file_out_csv_d = "/home/ravi/git/geo1004.2022/hw/01/data/torus_darts.csv";
    std::string file_out_csv_0 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_vertices.csv";
    std::string file_out_csv_1 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_edges.csv";
    std::string file_out_csv_2 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_faces.csv";
    std::string file_out_csv_3 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_volume.csv";

    // ## Read OBJ file ##

    /* Example how to read vertices from an obj file:

      std::ifstream stream_in;
      stream_in.open(file_in);
      std::vector<Vertex> vertices;
      if (stream_in.is_open()) {
        std::string line;
        while (getline(stream_in, line)) {
          std::istringstream iss(line);
          std::string word;
          iss >> word;
          if (word == "v") {
            std::vector<float> coordinates;
            while (iss >> word) coordinates.push_back(std::stof(word));
            if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
            else vertices.push_back(Vertex());
          }
        }
      }
    */



//Read vertices and faces from OBJ file

    std::ifstream stream_in;
    stream_in.open(file_in);
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;
    std::vector<Dart> darts;
    std::vector<std::pair<int, int>> ao;
    std::vector<std::pair<int, int>> a1;
    std::vector<std::pair<int, int>> a2;

    int id_vert = 1;
    int id_edge = 1;
    int id_dart = 1;
    int id_face = 1;
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
                else vertices.emplace_back();   // question -> what this line does?
            }
            if (word == "f") {
                std::vector<int> index;
                while (iss >> word)
                    index.push_back(std::stoi(word)); // -1: OBJ thus uses 1-based indexes while C++ 0-based indexes
                if (index.size() == 4) {
                    faces.emplace_back(Face{index[0], index[1], index[2], index[3]});
                    //else faces.push_back(Vertex());


                    //Create the first edge of a face/////////////////////////////////
                    Edge edge_1 = Edge{id_edge, index[0], index[1]};
                    int k = 0;
                    for (auto &edge: edges) {
                        if ((edge.start == edge_1.end) && (edge_1.start == edge.end)) {
                            edge_1.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the first edge:
                    Dart dart_1 = Dart{id_dart, index[0], edge_1.id_edge, id_face};
                    darts.push_back(dart_1);

                    (vertices[index[0]-1]).id_dart = id_dart;
                    id_dart = id_dart + 1;




                    Dart dart_2 = Dart{id_dart, index[1], edge_1.id_edge, id_face};
                    darts.push_back(dart_2);
                    id_dart = id_dart + 1;

                    //Ao involution (1st edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_1.id_dart, dart_2.id_dart);
                    ao.emplace_back(dart_2.id_dart, dart_1.id_dart);

                    //std::cout << "dartid_1:" << dart_1.id_dart <<  std::endl;
                    // std::cout << "dartid_2:" << dart_2.id_dart <<  std::endl;

                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_1);





                    //Create the second edge of a face//////////////////////////////
                    Edge edge_2 = Edge{id_edge, index[1], index[2]};
                    k = 0;
                    for (auto &edge: edges) {
                        if ((edge.start == edge_2.end) && (edge_2.start == edge.end)) {
                            edge_2.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the second edge:
                    Dart dart_3 = Dart{id_dart, index[1], edge_2.id_edge, id_face};
                    darts.push_back(dart_3);
                    vertices[index[1]-1].id_dart = id_dart;

                    id_dart = id_dart + 1;

                    Dart dart_4 = Dart{id_dart, index[2], edge_2.id_edge, id_face};
                    darts.push_back(dart_4);
                    id_dart = id_dart + 1;



                    //Ao involution (2nd edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_3.id_dart, dart_4.id_dart);
                    ao.emplace_back(dart_4.id_dart, dart_3.id_dart);

                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_2);





                    //Create the third edge of a face/////////////////////////////////
                    Edge edge_3 = Edge{id_edge, index[2], index[3]};
                    k = 0;
                    for (auto &edge: edges) {
                        if ((edge.start == edge_3.end) && (edge_3.start == edge.end)) {
                            edge_3.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the third edge:
                    Dart dart_5 = Dart{id_dart, index[2], edge_3.id_edge, id_face};
                    darts.push_back(dart_5);
                    vertices[index[2]-1].id_dart = id_dart;

                    id_dart = id_dart + 1;

                    Dart dart_6 = Dart{id_dart, index[3], edge_3.id_edge, id_face};
                    darts.push_back(dart_6);
                    id_dart = id_dart + 1;



                    //Ao involution (3rd edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_5.id_dart, dart_6.id_dart);
                    ao.emplace_back(dart_6.id_dart, dart_5.id_dart);

                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_3);







                    //Create the forth edge of a face/////////////////////////////
                    Edge edge_4 = Edge{id_edge, index[3], index[0]};
                    k = 0;
                    for (auto &edge: edges) {
                        if ((edge.start == edge_4.end) && (edge_4.start == edge.end)) {
                            edge_4.id_edge = edge.id_edge;
                            k++;
                        }
                    }

                    //Create 2 darts for the forth edge:
                    Dart dart_7 = Dart{id_dart, index[3], edge_4.id_edge, id_face};
                    darts.push_back(dart_7);
                    vertices[index[3]-1].id_dart = id_dart;

                    id_dart = id_dart + 1;

                    Dart dart_8 = Dart{id_dart, index[0], edge_4.id_edge, id_face};
                    darts.push_back(dart_8);
                    id_dart = id_dart + 1;
                    id_face = id_face + 1; // increase the id for the face



                    //Ao involution (4rth edge) -> same edge & face, change vertex
                    ao.emplace_back(dart_7.id_dart, dart_8.id_dart);
                    ao.emplace_back(dart_8.id_dart, dart_7.id_dart);

                    if (k == 0) {
                        id_edge = id_edge + 1; //increase the id for the edge
                    }
                    edges.push_back(edge_4);




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







//                    else {
//                        Edge edge_1 = Edge{id_edge, index[0], index[1]};
//                        for (Edge edge: edges) {
//                            if (!(((edge_1.start == edge.start) and (edge_1.end == edge.end)) ||
//                                  ((edge_1.start == edge.end) and (edge_1.end == edge.start)))) {
//                                edges.push_back(edge_1);
//                                id_edge = id_edge + 1;
//                                //std::cout <<"1:"<< id_edge<< std::endl;
//                                std::cout <<"1:"<< edge_1.start << edge_1.end << std::endl;
//
//                            }
//                        }
//
//
//                        Edge edge_b = Edge{id_edge, index[1], index[2]};
//                        for (Edge edge: edges) {
//                            if (!(((edge_b.start == edge.start) and (edge_b.end == edge.end)) ||
//                                  ((edge_b.start == edge.end) and (edge_b.end == edge.start)))) {
//                                edges.push_back(edge_b);
//                                id_edge = id_edge + 1;
//                               // std::cout << "2:" << id_edge << std::endl;
//                            }
//                        }
//
//
//                        Edge edge_c = Edge{id_edge, index[2], index[3]};
//                        for (Edge edge:edges) {
//                            if (!(((edge_c.start== edge.start) and (edge_c.end == edge.end)) || ((edge_c.start == edge.end) and (edge_c.end == edge.start)))) {
//                                edges.push_back(edge_c);
//                                id_edge = id_edge+1;
//                            }
//                        }
//
//                        Edge edge_d = Edge{id_edge, index[3], index[0]};
//                        for (Edge edge:edges) {
//                            if (!(((edge_d.start== edge.start) and (edge_d.end == edge.end)) || ((edge_d.start == edge.end) and (edge_d.end == edge.start)))) {
//                                edges.push_back(edge_d);
//                                id_edge = id_edge+1;
//                            }
//                        }
//                    }
                }
            }
        }
    }

//    for (Edge edge : edges){
//        std::cout << edge.id_edge << " " << edge.start << " " << edge.end << std::endl;
//    }

//A2 involution -> same vertex & edge, change face
    for (auto &dart: darts) {
        for (auto &dart2: darts) {
            // std::cout << "oxi " << std::endl;
            if ((dart.id_vert == dart2.id_vert) && (dart.id_edge == dart2.id_edge) && (dart.id_face != dart2.id_face)) {
                a2.emplace_back(dart.id_dart, dart2.id_dart);
                //a2.emplace_back (dart2.id_dart, dart.id_dart);
//                std::cout << dart.id_vert << std::endl;
//                std::cout << "nai " << std::endl;
            }
        }
    }


    //Output darts in CSV file:
    std::ofstream myFile;
    myFile.open("darts.csv");

    myFile << "Dart ID" << ";" << " ao " << ";" << " a1 " << ";" << " a2 " << ";" << " a3 " << ";" << " v "
           << ";" << " e " << ";" << " f " << std::endl;

    for (int i = 0; i < darts.size(); i++) {
        int a3 = 0;
        myFile << "d" << darts[i].id_dart << ";" << ao[i].second << ";" << a1[i].second << ";" << a2[i].second
        << ";" << a3 << ";" << darts[i].id_vert << ";" << darts[i].id_edge << ";" << darts[i].id_face
        << std::endl;
    }


    //Output vertices in CSV file
    std::ofstream myFile1;
    myFile1.open("vertices.csv");

    myFile1 << "id" << ";" << " dart " << ";" << " x " << ";" << " y " << ";" << " z " << std::endl;

    for (Vertex vert: vertices) {
        myFile1  << vert.id_vert  << " ; " << vert.id_dart << " ; " <<  vert.point.x  << " ; " << vert.point.y  << " ; " << vert.point.z << std::endl;
    }



    for (Vertex vert: vertices) {
        std::cout << "vert_id : " << vert.id_vert << " id_dart : " << vert.id_dart << " point: " << vert.point << std::endl;

    }


    //A2 involution -> same vertex & edge, change face
//    for (int i =0; i < darts.size(); i++) {
//        //for (int j = 0; j < darts.size(); j++) {
//            std::cout << darts[i].id_edge << std::endl;
//
//
//        //}
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



//    for (Face face:faces){
//        std::cout << face.Face_ID << std::endl;
//    }

    //PRINT FOR DARTS
//    for (Dart dart:darts){
//        std::cout << dart.id_dart << " " << dart.id_vert << " " << dart.id_edge << " " << dart.id_face << std::endl;
//    }




//    for (int i =0; i < edges.size(); i++)  {
//        for (int j = 0; j < edges.size(); j++) {
//           if (Edge( edges)   Edge{edges[0], index[1], index[2], index[3]})
//        }
//    }



    //Print the vector edges
    for (Edge edge: edges) {
        //std::cout << edge.id_edge << edge.start << edge.end << std::endl;
    }

    // std::cout << vertices.size() << std::endl;
    // std::cout << faces.size() << std::endl;


    //Print vertices and faces input data
    for (Vertex vert: vertices) {
        std::cout << "vert_id : " << vert.id_vert << " id_dart : " << vert.id_dart << " point: " << vert.point << std::endl;

    }

    //Print the vertices
    for (int i=1; i<vertices.size()+1;i++){

       // std::cout << i << vertices[i-1].point.x << std::endl;
}

    for (Face face:faces) {
      //  std::cout<< face.a << face.b << face.c << face.d << std::endl;
    }

    for (Face face:faces) {
       //std::cout<< face.id_face << std::endl;
    }

   std::cout <<"test point: " << Point() << std::endl;

//
//    std::vector<Edge> edges; //create a vector named edges
//    int id_edge =1; //id_edge for edges
    for (Face face:faces) {
//        //std::cout<< face.a << face.b << face.c << face.d << std::endl;
//        //std::cout<< vertices[(face.a)-1].point  << std::endl;  // how to print the coordinates of the index that correspond to vertex
//
        //Create variable with vertices
        Point v1 = vertices[(face.a) - 1].point; //coordinates (x,y,z) for the first indices of each face
//        Point v2 = vertices[(face.b)-1].point;
//        Point v3 = vertices[(face.c)-1].point;
//        Point v4 = vertices[(face.d)-1].point;
//
//
       // std::cout << "v1:" << v1 << std::endl;
    }
//
//        // Initialize value for vector edges
//        if (id_edge ==1) {
//            Edge edge_1 = Edge{id_edge, (face.a), (face.b)};
//            edges.push_back(edge_1);
//            id_edge++;
//            Edge edge_2 = Edge{ id_edge , (face.b), (face.c)};
//            edges.push_back(edge_2);
//            id_edge++;
//            Edge edge_3 = Edge{ id_edge , (face.c), (face.d)};
//            edges.push_back(edge_3);
//            id_edge++;
//            Edge edge_4 = Edge{ id_edge , (face.d), (face.a)};
//            edges.push_back(edge_4);
//
//
//            for (Edge edge:edges) {
//                std::cout << edge.start << edge.end << std::endl;
//            }
//
//        }
//
//        // Create edges (4 for each face) and remove the duplicate edges
//        for (Edge edge:edges) {
//            Edge edge_1 = Edge{ id_edge , (face.a), (face.b)};
//            if  (!(((face.a == edge.start) and (face.b == edge.end)) || ((face.a == edge.end) and (face.b == edge.end)))) {
//                  edges.emplace_back(edge_1);
//                  id_edge++;
//                std::cout<< "check1" << id_edge  << std::endl;
//                }
//
//            Edge edge_2 = Edge{ id_edge , (face.b), (face.c)};
//            if  (!(((face.b == edge.start) and (face.c == edge.end)) || ((face.b == edge.end) and (face.c == edge.end)))) {
//                edges.emplace_back(edge_2);
//                id_edge++;
//                std::cout<< "check2"  << id_edge << std::endl;
//
//            }
//
//
//            Edge edge_3 = Edge{ id_edge , (face.c), (face.d)};
//
//            if  (!(((face.c == edge.start) and (face.d == edge.end)) || ((face.c == edge.end) and (face.d == edge.end)))) {
//                edges.emplace_back(edge_3);
//                id_edge++;
//                std::cout<< "check3" <<id_edge  << std::endl;
//            }
//
//            Edge edge_4 = Edge{ id_edge , (face.d), (face.a)};
//            if  (!(((face.d == edge.start) and (face.a == edge.end)) || ((face.d == edge.end) and (face.a == edge.end)))) {
//                edges.emplace_back(edge_4);
//                id_edge++;
//                std::cout<< "check4" << id_edge << std::endl;
//            }
//
//        }
//
//
//
//
//    }









  
  // ## Construct generalised map using the structures from Gmap.h ##
  
  // ## Output generalised map to CSV ##

  // ## Create triangles from the darts ##

  // ## Write triangles to obj ##
  
  return 0;
}