#ifndef GRAPH_H_
#define GRAPH_H_


#include <iostream>
#include <vector>
#include <ctime>
#include <set>
#include <algorithm>
#include <climits>
#include <utility>
#include <memory>


// Edge object: (from, to) vertex Ids and weight (edge cost)
class Edge {
    const unsigned int from, vertex; // Connected vertices
    const unsigned int weight;       // Weight of this edge
public:
    Edge(unsigned int f, unsigned int v, unsigned int w) : from(f), vertex(v), weight(w) {}
    Edge(const Edge& e) : from(e.from), vertex(e.vertex), weight(e.weight) {}   // copy constructor

    // Methods
    unsigned int From() const { return this->from; }
    unsigned int To() const { return this->vertex; }
    unsigned int Weight() const { return this->weight; }

    friend std::ostream& operator<< (std::ostream &out, const Edge& edge);
    friend class EdgeCompare;
};

// Class with overloaded operator () for proper sorting in std::multiset<Edge>
class EdgeCompare{
public:
    bool operator()( const Edge& lhs, const Edge& rhs)
    {
        return (lhs.weight < rhs.weight);
    }
};

enum class Colour {NONE, RED, BLUE};

std::ostream& operator<< (std::ostream &out, const Colour &c);

// Single vertex class. Contains information about colour and keeps edge list
class Vertex
{
public:
    explicit Vertex(unsigned int vid) : id(vid), color(Colour::NONE) {}
    explicit Vertex(unsigned int vid, Colour c ) : id(vid), color(c) {}
    void Add(unsigned int to, unsigned int weight);
    bool hasConnection(unsigned int to);
    void printEdgeConnections();
    unsigned int Id() { return id; }
    Colour getColour() { return color; }
    bool Empty() { return edges.empty(); }
private:
    const unsigned int id;
    Colour color;
    std::vector<Edge> edges;

public:
    friend class ListGraph;
    friend class MstGraph;
};

/*
 * Class which is handling weighted graph represented as a list of nodes:
  1 -> 1, 2, 3, 4
  2 -> 1
  3 -> 2, 4
  4 -> 2, 3
  Vector of vectors for graph representation as a list of connections
 */
class ListGraph
{
protected:
    std::vector< std::unique_ptr<Vertex> > graph;
    unsigned int edgeCnt;                       // Number of edges
    unsigned int nodeCnt;                       // Number of nodes
    double density;
    const unsigned int nodes_limit = 10000;
public:
    ListGraph() : edgeCnt(0), nodeCnt(0), density(0.0) {}   // Empty graph
    ListGraph(unsigned int n);                              // Graph with n nodes without any edges

    void printGraph();
    unsigned int V() { return nodeCnt; }
    unsigned int E() { return edgeCnt; }
    unsigned int D() { return density; }
    // Launch algorithms on graph
    bool isConnected();
    unsigned int Dijkstra(unsigned int source); // Start from "source" node number
protected:
    // Add() methods adds an edge if it is not there yet
    void Add(unsigned int from, unsigned int to, unsigned int weight);
    void Add(Edge &edge);
    void setColour(unsigned int id, Colour c) { graph[id]->color = c; }
    Colour getColour(unsigned int id) { return graph[id]->color; }
};

// Inherited class to implement MST algorithms on it
class MstGraph : public ListGraph
{
protected:
    bool areConnected(unsigned int Xnode, unsigned int Ynode, Colour c);
public:
    MstGraph() : ListGraph() {};
    MstGraph(unsigned int n) : ListGraph(n) {};

    unsigned int MST(unsigned int source);      // Start from "source" node number - Prim's algorithm
    unsigned int MST();                         // MST using KRUSKAL algorithm
};


#endif // GRAPH_H_