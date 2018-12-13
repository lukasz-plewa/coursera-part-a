//============================================================================
// Name        : homework-A3.cpp
// Author      : Lukasz Plewa
// Version     :
// Copyright   : Copyright Lukasz Plewa
// Description : Graph as nodes list with Dijkstra and MST implementation
//============================================================================

#include <iomanip>
#include <iterator>

#include "graph.h"

//using namespace std;


// Simple print to console the edge in format: (from ~ to, weight)
std::ostream& operator<< (std::ostream &out, const Edge &edge)
{
    out << "(" << std::setw(2) << edge.from << "~"<< std::setw(2) << edge.vertex << "," << std::setw(2) << edge.weight << ")"; return out;
}

std::ostream& operator<< (std::ostream &out, const Colour &c)
{
    switch(c)
    {
        case Colour::NONE:
            out << "."; break;
        case Colour::RED:
            out << "X"; break;
        case Colour::BLUE:
            out << "O"; break;
    }
    return out;
}

void Vertex::Add(unsigned int to, unsigned int weight)
{
    // Add only if that edge doesn't exist in the graph
    if (std::find_if(edges.begin(), edges.end(), [&to](Edge & obj) -> bool {return (obj.To() == to);}) == edges.end())
    {
        edges.push_back(Edge(this->id, to, weight));
    }
}

bool Vertex::hasConnection(unsigned int to)
{
    return (std::find_if(edges.begin(), edges.end(), [&to](Edge & obj) -> bool {return (obj.To() == to);}) != edges.end());
}

void Vertex::printEdgeConnections()
{
    for ( Edge n: this->edges)
        std::cout << n << ", ";
}

// Prepare graph nodes. Every node has now an empty vector of Edges
ListGraph::ListGraph(unsigned int n)
{
    if ( n < nodes_limit)
    {
        graph.reserve(n);
        for (unsigned int i = 0; i < n; i++)
        {
            graph.push_back( std::unique_ptr<Vertex>(new Vertex(i, Colour::NONE)) );
        }

        std::cout << "Created vector of Vertices of count: " << graph.size() << std::endl;
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
        std::istream_iterator<int> start(inFile), end;
        std::vector<int> graph_file(start, end);
        n = graph_file[0];
        //logIt(logDEBUG) << "Number of nodes: " << n << "\n";
        if ( n < nodes_limit)
        {
            graph.reserve(n);
            for (unsigned int i = 0; i < n; i++)
            {
                graph.push_back( std::unique_ptr<Vertex>(new Vertex(i, Colour::NONE)) );
            }
            if (n == graph.size())
                nodeCnt = n;
            else
                std::cout << "Error creating nodes vector\n";

            for (auto it = (graph_file.begin()+1); it != graph_file.end(); ++it)
            {
                from = *it++;
                to = *it++;
                weight = *it;
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
            std::cout << "ERROR creating ListGraph object!" << std::endl;
    }
    else
    {
        std::cout << "ERROR! Null pointer as parameter!" << std::endl;
    }
}

int ListGraph::loadFromMatrix(unsigned int** matrix, size_t N)
{
    if (matrix == NULL && N > 0)
    {
        std::cout << "Error! NULL pointer for matrix" << std::endl;
        return -1;
    }

    if (this->nodeCnt == 0)
    {
        graph.reserve(N);
        for (unsigned int i = 0; i < N; i++)
        {
            graph.push_back( std::unique_ptr<Vertex>(new Vertex(i, Colour::NONE)) );
        }
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = i; j < N; j++)
            {
                if (matrix[i][j])
                {
                    this->Add(i, j, matrix[i][j]);
                }
                //logIt(logDEBUG) << " " << matrix[i][j] << " ";
            }
            //logIt(logDEBUG) << "\n";
        }
        this->nodeCnt = N;
    }
    else
    {
        std::cout << "ERROR! Graph is already initialized to size " << this->nodeCnt << std::endl;
        return -1;
    }
    return 0;
}

// Insert new edge into graph (if it doesn't exist)
void ListGraph::Add(unsigned int from, unsigned int to, unsigned int weight)
{
    if (from >= nodeCnt || to >= nodeCnt)
    {
        std::cout << "Error! Cannot add edge. Edge number " << from << " or " << to << " doesn't exist." << std::endl;
        return;
    }
    // Add only if that edge doesn't exist in the graph
    if (!graph[from]->hasConnection(to))
    {   // Only if that edge doesn't exist yet in the graph
        graph[from]->Add(to, weight);
        graph[to]->Add(from, weight);
        edgeCnt++;
        std::cout << "Add (" << from << " - " << to << ")" << weight << "\n";
    }
}

void ListGraph::Add(Edge &edge)
{
    if (!graph[edge.From()]->hasConnection(edge.To()))
    {   // Only if that edge doesn't exist yet in the graph
        graph[edge.From()]->Add(edge.To(), edge.Weight());
        graph[edge.To()]->Add(edge.From(), edge.Weight());
        edgeCnt++;
        //logIt(logDEBUG) << "Add (" << from << " - " << to << ")" << edge.weight << "\n";
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

        graph.reserve(N);
        for (i = 0; i < N; i++) {
            graph.push_back( std::unique_ptr<Vertex>(new Vertex(i, Colour::NONE)) );
        }
        this->nodeCnt = N;
        this->density = dens;
        for (i = 0; i < N; i++) {
            for(unsigned int j = (i+1); j < N; j++) {
                if ( prob() < dens ) {
                    unsigned int d = distance(distanceRange);
                    this->Add(i, j, d);
                }
            }
        }
    }
    else
    {
        std::cout << "ERROR! Graph is already initialized to size " << this->nodeCnt << std::endl;
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
        for (auto &v: this->graph)
        {
            std::cout << std::endl << std::setw(3) << i++ << " -> ";
            v->printEdgeConnections();
            
        }
        std::cout << std::endl;
    }
    else
        std::cerr << "Graph is empty." << "\n";
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
        //logIt(logERROR) << "Graph is empty." << "\n";
        return false;
    }

    do
    {
        if (this->graph[connected]->Empty()) // Vertex without any edges
            break;

        for(Edge e: this->graph[connected]->edges)    // For every vertex reachable from 'connected' node
        {   // Add all reachable nodes to open set if they are not in closed set yet
            if (closedset.find(e.To()) == closedset.end())
                openset.insert(e.To());        // Add to open set if not in closed set
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
    if ( source >= nodeCnt)
    {
        std::cerr<< "Error! " << source << " node number is not in the graph!" <<
                "This graph has " << nodeCnt << " elements (vertices)" << "\n";
        return 0;
    }

    // Initialize data structures
    std::vector<unsigned int> distances(nodeCnt, UINT_MAX);   // Calculated distances
    std::set<unsigned int>* Q = new std::set<unsigned int>;        // Set of complete nodes
    for (unsigned int i=0; i<this->nodeCnt; ++i)
        Q->insert(i);
    for (auto v: this->graph[source]->edges)
        distances[v.To()] = v.Weight();     // Initialize distances from source node to connected nodes
    distances[source] = 0;                  // Distance to starting node is zero
    Q->erase(source);                       // source node is can be marked as complete
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
        for (auto v: this->graph[current]->edges)
        {
            if (distances[v.To()] > (distances[current] + v.Weight())) {
                distances[v.To()] = (distances[current] + v.Weight());
            }
        }
        current_old = current;
    }

    std::cout << "\nDijstra solution from node " << source << " is:\n" << "[node]: [distance]\n";
    for (unsigned int i = 0; i < distances.size(); ++i) {
        std::cout << std::setw(2) << i << ":  ";
        if (distances[i] == UINT_MAX)
            std::cout << "not reachable\n";
        else
            std::cout << distances[i] << "\n";

    }

    delete Q;
    return (average/distances.size());
}


template<typename A> void printEdges(A& edges, const std::string & delim)
{
    for (auto e: edges)
        std::cout << e << delim;
    std::cout << std::endl;
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
        for (auto edge: graph[cur]->edges)
        {
            if(!covered[edge.To()])
            {
                EdgeCandidates.insert(edge);
            }
        }

        auto candidateItr = EdgeCandidates.begin();
        // Avoid loops - remove edges for covered nodes
        while( covered[candidateItr->From()] && covered[candidateItr->To()])
        {
            EdgeCandidates.erase(candidateItr++);
        }

        // Add new node to MST
        mst->Add(candidateItr->From(), candidateItr->To(), candidateItr->Weight());
        std::cout << "Edge added to MST: " << *candidateItr << std::endl;
        mst_cost += candidateItr->Weight();

        cur = candidateItr->To();
        covered[cur] = true;
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
        if (this->graph[connected]->Empty())         // Vertex without any edges
            break;

        // For every vertex reachable from 'connected' node:
        for(Edge node: this->graph[connected]->edges)
        {
            if(node.To() == Ynode)    // Wanted Ynode node found! Success
                return true;
            if (closedset.find(node.To()) == closedset.end())
                openset.insert(node.To());        // Add to open set if not in closed set
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
    for (auto &pnode: this->graph)
    {
        for(auto edge: pnode->edges)
        {
            EdgeCandidates.insert(edge);
        }
    }

    // 4. Now pick the smallest weight node from the list:
    std::multiset<Edge, EdgeCompare>::iterator eit = EdgeCandidates.begin();
    while (mst->edgeCnt != (this->nodeCnt-1)) // if MST is not yet spanning
    {
        // check if both nodes from the edge are not in reached nodes: avoid loops
        if ( !mst->areConnected(eit->From(), eit->To()) )
        {
            mst->Add(eit->From(), eit->To(), eit->Weight());
            mstCost += eit->Weight();
            std::cout << "Added edge " << *eit << ", current MST has " << mst->edgeCnt << " edges and cost " << mstCost << std::endl;
        }

        eit++;// take another with smallest weight
    }
    mst->printGraph();
    delete mst;
    return mstCost;
}
