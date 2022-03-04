// some standard libraries that are helpful for reading/writing text files
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

// additional function to create a string from a point (needed for triangulation later)
// Helped by this website: https://newbedev.com/c-member-reference-base-type-int-is-not-a-structure-or-union
std::string create_string(Point point) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << point.x << point.y << point.z;
    std::string created_string = stream.str();
    return created_string;
}


int main(int argc, const char * argv[]) {
    std::string file_in = "torus.obj";
    std::string file_out_obj = "torus_triangulated.obj";
    std::string file_out_csv_d = "torus_darts.csv";
    std::string file_out_csv_0 = "torus_vertices.csv";
    std::string file_out_csv_1 = "torus_edges.csv";
    std::string file_out_csv_2 = "torus_faces.csv";
    std::string file_out_csv_3 = "torus_volume.csv";

    // ## Read OBJ file ##

//Read vertices and faces from OBJ file

    // Creation of vectors
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;
    std::vector<Dart> darts;
    std::vector<Volume> volume;
    std::vector<std::pair<int, int>> a0;
    std::vector<std::pair<int, int>> a1;
    std::vector<std::pair<int, int>> a2;
    std::vector<std::pair<int, int>> a3;

    volume.emplace_back(0,0);


    //Output faces in CSV file (header)
    std::ofstream myFile4;
    myFile4.open("torus_faces.csv");
    myFile4 << "id " << ";" << " dart " << std::endl;

    for (Face face: faces) {
        myFile4 << face.id_face<< " ; " << face.id_dart << std::endl;
    }
    myFile4.close();


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
                    Vertex vertex = Vertex(id_vert, id_dart, coordinates[0], coordinates[1], coordinates[2]);
                    vertex.id_vert = id_vert;
                    id_vert++;
                    vertices.emplace_back(vertex);
                }
            }
            if (word == "f") {
                std::vector<int> index;
                while (iss >> word)
                    index.push_back(std::stoi(word));
                if (index.size() == 4) {
                     faces.emplace_back(Face(id_face, id_dart));


                    // ## Construct generalised map using the structures from Gmap.h ##

                    //Create the first edge of a face/////////////////////////////////
                    Edge edge_1 = Edge{id_edge, id_dart};
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
                    Edge edge_2 = Edge{id_edge, id_dart}; // index[1], index[2]};
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
                    Edge edge_3 = Edge{id_edge, id_dart};
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
                    Edge edge_4 = Edge{id_edge, id_dart};
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
                    a0.emplace_back(dart_1.id_dart, dart_2.id_dart);
                    a0.emplace_back(dart_2.id_dart, dart_1.id_dart);

                    //Ao involution (2nd edge) -> same edge & face, change vertex
                    a0.emplace_back(dart_3.id_dart, dart_4.id_dart);
                    a0.emplace_back(dart_4.id_dart, dart_3.id_dart);

                    //Ao involution (3rd edge) -> same edge & face, change vertex
                    a0.emplace_back(dart_5.id_dart, dart_6.id_dart);
                    a0.emplace_back(dart_6.id_dart, dart_5.id_dart);

                    //Ao involution (4rth edge) -> same edge & face, change vertex
                    a0.emplace_back(dart_7.id_dart, dart_8.id_dart);
                    a0.emplace_back(dart_8.id_dart, dart_7.id_dart);


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
    for (int i=1; i<edges.size(); i++) {
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
    myFile1 << "Dart ID" << ";" << " a0 " << ";" << " a1 " << ";" << " a2 " << ";" << " a3 " << ";" << " v "
           << ";" << " e " << ";" << " f " << std::endl;

    for (int i = 0; i < darts.size(); i++) {
        int a3 = 0;
        myFile1 << "d" << darts[i].id_dart << ";" << a0[i].second << ";" << a1[i].second << ";" << a2[i].second
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
    // index for vertices to make an OBJ file.
    // Create a vector for holding indexes of newly created faces, index_face;
    // Create a vector for vertices of triangulated surfaces, triangulation;
    // Create an unordered map for ensuring that there are no copies.
    int index_obj = 1;
    std::vector<std::vector<int>> index_face;
    std::vector<Point> triangulation;
    std::unordered_map<std::string, int> triangulation_vertices = {};

    // loop through all temporarily created faces, and find the sum of all vertices that make up the face.
    // divide by the number of vertices that make up the face: this lets us find the barycentric coordinate.
    // index the centre of the face (barycentric coordinate), using above created index_obj index.
    for (auto face:faces) {
        Point vertices_sum;
        for (auto &n: face.vertices) {
            vertices_sum += n.point;
        }

        auto centre_of_face = vertices_sum / face.vertices.size();
        int index_centre_of_face = index_obj++;

        triangulation_vertices[create_string(centre_of_face)] = index_centre_of_face;
        triangulation.emplace_back(centre_of_face);


        // loop through all temporarily created vertices, and create vectors for the indices of the triangles
        for (int i = 0; i<face.vertices.size(); i++) {
            std::vector<int> index_triangle_1;
            std::vector<int> index_triangle_2;

            // return the current vertex, and move on to the next vertex.
            // create a point to hold the barycentric coordinate of the edge.
            auto current_vertex = face.vertices[i].point;
            Point next_vertex;
            Point edge_barycentric;

            // find the barycentric coordinate by summing the points and dividing in half.
            // upon reaching the end of the vertices, create an edge joining the last vertex to the initial one.
            if (i< (face.vertices.size() - 1)) {
                next_vertex = face.vertices[i + 1].point;
                edge_barycentric = ((current_vertex + next_vertex) / 2);
            }
            else{
                next_vertex = face.vertices[0].point;
                edge_barycentric = ((current_vertex + next_vertex) / 2);
            }

            // see if current vertex already exists in the index.
            // if it does, return the vertex's index.
            // if it doesn't, create an index for the vertex.
            int index_current_vertex;
            if (triangulation_vertices.count(create_string(current_vertex))) {
                index_current_vertex = triangulation_vertices.at(create_string(current_vertex));
            }

            else{
                index_current_vertex = index_obj++;
                triangulation_vertices[create_string(current_vertex)] = index_current_vertex;
                triangulation.emplace_back(current_vertex);
            }

            // see if next vertex already exists in the index.
            // if it does, return the vertex's index.
            // if it doesn't create an index for the vertex.
            int index_next_vertex;
            if (triangulation_vertices.count(create_string(next_vertex))) {
                index_next_vertex = triangulation_vertices.at(create_string(next_vertex));
            }

            else{
                index_next_vertex = index_obj++;
                triangulation_vertices[create_string(next_vertex)] = index_next_vertex;
                triangulation.emplace_back(next_vertex);
            }

            // see if current edge's barycentric already exists in the index.
            // if it does, return the vertex's index.
            // if it doesn't, create an index for the vertex.
            int edge_barycentric_vertex;
            if (triangulation_vertices.count(create_string(edge_barycentric))) {
                edge_barycentric_vertex = triangulation_vertices.at(create_string(edge_barycentric));
            }

            else{
                edge_barycentric_vertex = index_obj++;
                triangulation_vertices[create_string(edge_barycentric)] = edge_barycentric_vertex;
                triangulation.emplace_back(edge_barycentric);
            }

            // all of the created indexes should now be pushed to make triangles.

            // Triangle 1: the centre of the face (barycentre),the current vertex, and the edge's barycentric vertex.
            index_triangle_1.emplace_back(index_centre_of_face);
            index_triangle_1.emplace_back(index_current_vertex);
            index_triangle_1.emplace_back(edge_barycentric_vertex);

            // Triangle 2: centre of face, edge's barycentric vertex, and the following vertex.
            index_triangle_2.emplace_back(index_centre_of_face);
            index_triangle_2.emplace_back(edge_barycentric_vertex);
            index_triangle_2.emplace_back(index_next_vertex);

            // Push the two new triangles to the index of newly created faces.
            index_face.emplace_back(index_triangle_1);
            index_face.emplace_back(index_triangle_2);

        }
    }

    // Final step: creating the CSVs

    std::ofstream output_vertex;
    output_vertex.open(file_out_csv_0);
    output_vertex << "id, dart, x, y, z\n";
    for (auto vertex: vertices) {
        output_vertex << vertex.id_vert << ", " << vertex.id_dart << ", " << vertex.x << ", " << vertex.y << ", " << vertex.z
                      << std::endl;
    }
    output_vertex.close();


    std::ofstream output_dart;
    output_dart.open (file_out_csv_d);
    output_dart << "id, a0, a1, a2, a3, vertex, edge, face\n";
    for (auto dart: darts) {
        output_dart << dart.id_dart << ", " << dart.a0 << ", " << dart.a1 << ", " << dart.a2 << ", " << dart.a3 << ", " << dart.id_vert << ", " << dart.id_edge << ", " << dart.id_face << std::endl;
    }
    output_dart.close();


    std::ofstream output_edge;
    output_edge.open (file_out_csv_1);
    output_edge << "id, dart\n";
    for (auto edge : edges) {
        output_edge << edge.id_edge << ", " << edge.id_dart << std::endl;
    }
    output_edge.close();


    std::ofstream output_face;
    output_face.open(file_out_csv_2);
    output_face << "id, dart\n";
    for (auto face: faces) {
        output_face << id_face << ", " << id_dart << std::endl;
    }
    output_face.close();


    std::ofstream output_volume;
    output_volume.open (file_out_csv_3);
    output_volume << "id, dart\n";
    for (auto volumee : volume) {
        output_volume << volumee.id_vol << ", " << volumee.id_dart << std::endl;
    }
    output_volume.close();


    std::ofstream output_obj;
    output_obj.open (file_out_obj);
    for (auto vertex : triangulation) {
        output_obj << "vertex " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    for (auto indices: index_face) {
        output_obj << "face " << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
    }
    output_obj.close();



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

//    //Print the first and second dart_id of the a0 involution
//    for (int i=0; i<a0.size();i++){
//        std::cout<< a2[i].first<<"  "<<a2[i].second<<std::endl;
//}

    //Print the darts.csv
//    for (int i=0; i<darts.size();i++){
//        int a3 = 0;
//        std::cout<< darts[i].id_dart << " " << a0[i].second << " " << a1[i].second << " " << a2[i].second << " " << a3 << " " << darts[i].id_vert << " " << darts[i].id_edge << " " << darts[i].id_face <<std::endl;
//    }

//    //Print the vertices
//    for (int i=1; i<vertices.size()+1;i++){
//         std::cout << i << vertices[i-1].point.x << std::endl;
//    }

  return 0;
}