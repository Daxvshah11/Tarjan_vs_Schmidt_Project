#include <bits/stdc++.h>
#include <chrono>
#include "graphGenerator.cpp"

using namespace std;

// function for performing Tarjan's DFS
void DFS(const map<int, unordered_set<int>> &graph, vector<bool> &visited, int node, vector<int> &discoveryTime, vector<int> &low, vector<int> &parent, int time, vector<int> &articulationPoints)
{
    // setting visited of this node to True
    visited[node] = true;

    // setting discovery time
    discoveryTime[node] = time;

    // setting low node value
    low[node] = time;

    // initialising children count
    int children = 0;

    // iterating over all the neighbours of node
    for (int neighbour : graph.at(node))
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
bool Tarjan_BiconnCheck(map<int, unordered_set<int>> &graph)
{
    // initialising bool
    bool isBiconnected{true};

    // intialising all variables for the DFS call
    vector<int> articulationPoints{};
    vector<bool> visited(graph.size() + 1, false);
    vector<int> discoveryTime(graph.size() + 1, -1);
    vector<int> low(graph.size() + 1, -1);
    vector<int> parent(graph.size() + 1, -1);
    int time = 0;
    int root = 0;
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
    // generating a randomg graph using the function from header file
    map<int, unordered_set<int>> graph = generateRandomGraph();

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