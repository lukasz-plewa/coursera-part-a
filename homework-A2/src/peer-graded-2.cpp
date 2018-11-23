//============================================================================
// Name        : peer-graded-2.cpp
// Author      : Lukasz Plewa (ukaszyk@gmail.com)
// Version     :
// Copyright   : Copyright Lukasz Plewa
// Description : Graph as nodes list and Dijkstra implementation
//============================================================================

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

inline double prob() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)); }
inline unsigned int distance(unsigned int limit) { return (rand() % limit); }

// Declaration of class for vertex and weight value pair
class Vertex {
    unsigned int vertex, weight;    // An vertex to which this edge connects to and weight of this edge
public:
    Vertex() : vertex(0), weight(0) {}                                  // default constructor
    Vertex(unsigned int v, unsigned int w) : vertex(v), weight(w) {}
    Vertex(const Vertex& v) { vertex = v.vertex; weight = v.weight; }   // copy constructor

    friend std::ostream& operator<< (std::ostream &out, const Vertex& edge);
    friend class ListGraph;
};

// Definition of class which is handling weighted graph represented as a list of nodes:
class ListGraph {
    std::vector< std::vector<Vertex> >graph;    // Vector of vectors for graph representation as a list of connections
    unsigned int edgeCnt;                       // Number of edges
    unsigned int nodeCnt;                       // Number of nodes
    double density;
public:
    ListGraph() : edgeCnt(0), nodeCnt(0), density(0.0) {}

    // Methods
    int generateRandom(unsigned int N, double dens, unsigned int distanceRange);
    void printGraph();
    unsigned int V() { return nodeCnt; }
    unsigned int E() { return edgeCnt; }
    unsigned int D() { return density; }
    bool isConnected();                         // Launch the "is connected" algorithm on graph
    unsigned int Dijkstra(unsigned int source); // Returns average distance
};


ostream& operator<< (ostream &out, const Vertex &edge) {
    out << "(" << std::setw(2) << edge.vertex << "," << std::setw(2) << edge.weight << ")"; return out;
}

// Generate weighted and undirected graph with list representation
int ListGraph::generateRandom(unsigned int N, double dens, unsigned int distanceRange)
{
    if (this->nodeCnt == 0)
    {
        unsigned int i = 0;
        for (i = 0; i < N; i++) {
            vector<Vertex> node_connections;
            this->graph.push_back(node_connections);
        }

        for (i = 0; i < N; i++) {
            for(unsigned int j = (i+1); j < N; j++) {
                if ( prob() < dens ) {
                    unsigned int d = distance(distanceRange);
                    Vertex v = Vertex(j, d); // Graph must be undirected
                    Vertex b = Vertex(i, d); // so add edge connections to both vertices
                    graph[i].push_back( v ); // Add connection from i to j node with distance
                    graph[j].push_back( b ); // Add connection from j to i node with distance
                    edgeCnt++;
                }
            }
        }
        this->nodeCnt = N;
        this->density = dens;
    }
    else
    {
        cout << "ERROR! Graph is already initialized to size " << this->nodeCnt << endl;
        return -1;
    }
    return 0;
}

void ListGraph::printGraph()
{
    if (this->nodeCnt > 0)
    {
        unsigned int i = 0;
        for ( vector<Vertex> v: this->graph)
        {
            cout << endl << setw(3) << i++ << " -> ";
            for ( Vertex n: v)
                cout << n << ", ";
        }
        cout << endl;
    }
    else
        cout << "Graph is empty." << endl;
}

// Checks if graph is connected (all nodes are reachable)
bool ListGraph::isConnected()
{
    bool isconnected = false;
    std::set<unsigned int> openset;
    std::set<unsigned int> closedset;

    unsigned int connected = 0;     // Start from '0' node - reference to that node connection list
    openset.insert(connected);      // Place node "0" in open set

    do
    {
        if (this->graph[connected].empty()) // Vertex without any edges
            break;

        for(Vertex node: this->graph[connected])    // For every vertex reachable from 'connected' node
        {   // Add all reachable nodes to open set if they are not in closed set yet
            if (closedset.find(node.vertex) == closedset.end())
                openset.insert(node.vertex);        // Add to open set if not in closed set
        }
        closedset.insert(connected);
        openset.erase(connected);
        if (!openset.empty()) {
            connected = *openset.begin();           // Take next node from open set
        }
        else if (closedset.size() == this->nodeCnt) {
            isconnected = true;
            break;
        }
        else    // No nodes available in open set
            break;
    }
    while ( true );   // empty node or all nodes reached
    return isconnected;
}


// Prints distances for all reachable nodes
// @in source   starting vertex for distance calculation
// @return      average distance in graph
unsigned int ListGraph::Dijkstra(unsigned int source)
{
    if ( source >= this->nodeCnt)
    {
        cout << "Error! " << source << " node number is not in the graph!" <<
                "This graph has " << this->nodeCnt << " elements (vertices)" << endl;
        return 0;
    }

    // Initialize data structures
    vector<unsigned int> distances(this->nodeCnt, UINT_MAX);   // Calculated distances
    set<unsigned int>* Q = new set<unsigned int>;                   // Set of complete nodes
    for (unsigned int i=0; i<this->nodeCnt; ++i)
        Q->insert(i);
    for (auto v: this->graph[source])
        distances[v.vertex] = v.weight;     // Initialize distances from source node to connected nodes
    distances[source] = 0;                  // Distance to starting node is zero
    Q->erase(source);                        // source node is can be marked as complete
    unsigned int current = source;
    unsigned int current_old;
    unsigned int closest;
    unsigned int average = 0;

    while(!Q->empty())
    {
        //1. Find smallest not revealed distance in Q set
        closest = UINT_MAX;
        for (unsigned int i = 0; i < this->nodeCnt; ++i)
            if ( (Q->find(i) != Q->end()) && distances[i] < closest)
            {
                closest = distances[i];
                current = i;
            }
        if (current_old == current) // no more reachanble vertices
            break;
        //2. Mark that node as complete
        Q->erase(current);
        average += distances[current];
        //3. Update distances from current node
        for (auto v: this->graph[current])
        {
            if (distances[v.vertex] > (distances[current] + v.weight)) {
                distances[v.vertex] = (distances[current] + v.weight);
            }
        }
        current_old = current;
    }
#if 1
    cout << endl << "Dijstra solution from node " << source << " is:" << endl << "[node]: [distance]" << endl;
    for (unsigned int i = 0; i < distances.size(); ++i) {
        cout << setw(2) << i << ":  ";
        if (distances[i] == UINT_MAX)
                cout << "not reachable" << endl;
        else
            cout << distances[i] << endl;

    }
#endif
    delete Q;
    return (average/distances.size());
}

// MAIN
const double density = 0.19;
const unsigned int distance_range = 20;
const int numComputations = 100;
const unsigned int N = 60;

int main() {
    cout << "Graph and Dijkstra implementation" << endl; // prints Graph and Dijkstra implementation
    ListGraph* pGraph = nullptr;    // New empty graph object pointer
    unsigned int averageDistance = 0, ret = 0;
    srand(time(0));

// Compute for a set of randomly generated graphs an average shortest path.
    for (int i=0; i < numComputations; ++i)
    {
        pGraph = new ListGraph();
        pGraph->generateRandom(N, density, distance_range);
        pGraph->printGraph();
        ret = pGraph->Dijkstra(0);
        averageDistance += ret;
        cout << "Graph " << i << " average distance is: " << ret << endl;

        delete pGraph;
    }
    cout << "Average distance for all graphs is: " << averageDistance/numComputations << endl;
    return 0;
}
