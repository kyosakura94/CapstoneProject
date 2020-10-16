#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <vector>
#include <random>
#include "../../Rendering/3D/GameObject.h"


class Graph
{
    std::random_device rd; //seed generator
    std::default_random_engine generator{ rd() }; //Default random engine based on the initialized seed 
    float distance; //Will temporarily store the Euclidean distance between two vertices

public:

    std::vector<int> vertex_x; // Vector contains x-components of all vertices (nodes)
    std::vector<int> vertex_y; // Vector contains y-components of all vertices (nodes)
    int size_graph; // Size of the Graph in terms of numbers of nodes
    int source_node; // Source node
    float max_distance; // Maximum allowed Euclidean distance for two nodes to have an edge between them
    std::vector<int>* neighbor_table; // Will store all the neighbors of each vertex

    Graph();
    Graph(int, float);
    ~Graph();
    void draw_graph();
};

#endif /* Graph_h */
