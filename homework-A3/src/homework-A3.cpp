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


// Definition of edge object: from vertex and weight value pair
class Edge {
    unsigned int from, vertex; // Connected vertices
    unsigned int weight;       // Weight of this edge
public:
    Edge() : from(0), vertex(0), weight(0) {}
    Edge(unsigned int f, unsigned int v, unsigned int w) : from(f), vertex(v), weight(w) {}
    Edge(const Edge& e) { from = e.from; vertex = e.vertex; weight = e.weight; }   // copy constructor

    friend std::ostream& operator<< (std::ostream &out, const Edge& edge);
    friend class ListGraph;
    friend class MstGraph;
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
    std::vector< std::vector<Edge> > graph;
    unsigned int edgeCnt;                       // Number of edges
    unsigned int nodeCnt;                       // Number of nodes
    double density;
    const unsigned int nodes_limit = 10000;
public:
    ListGraph() : edgeCnt(0), nodeCnt(0), density(0.0) {}   // Empty graph
    ListGraph(unsigned int n);                              // Graph with n nodes without any edges
    ListGraph(std::ifstream& inFile);                       // Graph defined in file as list of edges
    ListGraph(unsigned int** matrix, unsigned int N);       // Graph from simple 2D array

    int loadFromMatrix(unsigned int** matrix, size_t N);    // Load graph from 2D array
    int generateRandom(unsigned int N, double dens, unsigned int distanceRange);
    void printGraph();
    unsigned int V() { return nodeCnt; }
    unsigned int E() { return edgeCnt; }
    unsigned int D() { return density; }
    // Add() methods adds an edge if it is not there yet
    void Add(unsigned int from, unsigned int to, unsigned int weight);
    void Add(Edge &edge);
    // Launch algorithms on graph
    bool isConnected();
    unsigned int Dijkstra(unsigned int source); // Start from "source" node number
};

// Simple print to console the edge in format: (from ~ to, weight)
ostream& operator<< (ostream &out, const Edge &edge)
{
    out << "(" << std::setw(2) << edge.from << "~"<< std::setw(2) << edge.vertex << "," << std::setw(2) << edge.weight << ")"; return out;
}

class MstGraph : public ListGraph
{
    bool areConnected(unsigned int Xnode, unsigned int Ynode);
public:
    MstGraph() : ListGraph() {};
    MstGraph(unsigned int n) : ListGraph(n) {};
    MstGraph(std::ifstream& inFile) : ListGraph(inFile) {};

    unsigned int MST(unsigned int source);      // Start from "source" node number
    unsigned int MST();                         // MST using KRUSKAL algorithm
};

// Prepare graph nodes. Every node has now an empty vector of Edges
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

// Load graph from file.
// First line is an integer with nodes count.
// Every other line in file has three integer values for edge definition:
// For example: 0 1 7 - it means: edge from 0 to 1 with weight 7
ListGraph::ListGraph(std::ifstream& inFile)
{
    density = 0.0;
    edgeCnt = nodeCnt = 0;

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
            // Created graph with "n" nodes without any edge
            nodeCnt = n;

            while (true)
            {
                inFile >> from >> to >> weight; // Read one line from file
                if (inFile.eof()) break;        // Finish when file ends
                // Now read and add all nodes from file to graph
                this->Add(from, to, weight);
            }
        }
    }
}


ListGraph::ListGraph(unsigned int** matrix, unsigned int N)
{
    density = 0.0;
    edgeCnt = nodeCnt = 0;
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

// Insert new edge into graph (if it doesn't exist)
void ListGraph::Add(unsigned int from, unsigned int to, unsigned int weight)
{
    if (from >= nodeCnt || to >= nodeCnt)
    {
        cout << "Error! Cannot add edge. Edge number " << from << " or " << to << " doesn't exist." << endl;
        return;
    }
    // Add only if that edge doesn't exist in the graph
    if (std::find_if(graph[from].begin(), graph[from].end(), [&to](const Edge & obj) -> bool {return (obj.vertex == to);}) == graph[from].end())
    {
        graph[from].push_back(Edge(from, to, weight));
        graph[to].push_back(Edge(to, from, weight));
        edgeCnt++;
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
// Connections and weight values are random
// dens - graph density used when generating connections
// distanceRange - maximum distance when
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

// Simply print graph as a list of edges for every node
void ListGraph::printGraph()
{
    if (this->nodeCnt > 0)
    {
        unsigned int i = 0;
        for ( vector<Edge> node: this->graph)
        {
            cout << endl << setw(3) << i++ << " -> ";
            for ( Edge edge: node)
                cout << edge << ", ";
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

    if(this->nodeCnt == 0)
    {
        cout << "Graph is empty" << endl;
        return false;
    }
    unsigned int connected = 0;     // Start from '0' node - reference to that node connection list
    openset.insert(connected);      // Place node "0" in open set

    do
    {
        if (this->graph[connected].empty())         // Vertex without any edges
            break;

        // For every vertex reachable from 'connected' node:
        for(Edge node: this->graph[connected])
        {   // Add all reachable nodes to open set if they are not in closed set yet
            if (closedset.find(node.vertex) == closedset.end())
                openset.insert(node.vertex);        // Add to open set if not in closed set
        }
        closedset.insert(connected);                // This node is done
        openset.erase(connected);
        if (!openset.empty())
        {
            connected = *openset.begin();           // Take next node from open set
        }
        else if (closedset.size() == this->nodeCnt)
        {   // All nodes in closed set - finish
            isconnected = true;
            break;
        }
        else    // No nodes available in open set, finish
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
    return (average/(distances.size()-1));
}


template<typename A> void printEdges(A& edges, const std::string & delim)
{
    for (auto e: edges)
        cout << e << delim;
    cout << endl;
}

// MST with Prim's algorithm
unsigned int MstGraph::MST(unsigned int source)
{
    MstGraph* mst = new MstGraph(this->nodeCnt);        // Empty vertices forest
    unsigned int mst_cost = 0;
    std::multiset<Edge, EdgeCompare> EdgeCandidates;    // Set Vertices achievable from current MST
    std::vector<bool> covered(this->nodeCnt, false);
    unsigned int cur = source;                          // Initial vertex
    covered[cur] = true;

    do
    {
        // Add to priority queue all vertices achievable from newly added node (only if they are not in MST yet)
        for (Edge edge: graph[cur])
        {
            if(!covered[edge.vertex])
            {
                EdgeCandidates.insert(edge);
            }
        }

        auto candidateItr = EdgeCandidates.begin();
        // Avoid loops - remove edges for covered nodes
        while( covered[candidateItr->from] && covered[candidateItr->vertex])
        {
            EdgeCandidates.erase(candidateItr++);
        }

        // Add new node to MST
        mst->Add(candidateItr->from, candidateItr->vertex, candidateItr->weight);
        cout << "Edge added to MST: " << *candidateItr << endl;
        mst_cost += candidateItr->weight;

        covered[candidateItr->vertex] = true;
        cur = candidateItr->vertex;
        EdgeCandidates.erase(candidateItr);
    }while (mst->edgeCnt != (this->nodeCnt-1)); // until all nodes are in MST

    mst->printGraph();
    delete mst;
    return mst_cost;
}

// Checks if two graph Vertices are connected
bool MstGraph::areConnected(unsigned int Xnode, unsigned int Ynode)
{
    bool isconnected = false;
    std::set<unsigned int> openset;
    std::set<unsigned int> closedset;

    unsigned int connected = Xnode; // Start from Xnode
    openset.insert(connected);      // Place node Xnode in open set

    do
    {
        if (this->graph[connected].empty())         // Vertex without any edges
            break;

        // For every vertex reachable from 'connected' node:
        for(Edge node: this->graph[connected])
        {
            if(node.vertex == Ynode)    // Wanted Ynode node found! Success
                return true;
            if (closedset.find(node.vertex) == closedset.end())
                openset.insert(node.vertex);        // Add to open set if not in closed set
        }
        closedset.insert(connected);                // This node is done
        openset.erase(connected);
        if (!openset.empty())
            connected = *openset.begin();           // Take next node from open set
        else    // No nodes available in open set, finish
            break;
    }
    while ( true );   // empty node or all nodes reached
    return isconnected;
}

// Find MST using KRUSKAL's algorithm
// Returns MST cost
unsigned int MstGraph::MST()
{
    // 1. Prepare a set of edges ordered by weight
    std::multiset<Edge, EdgeCompare> EdgeCandidates; // Set of vertices ordered by weight
    // 2. Prepare an empty forest: every tree is an graph node
    MstGraph* mst = new MstGraph(this->nodeCnt);  // empty vertices forest
    unsigned int mstCost = 0;

    // 3. Fill the edge candidates set
    for (auto node_vector: this->graph)
    {
        for(auto edge: node_vector)
        {
            EdgeCandidates.insert(edge);
        }
    }

    // 4. Now pick the smallest weight node from the list:
    std::multiset<Edge, EdgeCompare>::iterator eit = EdgeCandidates.begin();
    while (mst->edgeCnt != (this->nodeCnt-1)) // if MST is not yet spanning
    {
        // check if both nodes from the edge are not in reached nodes: avoid loops
        if ( !mst->areConnected(eit->from, eit->vertex) )
        {
            mst->Add(eit->from, eit->vertex, eit->weight);
            mstCost += eit->weight;
            cout << "Added edge " << *eit << ", current MST has " << mst->edgeCnt << " edges and cost " << mstCost << endl;
        }

        eit++;// take another with smallest weight
    }
    mst->printGraph();
    delete mst;
    return mstCost;
}

int main() {
    MstGraph* pGraph = nullptr;
    ifstream graphFile;
    bool connected = false;

    cout << endl << endl << "Test input from file input.txt:" << endl;
    graphFile.open("input.txt", ios::in);
    pGraph = new MstGraph(graphFile);
    graphFile.close();
    cout << "Graph loaded from file:" << endl <<
            "Number of vertices: " << pGraph->V() << endl <<
            "Number of edges:    " << pGraph->E() << endl;

//    pGraph->printGraph();
    connected = pGraph->isConnected();
    cout << "This graph " << (connected ? "is" : "is not") << " connected" << endl;

    if (connected) {
        unsigned int mst = 0;
        cout << "Launch Dijkstra algorithm from node 0" << endl;
        unsigned int average_cost= pGraph->Dijkstra(0);
        cout << "Average cost from node 0 to every other is " << average_cost << endl;

        mst = pGraph->MST();
        cout << endl << "MST (Kruskal's algorithm) cost is " << mst << endl;
        mst = pGraph->MST(4);
        cout << endl << "MST (Prim's algorithm) cost is " << mst << endl;
    }
    else
        cout << "Dijkstra not launched on not connected graph" << endl;

    delete pGraph;
    return 0;
}
