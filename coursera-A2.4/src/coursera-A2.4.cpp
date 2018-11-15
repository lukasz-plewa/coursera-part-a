//============================================================================
// Name        : 4.cpp
// Author      : Lukasz Plewa (l.plewa@adbglobal.com)
// Version     :
// Copyright   : Copyright Lukasz Plewa
// Description :
//============================================================================

#include <iostream>
#include "list_graph.h"

using namespace std;

const unsigned int N = 10;

unsigned int** graphLoad(const unsigned int table[][N]);    // Load from static array
unsigned int** graphGenerate(size_t size);                  // Generate random matrix

// This is matrix representation of weighted undirected graph (could be also read from file)
unsigned int** graphinput;

unsigned int c[N][N] = {
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
#if 0
//    cout << "Load weighted graph from connectivity matrix:" << endl;
//    graphinput = graphGenerate(N);
//    graphinput = graphLoad(c);
#endif

    ListGraph graph;    // New empty graph object
#if 0
    cout << "Load graph from connectivity matrix" << endl;
    graph.loadFromMatrix(graphinput, N);
#endif
    graph.generateRandom(20);
    graph.generateRandom(30);
    graph.printGraph();
    cout << "This graph " << (graph.isConnected() ? "is" : "is not") << " connected" << endl;

    return 0;
}

// =============================================================================

// Generate 2D matrix as weighted graph representation with random values
unsigned int** graphGenerate(size_t size)
{
    srand(time(0));
    unsigned int** randomGraph = new unsigned int*[size];
    for (size_t i = 0; i < size; i++)
    {
        randomGraph[i] = new unsigned int[size];
        // heap created 2D array of unsigned int
        for(size_t j = 0; j < size; j++)
        {
            if (i == j)
                randomGraph[i][j] = 0;  // no loops
            else
                randomGraph[i][j] = randomGraph[j][i] = (prob() < 0.19 ? distance() : 0);
        }
    }
    return randomGraph;
}

// Loads graph from file or from static matrix
unsigned int** graphLoad(const unsigned int table[][N])
{
    unsigned int** graph = new unsigned int*[N];
    for (size_t i = 0; i < N; i++)
    {
        graph[i] = new unsigned int[N];
        // heap created 2D array of unsigned int
        for(size_t j = 0; j < N; j++)
        {
            graph[i][j] = table[i][j];
        }
    }
    return graph;
}

// =============================================================================
