/*
 * list_graph.h
 *
 *  Created on: Nov 14, 2018
 *      Author: L.Plewa
 */

#ifndef LIST_GRAPH_H_
#define LIST_GRAPH_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

inline double prob() { double p = (static_cast<double>(rand() % 1000) / 1000.0); return p; }
inline unsigned int distance() { return (rand() % 100); }

// Declaration of special type for handling definition of edge: vertex and weight value pair
class Vertex {
    unsigned int vertex, weight;    // An vertex to which this edge connects to and weight of this edge
public:
    Vertex() : vertex(0), weight(0) {}
    Vertex(unsigned int v, unsigned int w) : vertex(v), weight(w) {}
    unsigned int id() { return vertex; }
    friend std::ostream& operator<< (std::ostream &out, const Vertex& edge);
    friend class ListGraph;
};

/*
 * Definition of object class which is handling weighted graph represented as a list of nodes:
  1 -> 1 -> 2 -> 3 -> 4
  2 -> 1
  3 -> 2 -> 4
  4 -> 2 -> 3
 */
class ListGraph {
    std::vector< std::vector<Vertex> >nodes;    // Vector of vectors for graph representation as a list of connections
public:
    ListGraph() { srand(time(0)); };
    ListGraph(unsigned int** matrix, unsigned int N);

    // Methods
    int loadFromMatrix(unsigned int** matrix, size_t N);
    int generateRandom(size_t size);
    void printGraph();
    bool isConnected();                 // Launch the "is connected" algorithm on graph
};

#endif /* LIST_GRAPH_H_ */
