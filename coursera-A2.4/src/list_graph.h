/*
 * list_graph.h
 *
 *  Created on: Nov 14, 2018
 *      Author: L.Plewa
 */

#ifndef LIST_GRAPH_H_
#define LIST_GRAPH_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

inline double prob() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)); }
inline unsigned int distance(unsigned int limit) { return (rand() % limit); }

enum class MatrixStyle {MATRIX_ARRAY, MATRIX_CONNECTIONS_ONLY, MATRIX_WEIGHT};

// Declaration of special type for handling definition of edge: vertex and weight value pair
class Edge {
    unsigned int from, vertex, weight;    // An vertex to which this edge connects to and weight of this edge
public:
    Edge() : from(0), vertex(0), weight(0) {}                                  // default constructor
    Edge(unsigned int f, unsigned int v, unsigned int w) : from(f), vertex(v), weight(w) {}
    Edge(const Edge& v) { from = v.from; vertex = v.vertex; weight = v.weight; }   // copy constructor

    unsigned int getF() { return from; }
    unsigned int getV() { return vertex; }
    unsigned int getW() { return weight; }
    friend std::ostream& operator<< (std::ostream &out, const Edge& edge);
    bool operator<(const Edge & right) const
    {   // Compare weight only for different edges: needed by std::set<Edge>
        if ((this->from != right.from) || (this->vertex != right.vertex))
            return (this->weight <= right.weight);
        return false;

    }
    friend class ListGraph;
    friend class EdgeCompare;
};

class EdgeCompare{
public:
    bool operator()( const Edge& lhs, const Edge& rhs)
    {
            return (lhs.weight < rhs.weight);
    }
};
/*
 * Definition of object class which is handling weighted graph represented as a list of nodes:
  1 -> 1, 2, 3, 4
  2 -> 1
  3 -> 2, 4
  4 -> 2, 3
 */
class ListGraph {
    std::vector< std::vector<Edge> >graph;    // Vector of vectors for graph representation as a list of connections
    unsigned int edgeCnt;                       // Number of edges
    unsigned int nodeCnt;                       // Number of nodes
    double density;
public:
    ListGraph() : edgeCnt(0), nodeCnt(0), density(0.0) {}   // Create empty graph
    ListGraph(unsigned int n);                              // Create graph with n nodes without any edges
    ListGraph(std::ifstream& inFile);                       // Create graph defined in file as list of edges
    ListGraph(unsigned int** matrix, unsigned int N);       // Create graph from simple 2D array

    // Methods
    int loadFromMatrix(unsigned int** matrix, size_t N);
    int generateRandom(unsigned int N, double dens, unsigned int distanceRange);
    void printGraph();
    void printGraph(MatrixStyle style);
    unsigned int V() { return nodeCnt; }
    unsigned int E() { return edgeCnt; }
    unsigned int D() { return density; }
    void Add(unsigned int from, unsigned int to, unsigned int weight); // Adds an edge if it is not there
    void Add(Edge &edge);
    bool isConnected();                             // Launch the "is connected" algorithm on graph
    unsigned int Dijkstra(unsigned int source);
    unsigned int MST(unsigned int source);
};

#endif /* LIST_GRAPH_H_ */
