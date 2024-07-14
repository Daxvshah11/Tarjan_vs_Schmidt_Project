#include <bits/stdc++.h>
#include <map>
using ll = long long;
using namespace std;

map<ll, unordered_set<ll>> generateRandomGraph()
{
    ll numNodes, numEdges;
    cout << "Enter the number of nodes : ";
    cin >> numNodes;
    cout << "Enter the number of edges : ";
    cin >> numEdges;

    srand(time(0));
    map<ll, unordered_set<ll>> graph;

    for (ll i = 0; i < numNodes; i++)
    {
        graph[i] = unordered_set<ll>();
    }

    for (ll i = 0; i < numEdges; i++)
    {
        ll node1 = (rand() % numNodes);
        ll node2 = (rand() % numNodes);

        if (node1 == node2 || graph[node1].find(node2) != graph[node1].end())
        {
            i--;
            continue;
        }

        graph[node1].insert(node2);
        graph[node2].insert(node1);
    }

    return graph;
}

void printGraph(const map<ll, unordered_set<ll>> &graph)
{
    for (const auto &pair : graph)
    {
        cout << "Node " << pair.first << " is connected to: ";
        for (ll node : pair.second)
        {
            cout << node << " ";
        }
        cout << endl;
    }
}

void writeGraphToFile(const map<ll, unordered_set<ll>> &graph, const string &filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    for (const auto &pair : graph)
    {
        outFile << pair.first;
        for (ll node : pair.second)
        {
            outFile << " " << node;
        }
        outFile << "\n";
    }
    outFile.close();
    cout << "Graph successfully written to " << filename << endl;
}

// COMMENTABLE CODE BELOW (only for testing)

// int main()
// {
//     map<int, unordered_set<int>> randomGraph = generateRandomGraph();
//     // printGraph(randomGraph);
//     string filename = "adjacency-list.txt";
//     writeGraphToFile(randomGraph, filename);

//     return 0;
// }
