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
    if (matrix == NULL)
    {
        cout << "Error! NULL pointer for matrix" << endl;
        return -1;
    }

    if (this->nodes.size() == 0)
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
            this->nodes.push_back(node_connections);
        }
    }
    else
    {
        cout << "ERROR! Graph is already initialized to size " << this->nodes.size() << endl;
        return -1;
    }
    return 0;
}

// Generate weighted and undirected graph with list representation
int ListGraph::generateRandom(size_t N)
{
    if (this->nodes.size() == 0)
    {
        cout << "Generate random graph" << endl;
        for (size_t i = 0; i < N; i++)
        {
            vector<Vertex> node_connections;
            for(size_t j = 0; j < N; j++)
            {
                if (i == j) continue;  // no loops
                else if ( prob() < density )
                {
                    Vertex v = Vertex(j, distance());
                    node_connections.push_back( v ); // Add connection from i to j node with distance
                }
            }
            this->nodes.push_back(node_connections);
        }
    }
    else
    {
        cout << "ERROR! Graph is already initialized to size " << this->nodes.size() << endl;
        return -1;
    }
    return 0;
}

void ListGraph::printGraph()
{
    if (this->nodes.size() > 0)
    {
        for ( int i = 0; i < static_cast<int>(this->nodes.size()); i++)
        {
            cout << endl;
            cout.width(3);
            cout << i << " -> ";
            for ( int j = 0; j < static_cast<int>(this->nodes[i].size()); j++)
                cout << this->nodes[i][j] << ", ";
        }
        cout << endl;
    }
    else
        cout << "Graph is empty." << endl;
}

// Print graph on screen as connectivity matrix
void ListGraph::printGraph(MatrixStyle style)
{
    unsigned int size = this->nodes.size();
    if (size == 0)
    {
        cout << "Graph is empty." << endl;
        return;
    }

    for ( unsigned int i = 0; i < size; i++)
    {
        cout.width(2);
        cout << endl << i << "|  ";
        vector<Vertex>::iterator it = this->nodes[i].begin();
        for ( unsigned int j = 0; j < size; j++)
        {
            if (it != this->nodes[i].end() && it->vertex == j)
            {
                cout << " ";
                if(style == MatrixStyle::MATRIX_WEIGHT)
                {
                    cout.width(2);
                    cout << it->weight << " ";
                }
                else
                {
                    cout << "1 ";
                }
                it++;
            }
            else
            {
                if(style == MatrixStyle::MATRIX_ARRAY)
                    cout << " 0 ";
                else if(style == MatrixStyle::MATRIX_WEIGHT)
                    cout << "  - ";
                else
                    cout << " - ";
            }
        }
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
        if (this->nodes[connected].empty()) // Vertex without any edges
            break;

        for(Vertex node: this->nodes[connected])    // For every vertex reachable from 'connected' node
        {   // Add all reachable nodes to open set if they are not in closed set yet
            if (closedset.find(node.vertex) == closedset.end())
                openset.insert(node.vertex);        // Add to open set if not in closed set
        }
        closedset.insert(connected);
        openset.erase(connected);
        if (!openset.empty()) {
            connected = *openset.begin();           // Take next node from open set
        }
        else if (closedset.size() == this->nodes.size()) {
            isconnected = true;
            break;
        }
        else    // No nodes available in open set
            break;
    }
    while ( true );   // empty node or all nodes reached
    return isconnected;
}

// Returns distance and prints sequence of nodes
unsigned int ListGraph::pathDijkstra()
{
    return 0;
}
