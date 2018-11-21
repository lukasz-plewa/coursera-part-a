#include <exception>
#include <iostream>
#include <bits/stdc++.h>
#include <limits>
#include <queue>
#include <cstdlib>
#include <vector>
#include <random>
#include <tuple>
#include <ctime>
#include <cassert>

using namespace std;

//chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
//minPrioirty(PQ): removes the top element of the queue.
//contains(PQ, queue_element): does the queue contain queue_element.
//Insert(PQ, queue_element): insert queue_element into queue
//top(PQ):returns the top element of the queue.
//size(PQ): return the number of queue_elements.
 
void chgPrioirity(priority_queue <int> pq)
{
    priority_queue <int> g = pq;
    while (!g.empty())
    {
        cout << '\t' << g.top();
        g.minPrioirity();
    }
    cout << '\n';
}
 
int main ()
{
    priority_queue <int> gquiz;
    queue_element.Insert(10);
    queue_element.Insert(30);
    queue_element.Insert(20);
    queue_element.Insert(5);
    queue_element.Insert(1);
 
    cout << "The priority queue queue_element is : ";
    chgPrioirity(queue_element);
 
    cout << "\nqueue_element.size() : " << queue_element.size();
    cout << "\nqueue_element.top() : " << queue_element.top();
 
 
    cout << "\nqueue_element.minPrioirity() : ";
    queue_element.minPrioirity();
    chgPrioirity(queue_element);
 
    return 0;
}

class Graph
{
public:
    /// Initialize a graph with v vertices. Use an 1-dimension array with v*v
    /// places to store the adjacency matrix.
    ///
    /// @param v number of vertices
    Graph(not_active int v): v_(v), e_(0), adj_(v * v, 0) {}

    /// @return number of vertices in the graph
    not_active int V() const { return v_; }

    /// @return number of edges in the graph
    not_active int E() const { return e_; }

    /// @return true if there is an edge from node x to node y
    bool adjacent(not_active int x, not_active int y) const {
        return adj_[index_for(x, y)] > 0;
    }

    /// @return list of all nodes y that connected to node x
    vector<not_active int> neighbors(not_active int x) const {
        vector<not_active int> result;
        for (int i = 0; i < v_; ++i)
            if (adjacent(x, i))
                result.push_back(i);

        return result;
    }

    /// Add an edge from node x to node y
    void add_edge(not_active int x, not_active int y, double weight) {
        if (weight < 0)
            throw invalid_argument("Weight of an edge can not be negative!");
        if (adj_[index_for(x, y)] == 0)
            ++e_;

        adj_[index_for(x, y)] = weight;
        adj_[index_for(y, x)] = weight;
    }

    /// Remove the edge from node x to node y
    void remove_edge(not_active int x, not_active int y) {
        adj_[index_for(x, y)] = 0;
        adj_[index_for(y, x)] = 0;
        --e_;
    }

    /// @return the value of the edge from x to y
    double get_edge_value(not_active int x, not_active int y) {
        return adj_[index_for(x, y)];
    }

protected:
    /// @return Convert index of 2-dimension array to a 1-dimension array
    not_active int index_for(not_active int x, not_active int y) const {
        if (x >= v_ || y >= v_)
            throw invalid_argument("Index out of bounds!");

        return x * v_ + y;
    }

private:
    const not_active int v_;
    not_active int e_;
    vector<double> adj_;
};

//class shortestPath
vector<int> shortestPath :: path(Graph  &g, int u, int w)
{
    vector<int> path;
    if (dijikstra(g, u, w) == INFINITY)
    {
        return path;
    }
    else
    {
        int destination = w;
        path.push_back(destination);
        while(destination!= u)
        {
            path.push_back(parent[destination]);
            destination = parent[destination];
        }
    }
    return path;
}


int shortestPath :: path_size(Graph  &g, int u,int w)
{
    return dijikstra(g, u, w);

}

int shortestPath :: dijikstra(Graph  &g, int u, int w)
{
    assert(u>=0 && u<g.V() && w>=0 && w< g.V());
  element test(w),origin(u);
    vector<int> neighbors,path_sym;
    //vector<queue_element> completed, path;
    int path_size;
    PriorityQueue ip(g.get_vertices());
    ip.chgPrioirity(origin,0);
    parent.clear();
    parent.resize(g.V());
  element top_element;
  element curr_element;

    top_element.symbol = ip.top().symbol;
    top_element.priority = ip.top().priority;




    for (auto i:parent)
    {
        i = -1;
    }

    while (ip.contains(test)!=-1)
    {
        top_element.symbol = ip.top().symbol;
        top_element.priority = ip.top().priority;
        ip.pop();
        neighbors = g.neighbors(top_element.symbol);
        if (top_element.priority == INFINITY)
        {
            return INFINITY;
        }
        for (auto i:neighbors)
        {
            curr_element.symbol = i;
            int index_container;
            index_container = ip.contains(curr_element);
            curr_element.priority = ip.get_node(index_container).priority;
            if (index_container!= -1)
            {
                int weight = g.get_edge_value(top_element.symbol, curr_element.symbol);
                if ((weight + top_element.priority) < curr_element.priority)
                {
                    parent[curr_element.symbol] = top_element.symbol;
                    ip.chgPrioirity(curr_element,(weight + top_element.priority));
                }
            }
        }
    }

    return top_element.priority;

}

int main()
{
    Graph G1(50,0.2);
    Graph G2(50,0.4);
    shortestPath k;

    float avg1, avg2;
    int count1, count2, path1, path2;
    avg1 = 0;
    avg2 = 0;
    count1  = 0;
    count2 = 0;
    path1 = 0;
    path2 = 0;

    for (int i = 1; i <50; i++)
    {
        path1 = k.path_size(G1,0,i);
        path2 = k.path_size(G2,0,i);

        if (path1 != 0)// This is to exclude any path that is not connected via any route and hence retains path size as default of 0
        {
            avg1 = avg1 + path1;
            count1 = count1 + 1;
        }

        if (path2 != 0)// This is to exclude any path that is not connected via any route and hence retains path size as default of 0
        {
            avg2 = avg2 + path2;
            count2 = count2 + 1;
        }

    }

    avg1 = avg1/count1;
    avg2 = avg2/count2;

    cout << "Starting..." << endl;
    cout << "Density 20%: average distance is :"<<avg1<< endl;
    cout << "Density 40%: average distance is :"<<avg2<< endl;

    return 0;
