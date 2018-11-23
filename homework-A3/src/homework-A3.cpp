//============================================================================
// Name        : homework-A3.cpp
// Author      : Lukasz Plewa
// Version     :
// Copyright   : Copyright Lukasz Plewa
// Description : Graph as nodes list with Dijkstra and MST implementation
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

inline double prob() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)); }
inline unsigned int distance(unsigned int limit) { return (rand() % limit); }


// Declaration of special type for handling definition of edge: vertex and weight value pair
class Edge {
    unsigned int from, vertex, weight;    // An vertex to which this edge connects to and weight of this edge
public:
    Edge() : from(0), vertex(0), weight(0) {}                                  // default constructor
    Edge(unsigned int f, unsigned int v, unsigned int w) : from(f), vertex(v), weight(w) {}
    Edge(const Edge& v) { from = v.from; vertex = v.vertex; weight = v.weight; }   // copy constructor

    friend std::ostream& operator<< (std::ostream &out, const Edge& edge);
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
    unsigned int V() { return nodeCnt; }
    unsigned int E() { return edgeCnt; }
    unsigned int D() { return density; }
    void Add(unsigned int from, unsigned int to, unsigned int weight); // Adds an edge if it is not there
    void Add(Edge &edge);
    bool isConnected();                             // Launch the "is connected" algorithm on graph
    unsigned int Dijkstra(unsigned int source);
    unsigned int MST(unsigned int source);
};

const double density = 0.19;
const unsigned int nodes_limit = 10000;

ostream& operator<< (ostream &out, const Edge &edge) {
    out << "(" << std::setw(2) << edge.from << "~"<< std::setw(2) << edge.vertex << "," << std::setw(2) << edge.weight << ")"; return out;
}

//bool operator<(const Edge & right) const
//{
//    return (this->weight < right.weight);
//}

//bool operator< (const Edge &left, const Edge &right)
//{
//    return left.weight < right.weight;
//}

ListGraph::ListGraph(unsigned int n)
{
    if ( n < nodes_limit)
    {
        for (unsigned int i = 0; i < n; i++)
        {
            vector<Edge> node;
            graph.push_back(node);
        }
        nodeCnt = n;
        edgeCnt = 0;
        density = 0.0;
    }
}

ListGraph::ListGraph(std::ifstream& inFile) : edgeCnt(0), nodeCnt(0)
{
    density = 0.0;
    if (inFile)
    {
        unsigned int n, from, to, weight;
        inFile >> n;
        cout << "Number of nodes: " << n << endl;
        if ( n < nodes_limit)
        {
            for (unsigned int i = 0; i < n; i++)
            {
                vector<Edge> node;
                graph.push_back(node);
            }
            if (n == graph.size())
                nodeCnt = n;
            else
                cout << "Error creating nodes vector" << endl;

            while (true)
            {
                inFile >> from >> to >> weight;
                if (inFile.eof()) break;
                this->Add(from, to, weight);
            }
        }
    }
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
            vector<Edge> node_connections;
            for (size_t j = 0; j < N; j++)
            {
                if (matrix[i][j])
                {
                    node_connections.push_back(Edge(i, j, matrix[i][j]));
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

void ListGraph::Add(unsigned int from, unsigned int to, unsigned int weight)
{
    if (from >= nodeCnt || to >= nodeCnt) // "from" vertex doesn't exist in graph
    {
        cout << "Error! Cannot add edge. Edge number " << from << " or " << to << " doesn't exist." << endl;
        return;
    }
    if (std::find_if(graph[from].begin(), graph[from].end(), [&to](const Edge & obj) -> bool {return (obj.vertex == to);}) == graph[from].end())
    {   // Only if that edge doesn't exist yet in the graph
        graph[from].push_back(Edge(from, to, weight));
        graph[to].push_back(Edge(to, from, weight));
        edgeCnt++;
//        cout << "Add (" << from << " - " << to << ")" << weight << endl;
    }
}

void ListGraph::Add(Edge &edge)
{
    unsigned int to = edge.vertex;
    unsigned int from = edge.from;
    if (std::find_if(graph[from].begin(), graph[from].end(), [&to](const Edge & obj) -> bool {return (obj.vertex == to);}) == graph[from].end())
    {   // Only if that edge doesn't exist yet in the graph
        graph[from].push_back(edge);
        graph[to].push_back(edge);
        edgeCnt++;
        cout << "Add (" << from << " - " << to << ")" << edge.weight << endl;
    }
}

// Generate weighted and undirected graph with list representation
int ListGraph::generateRandom(unsigned int N, double dens, unsigned int distanceRange)
{
    if (this->nodeCnt == 0 && N > 0)
    {
        unsigned int i = 0;
        for (i = 0; i < N; i++) {
            vector<Edge> node_connections;
            this->graph.push_back(node_connections);
        }
        this->nodeCnt = N;
        this->density = dens;
        for (i = 0; i < N; i++) {
            for(unsigned int j = (i+1); j < N; j++) {
                if ( prob() < dens ) {
                    unsigned int d = distance(distanceRange);
                    Add(i, j, d); // Graph must be undirected
                    edgeCnt++;
                }
            }
        }
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
        for ( vector<Edge> v: this->graph)
        {
            cout << endl << setw(3) << i++ << " -> ";
            for ( Edge n: v)
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
    if(nodeCnt == 0)
    {
        cout << "Graph is empty" << endl;
        return false;
    }

    do
    {
        if (this->graph[connected].empty()) // Vertex without any edges
            break;

        for(Edge node: this->graph[connected])    // For every vertex reachable from 'connected' node
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
        if (current_old == current)          // no more reachable vertices
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
    cout << "[node]: [distance]" << endl;
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


template<typename A> void printEdgeSet(A& edges)
{
    for (auto e: edges)
        cout << e << ", ";
    cout << endl;
}


unsigned int ListGraph::MST(unsigned int source)
{
    ListGraph* mst = new ListGraph(this->nodeCnt);              // empty vertices forest
    unsigned int mst_cost = 0;
    multiset<Edge, EdgeCompare> Q;                               // Set Vertices achievable from current MST
    vector<unsigned int> covered(nodeCnt, 0);
    unsigned int cur = 0;
    covered[0] = 1;

    do
    {
        // Add to priority queue all vertices achievable from newly added node (only if they are not in mst yet)
        for (auto v: graph[cur])
        {
            if(!covered[v.vertex]) {
//                cout << "Found not covered vertex from " << cur << ": " << v << endl;
                Q.insert(v);
            }
        }
//        printEdgeSet(Q);
        Edge minV = *Q.begin();
        mst->Add(minV.from, minV.vertex, minV.weight);
        mst_cost += minV.weight;
        for( multiset<Edge, EdgeCompare>::iterator v = Q.begin(); v != Q.end(); v++) {
            if(v->vertex == minV.vertex)
                Q.erase(v);
        }
        covered[minV.vertex] = 1;
        cur = minV.vertex;
//        cout << "Erased all connections to vertex " << minV.vertex << " from Q" << endl;
//        printEdgeSet(Q);

    }while (!Q.empty()); // until all nodes are in mst
    mst->printGraph();
    delete mst;
    return mst_cost;
}

int main() {
    ListGraph* pGraph = nullptr;
    ifstream graphFile;
    bool connected = false;

    cout << endl << endl << "Test input from file input.txt:" << endl;
    graphFile.open("input.txt", ios::in);
    pGraph = new ListGraph(graphFile);
    graphFile.close();
    cout << "Graph loaded from file:" << endl <<
            "Number of vertices: " << pGraph->V() << endl <<
            "Number of edges:    " << pGraph->E() << endl;

//    pGraph->printGraph();
    connected = pGraph->isConnected();
    cout << "This graph " << (connected ? "is" : "is not") << " connected" << endl;

    if (connected) {
        cout << "Launch Dijkstra algorithm from node 0" << endl;
        unsigned int average_cost= pGraph->Dijkstra(0);
        cout << "Average cost from node 0 to every other is " << average_cost << endl;

        cout << endl << "MST on that graph:";
        unsigned int mst = pGraph->MST(0);
        cout << "Mst cost is " << mst << endl;
    }
    else
        cout << "Dijkstra not launched on not connected graph" << endl;

    delete pGraph;
    return 0;
}
