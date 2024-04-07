#include <bits/stdc++.h>

using namespace std;

//  min + (rand() % (int)(max - min + 1))
// above is the formula for generating between min and max

// this is the function that generates a random graph
map<int, unordered_set<int>> generateRandomGraph(int numNodes, int numEdges)
{
    // initialising the graph
    map<int, unordered_set<int>> graph;

    // adding all the nodes pre-hand with empty sets initialised
    for (int i = 1; i <= numNodes; i++)
    {
        graph[i] = unordered_set<int>();
    }

    // this part is to make each and every chosen edge by randomly choosing 2 nodes
    for (int i = 1; i <= numEdges; i++)
    {
        // choosing random 2 nodes
        int node1 = 1 + (rand() % numNodes);
        int node2 = 1 + (rand() % numNodes);

        // explicitly checking for self loops just in case
        if (node1 == node2)
        {
            // ignoring this edge generation
            i--;
            continue;
        }

        // checking if the selected edge didn't already exist
        if (graph[node1].find(node2) != graph[node1].end())
        {
            // ignoring this edge generation
            i--;
            continue;
        }

        // otherwise, finally adding the random edge to the graph
        graph[node1].insert(node2);
        graph[node2].insert(node1);
    }

    // returning the graph
    return graph;
}

// this function is just for our purpose to check if random generation is working fine
void printGraph(const map<int, unordered_set<int>> &graph)
{
    for (const auto &pair : graph)
    {
        cout << "Node " << pair.first << " is connected to: ";
        for (int node : pair.second)
        {
            cout << node << " ";
        }
        cout << endl;
    }
}

// MAIN
int main()
{
    // seeding the random number generator
    srand(time(0));

    // just checking working here
    int numNodes, numEdges;
    cout << "Enter the number of nodes : ";
    cin >> numNodes;
    cout << "Enter the number of edges : ";
    cin >> numEdges;

    // intialise and get random
    map<int, unordered_set<int>> randomGraph = generateRandomGraph(numNodes, numEdges);

    // print check
    printGraph(randomGraph);

    return 0;
}