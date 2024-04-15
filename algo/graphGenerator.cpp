#include <bits/stdc++.h>
#include <fstream>
#include <unordered_set>
#include <map>

using namespace std;

map<int, unordered_set<int>> generateRandomGraph()
{
    int numNodes, numEdges;
    cout << "Enter the number of nodes : ";
    cin >> numNodes;
    cout << "Enter the number of edges : ";
    cin >> numEdges;

    srand(time(0));
    map<int, unordered_set<int>> graph;

    for (int i = 0; i < numNodes; i++)
    {
        graph[i] = unordered_set<int>();
    }

    for (int i = 0; i < numEdges; i++)
    {
        int node1 = (rand() % numNodes);
        int node2 = (rand() % numNodes);

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

void printGraph(const map<int, unordered_set<int>>& graph)
{
    for (const auto& pair : graph)
    {
        cout << "Node " << pair.first << " is connected to: ";
        for (int node : pair.second)
        {
            cout << node << " ";
        }
        cout << endl;
    }
}

void writeGraphToFile(const map<int, unordered_set<int>>& graph, const string& filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    for (const auto& pair : graph)
    {
        outFile << pair.first;
        for (int node : pair.second)
        {
            outFile << " " << node;
        }
        outFile << "\n";
    }
    outFile.close();
    cout << "Graph successfully written to " << filename << endl;
}

int main()
{
    map<int, unordered_set<int>> randomGraph = generateRandomGraph();
    // printGraph(randomGraph);
    string filename = "adjacency-list.txt";
    writeGraphToFile(randomGraph, filename);

    return 0;
}
