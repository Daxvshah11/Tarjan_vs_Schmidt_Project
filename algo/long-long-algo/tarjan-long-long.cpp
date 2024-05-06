#include <bits/stdc++.h>
#include <chrono>
#include "graphGenerator-long-long.cpp"
using ll = long long;
using namespace std;

// function for performing Tarjan's DFS
void DFS(const map<ll, unordered_set<ll>> &graph, vector<bool> &visited, ll node, vector<ll> &discoveryTime, vector<ll> &low, vector<ll> &parent, ll time, vector<ll> &articulationPoints)
{
    // setting visited of this node to True
    visited[node] = true;

    // setting discovery time
    discoveryTime[node] = time;

    // setting low node value
    low[node] = time;

    // initialising children count
    ll children = 0;

    // iterating over all the neighbours of node
    for (ll neighbour : graph.at(node))
    {
        // checking if the neighbour is visited or not
        if (!visited[neighbour])
        {
            // setting neighbour's parent
            parent[neighbour] = node;

            // adding a children count
            children++;

            // recursive DFS call on the neighbour
            DFS(graph, visited, neighbour, discoveryTime, low, parent, time + 1, articulationPoints);

            // updating low value for node
            low[node] = min(low[node], low[neighbour]);

            // checking if the node is an articulation point
            if (parent[node] == -1 && children > 1)
            {
                // this is if node is root and also an articulation point
                articulationPoints.push_back(node);
            }
            else if (parent[node] != -1 && low[neighbour] >= discoveryTime[node])
            {
                // this is if node is not root and is an articulation point
                articulationPoints.push_back(node);
            }
        }

        else if (neighbour != parent[node])
        {
            // updating low value for node
            low[node] = min(low[node], discoveryTime[neighbour]);
        }
    }

    // returning after all neighbours done
    return;
}

/*
*/

// function for performing Tarjan's algo
bool Tarjan_BiconnCheck(map<ll, unordered_set<ll>> &graph)
{
    // initialising bool
    bool isBiconnected{true};

    // intialising all variables for the DFS call
    vector<ll> articulationPoints{};
    vector<bool> visited(graph.size() + 1, false);
    vector<ll> discoveryTime(graph.size() + 1, -1);
    vector<ll> low(graph.size() + 1, -1);
    vector<ll> parent(graph.size() + 1, -1);
    ll time = 0;
    ll root = 0;
    // START TIMER
    auto CLOCK_START = chrono::high_resolution_clock::now();

    DFS(graph, visited, root, discoveryTime, low, parent, time, articulationPoints);
    // Check for graph connectedness
    if (graph.size() != visited.size() - 1) 
    {
        // this means the graph is DISCONNECTED in the first place
        isBiconnected = false;
    }

    // checking if articulation points are found
    if (articulationPoints.size() != 0)
    {
        // this means the graph is NOT BICONNECTED
        isBiconnected = false;
    }

    // STOP TIMER
    auto CLOCK_STOP = chrono::high_resolution_clock::now();

    // TIME PRINT
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(CLOCK_STOP - CLOCK_START).count();
    time_taken *= 1e-9;
    cout << "\nTime taken : " << fixed << time_taken << setprecision(9) << " sec" << endl;

    // returning the articulation points
    return isBiconnected;
}

/*
*/

// MAIN
int main()
{
    // generating a randomg graph using the function
    map<ll, unordered_set<ll>> graph = generateRandomGraph(); 
    // map<ll, unordered_set<ll>> graph = {
    //         {0, {1, 2}},
    //         {1, {0, 2}},
    //         {2, {0, 1, 3, 4}},
    //         {3, {2, 4}},
    //         {4, {2, 3}}
    //     };
    // printing the graph (COMMENT THIS FOR LARGE DATASET INPUTS)
    // printGraph(graph);

    // finding articulation points
    bool isBiconnected = Tarjan_BiconnCheck(graph);

    // printing final output
    if (isBiconnected)
        cout << "\nGraph is biconnected" << endl;
    else
        cout << "\nGraph is not biconnected" << endl;
}