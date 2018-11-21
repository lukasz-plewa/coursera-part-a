#include <iostream>
#include <vector>

using namespace std;


// Generates random number in range [start, end)
inline int random_in_range(int start, int end) {
    return (rand() % (end - start)) + start;
}

// Random float between [0, 1]
inline float random_float() {
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

// Determines if set a is empty
inline bool not_empty(bool set[], int n) {
    for (int i = 0; i < n; i++) {
        if (set[i])
            return true;
    }

    return false;
}



class Graph {

    private:

    vector< vector<int> > graph;

    public:

    int size;

    // Constructor. Initializes with default values
    // unless asked not to, in which case user
    // may manually initialize at a later point 
    Graph(int size = 30, int max_weight = 9, float density = 0.2):size(size) {

        // For each location, if it's self-referential, make it 0
        // Otherwise, DENSITY percent chance of random non-zero weight
        for (int i = 0; i < size; i++) {
            vector<int> t;
            for (int j = 0; j < size; j++) {
                if (i == j || random_float() >= density)
                    t.push_back(0);
                else
                    t.push_back(random_in_range(1, max_weight + 1));
            }
            graph.push_back(t);
        }

    }

    // Returns all neighbors, accompanied by weights
    vector<int> neighbors(int node) {

        vector<int> n;
        // For each nodinte i, if it's connected to inputted node
        // then add {i,int weight} to vector
        for (int i = 0; i < size; i++) {
            if (graph[node][i])
                n.push_back(i);
        }

        return n;
    }

    // Returns weight between two nodes
    int weight(int node1, int node2) const {
        return graph[node1][node2];
    }

};

// Operator overload to print graph
// Does not pad for weights with multiple digits
ostream& operator<<(ostream& out, const Graph& g) {
    for (int i = 0; i < g.size; i++) {
        for (int j = 0; j < g.size; j++) {
            out << g.weight(i, j) << " ";
        }
        out << endl;
    }
    out << endl;

    return out;
}

// Determines if graph is connected
bool is_connected(Graph &graph) {

    int n = graph.size;

    bool *open = new bool[n];
    bool *closed = new bool[n];

    // Initialize such that only starting node is open
    // And no nodes are closed
    for (int i = 0; i < n; i++) {
        open[n] = false;
        closed[n] = false;
    }
    open[0] = true;


    
    while (not_empty(open, n)) {

        int on;
        for (int i = 0; i < n; i++) {
            if (open[i])
                on = i;
        }

        open[on] = false;
        closed[on] = true;
 
        vector<int> nb = graph.neighbors(on);
        
        // For all neighbors of on, 
        // if they're not already in closed, add to open
        for (int i = 0; i < nb.size(); i++) {
            if (!closed[nb[i]])
                open[nb[i]] = true;
        }
    }

    // At the end, if any node is unclosed, graph is incomplete
    for (int i = 0; i < n; i++) {
        if (!closed[i])
            return false;
    }

    // If all nodes have been reached, graph is complete
    return true;
}


// Dijkstra's shortest path algorithm!  :)
int dijkstra(Graph &g, int start, int goal) {

    // Initialize open set, distance and previous "maps"
    bool open[g.size];
    int distance[g.size];
    int previous[g.size];

    for (int i = 0; i < g.size; i++) {
        open[i] = true;
        distance[i] = INT_MAX;
        previous[i] = -1;
    }

    distance[start] = 0;

    // While there are open nodes
    while (not_empty(open, g.size)) {

        // Get open node with lowest distance
        int lowest = INT_MAX;
        int root = -1;
        for (int i = 0; i < g.size; i++) {
            if (distance[i] < lowest && open[i]) {
                lowest = distance[i];
                root = i;
            }
        }

        // Close this node
        open[root] = false;

        // We're done if we're at our goal
        if (root == goal)
            return distance[root];


        // For each neighbor, if we've found a new fastest path,
        // Record it as such, update distance and previous accordingly
        vector<int> n = g.neighbors(root);

        for (int i = 0; i < n.size(); i++) {
            int potential = distance[root] + g.weight(root, n[i]);
            if (potential < distance[n[i]]) {
                distance[n[i]] = potential;
                previous[n[i]] = root;
            }
        }
    }

    return -1;
}



int main() {

    cout << "This was a really awesome project.  I learned plenty the lectures on C++, but I learned far more from actually implementing Dijkstraâ€™s algorithm, as is usually the case with computer programming.  I elected to stray from the suggested format because I felt it was bloated and partially unnecessary, but I did implement a more lightweight graph class and a dijkstra() function.  I learned that class constructors can use initializers to initialize class variables - Constructor(int argument):var_name(argument).  I learned that vectors are incredibly powerful when you have an unknown number of things to be returned (for instance, the number of neighbors on a given node).  While I already new of Dijkstraâ€™s algorithm along with A* and IDA*, and had implemented all three in C, doing so in C++ was far cleaner and easier to understand.  There was no mess with pointers.  I didnâ€™t need to split up various functions into several different files to make my code coherent, meaning I didnâ€™t need to bother with a makefile.  And everything makes logical sense together.  Everythingâ€™s coherent.  I love C, but C++ continues to appear superior the more I learn about it." << endl << endl;

    Graph *g;
    do {
        g = new Graph(50, 10, 0.2);
    } while (!is_connected(*g));

    int total = 0;
    for (int i = 1; i < 50; i++)
        total += dijkstra(*g, 0, i);
    float avg = static_cast<float>(total) / 49.0;
    
    cout << "With 50 nodes, 1-10 weights, and density of 0.2," << endl;
    cout << "average path length between two nodes = " << avg << endl << endl;

    do {
        g = new Graph(50, 10, 0.4);
    } while (!is_connected(*g));

    total = 0;
    for (int i = 1; i < 50; i++)
        total += dijkstra(*g, 0, i);
    avg = static_cast<float>(total) / 49.0;

    cout << "With 50 nodes, 1-10 weights, and density of 0.4," << endl;
    cout << "average path length between two nodes = " << avg << endl << endl;
}

