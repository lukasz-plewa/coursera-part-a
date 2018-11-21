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

inline double prob() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)); }
inline unsigned int distance(unsigned int limit) { return (rand() % limit); }

enum class MatrixStyle {MATRIX_ARRAY, MATRIX_CONNECTIONS_ONLY, MATRIX_WEIGHT};

// Declaration of special type for handling definition of edge: vertex and weight value pair
class Vertex {
    unsigned int vertex, weight;    // An vertex to which this edge connects to and weight of this edge
public:
    Vertex() : vertex(0), weight(0) {}                                  // default constructor
    Vertex(unsigned int v, unsigned int w) : vertex(v), weight(w) {}
    Vertex(const Vertex& v) { vertex = v.vertex; weight = v.weight; }   // copy constructor

    friend std::ostream& operator<< (std::ostream &out, const Vertex& edge);
    friend class ListGraph;
};

/*
 * Definition of object class which is handling weighted graph represented as a list of nodes:
  1 -> 1, 2, 3, 4
  2 -> 1
  3 -> 2, 4
  4 -> 2, 3
 */
class ListGraph {
    std::vector< std::vector<Vertex> >graph;    // Vector of vectors for graph representation as a list of connections
    unsigned int edgeCnt;                       // Number of edges
    unsigned int nodeCnt;                       // Number of nodes
    double density;
public:
    ListGraph() : edgeCnt(0), nodeCnt(0), density(0.0) {}
    ListGraph(unsigned int** matrix, unsigned int N);       // Constructor from simple 2D array

    // Methods
    int loadFromMatrix(unsigned int** matrix, size_t N);
    int generateRandom(unsigned int N, double dens, unsigned int distanceRange);
    void printGraph();
    void printGraph(MatrixStyle style);
    unsigned int V() { return nodeCnt; }
    unsigned int E() { return edgeCnt; }
    unsigned int D() { return density; }
    bool isConnected();                             // Launch the "is connected" algorithm on graph
    unsigned int Dijkstra(unsigned int source);
};

#endif /* LIST_GRAPH_H_ */
