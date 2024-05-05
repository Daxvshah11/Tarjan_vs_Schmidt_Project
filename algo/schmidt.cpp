#include <bits/stdc++.h>
#include <chrono>
#include "graphGenerator.cpp"

using namespace std;

void DFS(const map<int, unordered_set<int>> &graph, map<int, unordered_set<int>> &directedGraphEdges, map<int, unordered_set<int>> &directedGraphBackEdges, vector<int> &visitingOrder, vector<bool> &visited, int node, int parent)
{
    visited[node] = true;
    visitingOrder.push_back(node);

    // iterating over all the neighbours
    for (int neighbour : graph.at(node))
    {
        // checking if the neighbour is parent
        if (neighbour == parent)
            continue;

        // checking the visited status of neighbour
        if (visited[neighbour])
        {
            // if its visited then back edge case

            // adding a back edge from the neighbour to the node in directed graph

            auto it_node = find(visitingOrder.begin(), visitingOrder.end(), node);
            auto it_neighbour = find(visitingOrder.begin(), visitingOrder.end(), neighbour);
            if (it_neighbour < it_node)
            {
                // Adding a back edge from the neighbour to the node in directed graph
                directedGraphBackEdges[neighbour].insert(node);
            }

            // directedGraphBackEdges[neighbour].insert(node);
        }
        else
        {
            // recursive call case

            // adding a directed edge normally from node to neighbour in directed graph
            directedGraphEdges[neighbour].insert(node);

            // recursive call
            DFS(graph, directedGraphEdges, directedGraphBackEdges, visitingOrder, visited, neighbour, node);
        }
    }
}

/*
*/

void deepMove(map<int, unordered_set<int>> &directedGraphEdges, int node)
{
    auto edgesCopy = directedGraphEdges[node];
    // iterating over possible end points from node
    for (int endPoint : edgesCopy)
    {
        // removing the edge
        directedGraphEdges[node].erase(endPoint);

        // printing that edge
        cout << "deepmove" << node << "-" << endPoint << endl;

        // recursive call on the end point
        deepMove(directedGraphEdges, endPoint);
    }
}

bool Schmidt_BiconnCheck(map<int, unordered_set<int>> &graph)
{
    // declaring variables
    map<int, unordered_set<int>> directedGraphEdges{};
    map<int, unordered_set<int>> directedGraphBackEdges{};
    vector<bool> visited(graph.size(), false);
    vector<int> visitingOrder{};

    // calling the DFS function
    DFS(graph, directedGraphEdges, directedGraphBackEdges, visitingOrder, visited, 0, -1);

    for (auto value : visitingOrder) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    for (const auto& pair : directedGraphEdges) {
        std::cout << "directedGraphEdges Key: " << pair.first << ", Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    for (const auto& pair : directedGraphBackEdges) {
        std::cout << "directedGraphBackEdges Key: " << pair.first << ", Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    // iterating over the directed graph again in DFS order
    for (int node : visitingOrder)
    {
        // doing DFS again from this node and removing the edges
        // choosing back edges from this node
        for (int backEdgeEndPoint : directedGraphBackEdges[node])
        {
            // removing the back edge
            directedGraphEdges[node].erase(backEdgeEndPoint);

            // printing this back edge
            cout << "schmidt-biconn" << node << "-" << backEdgeEndPoint << endl;

            // doing a deep move
            deepMove(directedGraphEdges, backEdgeEndPoint);

            // moving to new line
            cout << endl;
        }
    }

    for (const auto& pair : directedGraphEdges) {
        std::cout << "leftover directedGraphEdges Key: " << pair.first << ", Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    for (const auto& pair : directedGraphBackEdges) {
        std::cout << "leftover directedGraphBackEdges Key: " << pair.first << ", Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    // checking if there is any edge still left in either of directed edge graphs or of back edges
    for (const auto &pair : directedGraphEdges)
    {
        if (pair.second.size() > 0)
            return false;
    }

    for (const auto &pair : directedGraphBackEdges)
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
    // map<int, unordered_set<int>> graph = generateRandomGraph();
    map<int, unordered_set<int>> graph = {
        {0, {1, 2}},
        {1, {0, 2, 4}},
        {2, {0, 1, 3, 5}},
        {3, {2, 6}},
        {4, {1, 5, 8}},
        {5, {2, 4, 6, 7}},
        {6, {3, 5, 7, 9}},
        {7, {5, 6, 8, 9}},
        {8, {4, 7}},
        {9, {6, 7}}
    };

    // check biconnectedness
    bool isBiconnected = Schmidt_BiconnCheck(graph);

    // printing final output
    if (isBiconnected)
        cout << "\nGraph is biconnected" << endl;
    else
        cout << "\nGraph is not biconnected" << endl;

    return 0;
}