/*
 * list_graph.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: L.Plewa
 */




#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>
#include <climits>
#include "list_graph.h"

using namespace std;

const double density = 0.19;

ostream& operator<< (ostream &out, const Vertex &edge) {
    out << "(" << std::setw(2) << edge.vertex << "," << std::setw(2) << edge.weight << ")"; return out;
}

ListGraph::ListGraph(unsigned int** matrix, unsigned int N)
{
    if (matrix != NULL)
    {
        if( 0 != this->loadFromMatrix(matrix, N))
            cout << "ERROR creating ListGraph object!" << endl;
    }
    else
    {
        cout << "ERROR! Null pointer as parameter!" << endl;
    }
}

int ListGraph::loadFromMatrix(unsigned int** matrix, size_t N)
{
    if (matrix == NULL && N > 0)
    {
        cout << "Error! NULL pointer for matrix" << endl;
        return -1;
    }

    if (this->nodeCnt == 0)
    {
        for (size_t i = 0; i < N; i++)
        {
            cout << i << " |  ";
            vector<Vertex> node_connections;
            for (size_t j = 0; j < N; j++)
            {
                if (matrix[i][j])
                {
                    node_connections.push_back(Vertex(j, matrix[i][j]));
                }
                cout << " " << matrix[i][j] << " ";
            }
            cout << endl;
            this->graph.push_back(node_connections);
        }
        this->nodeCnt = N;
    }
    else
    {
        cout << "ERROR! Graph is already initialized to size " << this->nodeCnt << endl;
        return -1;
    }
    return 0;
}

// Generate weighted and undirected graph with list representation
int ListGraph::generateRandom(unsigned int N, double dens, unsigned int distanceRange)
{
    if (this->nodeCnt == 0 && N > 0)
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

// Print graph on screen as connectivity matrix
void ListGraph::printGraph(MatrixStyle style)
{
    if (this->nodeCnt == 0) {
        cout << "Graph is empty." << endl;
        return;
    }

    for ( unsigned int i = 0; i < this->nodeCnt; i++)
    {
        vector<Vertex>::iterator it = this->graph[i].begin();
        for ( unsigned int j = 0; j < this->nodeCnt; j++)
        {
            if (it != this->graph[i].end() && it->vertex == j)
            {
                cout << setw(2) << it->weight;
                it++;
            }
            else
            {
                cout << setw(2) << " 0";
            }
        }
        cout << endl;
    }
    cout << endl;
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
