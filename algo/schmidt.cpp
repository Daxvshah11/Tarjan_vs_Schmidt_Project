#include <bits/stdc++.h>
#include <chrono>
#include "graphGenerator.cpp"
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

using namespace std;

void DFS(const map<int, unordered_set<int>> &graph, map<int, unordered_set<int>> &directedGraphEdges, map<int, unordered_set<int>> &directedGraphBackEdges, unordered_map<int, int> &visitedTime, vector<int> &visitingOrder, vector<bool> &visited, int node, int parent)
{
    static int time = 0;
    visited[node] = true;
    visitingOrder.push_back(node);
    visitedTime[node] = time++;

    // iterating over all the neighbours
    for (int neighbour : graph.at(node))
    {
        // checking if the neighbour is parent
        if (neighbour == parent)
            continue;

        // checking the visited status of neighbour
        if (visited[neighbour])
        {
            // adding a back edge from the neighbour to the node in directed graph according to visited times
            if (visitedTime.at(neighbour) < visitedTime.at(node)){
                directedGraphBackEdges[neighbour].insert(node);
            }
        }
        else
        {
            // adding a directed edge normally from node to neighbour in directed graph
            directedGraphEdges[neighbour].insert(node);

            // recursive call
            DFS(graph, directedGraphEdges, directedGraphBackEdges, visitedTime, visitingOrder, visited, neighbour, node);
        }
    }
}

/*
*/

void deepMove(map<int, unordered_set<int>> &directedGraphEdges, int node, vector<int> &ear)
{
    if ((directedGraphEdges.find(node) != directedGraphEdges.end()) && (!directedGraphEdges[node].empty())){
        int endPoint = *directedGraphEdges[node].begin();
        directedGraphEdges[node].erase(endPoint);
        ear.push_back(endPoint);
        // recursive call on the end point
        deepMove(directedGraphEdges, endPoint, ear);
    }
}

bool Schmidt_BiconnCheck(map<int, unordered_set<int>> &graph)
{
    // declaring variables
    map<int, unordered_set<int>> directedGraphEdges{};
    map<int, unordered_set<int>> directedGraphBackEdges{};
    unordered_map<int, int> visitedTime{};
    vector<bool> visited(graph.size(), false);
    vector<int> visitingOrder{};

    // calling the DFS function and benchmarking it.
    DFS(graph, directedGraphEdges, directedGraphBackEdges, visitedTime, visitingOrder, visited, 0, -1);

    // iterating over the directed graph again in DFS order
    vector<vector<int>> ears_list;
    int cycle_count = 0;
    int ear_edge_count = 0;
    for (int node : visitingOrder)
    {
        // doing DFS again from this node and removing the edges
        // choosing back edges from this node
        for (int backEdgeEndPoint : directedGraphBackEdges[node])
        {
            // doing a deep move
            vector<int> ear = {node, backEdgeEndPoint};
            deepMove(directedGraphEdges, backEdgeEndPoint, ear);
            ear_edge_count += ear.size() - 1;
            // ears_list.push_back(std::move(ear));
            if (ear.front() == ear.back()) {
                cycle_count++;
                if (cycle_count > 1) {
                    // More than one cycle found, return false
                    return false; 
                }
            }
        }
    }

    int graph_edge_count = 0;
    for (const auto &pair : graph)
    {
        graph_edge_count += pair.second.size();
    }
    graph_edge_count /= 2;

    if (ear_edge_count != graph_edge_count) {
        return false;
    }

    // checking if there is any edge still left in either of directed edge graphs or of back edges
    for (const auto &pair : directedGraphEdges)
    {
        if (pair.second.size() > 0)
            return false;
    }
    // if all edges are removed then graph is biconnected
    return true;
}

/*
*/

// MAIN
int main()
{
    // generating a randomg graph using the function
    map<int, unordered_set<int>> graph = generateRandomGraph();

    // map<int, unordered_set<int>> graph = {
    //     {0, {1, 2}},
    //     {1, {0, 2, 4}},
    //     {2, {0, 1, 3, 5}},
    //     {3, {2, 6}},
    //     {4, {1, 5, 8}},
    //     {5, {2, 4, 6, 7}},
    //     {6, {3, 5, 7, 9}},
    //     {7, {5, 6, 8, 9}},
    //     {8, {4, 7}},
    //     {9, {6, 7}}
    // };

    auto CLOCK_START = chrono::high_resolution_clock::now();
    // check biconnectedness
    bool isBiconnected = Schmidt_BiconnCheck(graph);

    // printing final output
    if (isBiconnected)
        cout << "\nGraph is biconnected" << endl;
    else
        cout << "\nGraph is not biconnected" << endl;

    auto CLOCK_STOP = chrono::high_resolution_clock::now();

    // TIME PRINT
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(CLOCK_STOP - CLOCK_START).count();
    time_taken *= 1e-9;
    cout << "\nTime taken : " << fixed << time_taken << setprecision(9) << " sec" << endl;

    return 0;
}