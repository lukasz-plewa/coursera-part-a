/*
 * list_graph.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: L.Plewa
 */




#include <vector>
#include "list_graph.h"

using namespace std;

const double density = 0.19;

ostream& operator<< (ostream &out, const VertexWeight &edge) {
    out << "(" << edge.vertex << "," << edge.weight << ")"; return out;
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
            cout << "reading connections for node " << i << endl;
            vector<VertexWeight> node_connections;
            for (size_t j = 0; j < N; j++)
            {
                if (matrix[i][j])
                {
                    cout << " - " << matrix[i][j] << " - ";
                    node_connections.push_back(VertexWeight(j, matrix[i][j]));
                }
                cout << endl;
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

// Generate weighted and undirected graph with list representation
int ListGraph::generateRandom(size_t N)
{
    if (this->nodes.size() == 0)
    {
        cout << "Generate random graph" << endl;
        for (size_t i = 0; i < N; i++)
        {
            vector<VertexWeight> node_connections;
            for(size_t j = 0; j < N; j++)
            {
                if (i == j) continue;  // no loops
                else if ( prob() < density )
                {
                    VertexWeight v = VertexWeight(j, distance());
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
            cout << endl << " " << i;
            for ( int j = 0; j < static_cast<int>(this->nodes[i].size()); j++)
                cout << "\t-> " << this->nodes[i][j];
        }
        cout << endl;
    }
    else
        cout << "Graph is empty." << endl;
}

bool ListGraph::isConnected()
{
    bool isconnected = false;
    vector<unsigned int> openset;
    vector<unsigned int> closedset;

    for(auto const& n: openset)
        cout << n << " - ";
    cout << endl;

    unsigned int connected = 0;    // Start from '0' node - reference to that node connection list
    openset.push_back(connected);

    while (this->nodes[connected].size() != 0 && closedset.size() < this->nodes.size())   // empty node or all nodes reached
    {
        for(vector<VertexWeight>::iterator it = this->nodes[connected].begin(); it != this->nodes[connected].end(); ++it)
        {
            bool isclosed = false;
            for(auto closed: closedset) {
                if (closed == connected) {
                    isclosed = true;
                    break;
                }
            }
            if (isclosed) openset[(*it).id()] = true;
        }
        closedset[connected] = true;
        openset[connected] = false;
        for (unsigned int i=0; i < openset.size(); ++i){
            if (openset[i]){
                connected = i;
                break;
            }
        }
    }
    return isconnected;
}
