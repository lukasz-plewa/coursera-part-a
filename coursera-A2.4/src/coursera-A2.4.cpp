//============================================================================
// Name        : 4.cpp
// Author      : Lukasz Plewa (l.plewa@adbglobal.com)
// Version     :
// Copyright   : Copyright Lukasz Plewa
// Description :
//============================================================================

#include <iostream>
#include <vector>

using namespace std;

const unsigned int N = 10;

// Declaration of special type for handling definition of edge: vertex and weight value pair
class VertexWeight {
    unsigned int vertex;    // An vertex to which this edge connects to
    unsigned int weight;    // Weight value of this edge
public:
    VertexWeight(unsigned int v, unsigned int w):vertex(v), weight(w) {}
    friend ostream& operator<< (ostream &out, const VertexWeight& edge);
};

ostream& operator<< (ostream &out, const VertexWeight &edge) {
    out << "(" << edge.vertex << "," << edge.weight << ")"; return out;
}

// This is matrix representation of weighted undirected graph (could be also read from file)
const unsigned int graphinput[N][N] = {
        {0,  6,  10, 0,  0,  0,  0,  0,  0,  0,  },
        {6,  0,  12, 11, 14, 0,  0,  0,  0,  0,  },
        {10, 12, 0,  12, 0,  0,  8,  16, 0,  0,  },
        {0,  11, 12, 0,  0,  6,  3,  0,  0,  0,  },
        {0,  14, 0,  0,  0,  4,  0,  0,  6,  0,  },
        {0,  0,  0,  6,  4,  0,  0,  0,  12, 0,  },
        {0,  0,  8,  3,  0,  0,  0,  0,  16, 6,  },
        {0,  0,  16, 0,  0,  0,  0,  0,  0,  8,  },
        {0,  0,  0,  0,  6,  12, 16, 0,  0,  13, },
        {0,  0,  0,  0,  0,  0,  6,  8,  13, 0,  },
};


int main() {
    cout << "Load weighted graph from connectivity matrix:" << endl;

    vector< vector<VertexWeight> >nodes;    // Vector of vectors for graph representation as a list of connections
    for (unsigned int i = 0; i < N; i ++)
    {
        vector<VertexWeight> node_connections;
        for (unsigned int j = 0; j < N; j++)
        {
            if (graphinput[i][j])
                node_connections.push_back(VertexWeight(j, graphinput[i][j]));
        }
        nodes.push_back(node_connections);
    }

    cout << "Now print graph represented as a list:" << endl;
    for ( int i = 0; i < static_cast<int>(nodes.size()); i++)
    {
        cout << endl << " " << i;
        for ( int j = 0; j < static_cast<int>(nodes[i].size()); j++)
            cout << " -> " << nodes[i][j];
    }
    return 0;
}
