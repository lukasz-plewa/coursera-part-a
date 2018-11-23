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

// This is matrix representation of weighted undirected graph (could be also read from file)
unsigned int** graphinput = nullptr;

#if 1
const unsigned int N = 10;
// This graph can be checked on http://graphonline.ru/en/?graph=KXdTUOPYKsvARtQu
unsigned int manual[N][N] = {
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
#else
const unsigned int N = 7;
// Graf z rysunku
unsigned int manual[N][N] = {
        /*   0  1  2  3  4  5  6 */
   /* 0 */ { 0, 2, 3, 8, 0, 0, 0 },
   /* 1 */ { 2, 0, 0, 0, 0, 4, 0 },
   /* 2 */ { 3, 0, 0, 0, 5, 2, 0 },
   /* 3 */ { 8, 0, 0, 0, 0, 1, 7 },
   /* 4 */ { 0, 0, 5, 0, 0, 0, 6 },
   /* 5 */ { 0, 4, 2, 1, 0, 0, 0 },
   /* 6 */ { 0, 0, 0, 7, 6, 0, 0 },
};
#endif

unsigned int** graphLoad(const unsigned int table[][N]);    // Load from static array
unsigned int** graphGenerate(size_t size);                  // Generate random matrix

#if 0
template<typename A> void printEdgeSet(A& edges)
{
    for (auto e: edges)
        cout << e << ", ";
    cout << endl;
}

#include <set>
int main()
{

    multiset<Edge, EdgeCompare> lista;
    lista.insert(Edge(1, 2, 4));
    lista.insert(Edge(1, 2, 3));
    lista.insert(Edge(1, 2, 4));
    lista.insert(Edge(1, 3, 4));
    lista.insert(Edge(1, 3, 3));
    lista.insert(Edge(2, 4, 3));
    lista.insert(Edge(3, 4, 12));
    lista.insert(Edge(4, 4, 8));
    lista.insert(Edge(0, 4, 1));

    cout << "Now print set:" << endl;
    printEdgeSet(lista);
}
#else

int main() {
    ListGraph* pGraph = nullptr;    // New empty graph object
    srand(time(0));
    pGraph = new ListGraph();
#if 0
//    cout << "Load weighted graph from connectivity matrix:" << endl;
//    graphinput = graphGenerate(N);
    graphinput = graphLoad(manual);
    cout << "Load graph from connectivity matrix" << endl;
    pGraph->loadFromMatrix(graphinput, N);
#else
    pGraph->generateRandom(20, 0.19, 10);
#endif
    pGraph->printGraph();
    bool connected = pGraph->isConnected();
    cout << "This graph " << (connected ? "is" : "is not") << " connected" << endl;

//    pGraph->printGraph(MatrixStyle::MATRIX_CONNECTIONS_ONLY);
//    pGraph->printGraph(MatrixStyle::MATRIX_ARRAY);
    pGraph->printGraph(MatrixStyle::MATRIX_WEIGHT);

    if (connected) {
        pGraph->Dijkstra(0);
        unsigned int mst = pGraph->MST(0);
        cout << "Mst cost is " << mst << endl;
    }
    else cout << "Dijkstra not launched on not connected graph" << endl;

    delete pGraph;
#if 1
    cout << endl << endl << "Now test input from file:" << endl;
    ifstream graphFile;
    graphFile.open("input.txt", ios::in);
    pGraph = new ListGraph(graphFile);
    graphFile.close();
    cout << "Graph loaded from file:" << endl <<
            "Number of vertices: " << pGraph->V() << endl <<
            "Number of edges:    " << pGraph->E() << endl;
    pGraph->printGraph();
    connected = pGraph->isConnected();
    cout << "This graph " << (connected ? "is" : "is not") << " connected" << endl;

    if (connected) {
        cout << "Launch Dijkstra algorithm from node 0" << endl;
        unsigned int average_cost= pGraph->Dijkstra(0);
        cout << "Average cost from node 0 to every other is " << average_cost << endl;
        unsigned int mst = pGraph->MST(0);
        cout << "Mst cost is " << mst << endl;
    }
    else cout << "Dijkstra not launched on not connected graph" << endl;

    delete pGraph;
#endif
    return 0;
}
#endif
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
                randomGraph[i][j] = randomGraph[j][i] = (prob() < 0.19 ? distance(20) : 0);
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
