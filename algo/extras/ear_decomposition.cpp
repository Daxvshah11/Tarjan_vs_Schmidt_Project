#include <chrono>
#include <bits/stdc++.h>
#include "../long-long-algo/graphGenerator-long-long.cpp"

using namespace std;

class Graph
{
public:
    map<int, unordered_set<int>> adjacency_list;
    vector<int> visit_order_index;
    Graph(const map<int, unordered_set<int>> &adj_list) : adjacency_list(adj_list)
    {
        visit_order_index.resize(adj_list.size(), -1);
    }

    void dfs_recursive(int node, vector<bool> &visited, map<int, int> &parent, set<pair<int, int>> &back_edges)
    {
        // Add node to visited nodes list
        static int order = 0;
        visited[node] = true;
        visit_order_index[node] = order++;
        for (int neighbor : adjacency_list[node])
        {
            // Keep track of parent node of the neighbour node and continue with DFS
            if (!visited[neighbor])
            {
                parent[neighbor] = node;
                dfs_recursive(neighbor, visited, parent, back_edges);
                // Find back-edges and create a "directed" edge as per algorithm.
            }
            else if (parent[node] != neighbor)
            {
                pair<int, int> edge = {node, neighbor};
                if (back_edges.find(edge) == back_edges.end())
                    back_edges.insert({neighbor, node});
            }
        }
    }

    /*
    After back-edges have been created, they're sorted using a custom key.
    Sorting is done based on the visited order of the node during DFS.
    Example:
        visit_order: 0 5 6 8 7 3 9 4 2 1
        back-edges before sorting: (3, 4), (7, 2), (9, 2), (6, 2), (7, 1), (4, 1), (8, 3), (6, 3), (0, 8), (0, 6)
        back-edges after sorting: (0, 6), (0, 8), (6, 3), (6, 2), (8, 3), (7, 2), (7, 1), (3, 4), (9, 2), (4, 1)
    */

    // SORTING NOT NEEDED since using SET for back_edges
    // void sort_back_edges(vector<pair<int, int>> &back_edges)
    // {
    //     // Sort back_edges using the precomputed visit_order_index for fast lookup
    //     sort(back_edges.begin(), back_edges.end(), [this](const pair<int, int> &a, const pair<int, int> &b)
    //          { return (visit_order_index[a.first] < visit_order_index[b.first] || (visit_order_index[a.first] == visit_order_index[b.first] && visit_order_index[a.second] < visit_order_index[b.second])); });
    // }
};

class GraphAnalysis
{
public:
    static vector<vector<int>> ear_decomposition(const map<int, int> &parents, const vector<pair<int, int>> &back_edges)
    {
        vector<vector<int>> ears_list;
        set<int> visited_nodes;

        for (const auto &edge : back_edges)
        {
            int node = edge.first, neighbour = edge.second;
            vector<int> ear = {node, neighbour};
            visited_nodes.insert(node);
            if (visited_nodes.find(neighbour) != visited_nodes.end())
            {
                ears_list.push_back(ear);
                continue;
            }
            visited_nodes.insert(neighbour);
            int parent_node = parents.at(neighbour);
            while (parent_node && visited_nodes.find(parent_node) == visited_nodes.end())
            {
                ear.push_back(parent_node);
                visited_nodes.insert(parent_node);
                parent_node = parents.at(parent_node);
            }
            ear.push_back(parent_node);
            ears_list.push_back(ear);
        }
        return ears_list;
    }

    static bool test_for_biconnectedness(const map<int, unordered_set<int>> &graph, const vector<vector<int>> &ears)
    {
        int edge_count = 0;
        for (const auto &ear : ears)
        {
            edge_count += ear.size() - 1;
        }
        int graph_edge_count = 0;
        for (const auto &pair : graph)
        {
            graph_edge_count += pair.second.size();
        }
        graph_edge_count /= 2;

        bool first_is_cyclic = !ears.empty() && (ears.front().front() == ears.front().back());
        bool others_are_not_cyclic = all_of(ears.begin() + 1, ears.end(), [](const vector<int> &ear)
                                            { return ear.front() != ear.back(); });

        return edge_count == graph_edge_count && first_is_cyclic && others_are_not_cyclic;
    }
};

int main()
{
    // map<int, unordered_set<int>> graph_data = {
    //     {0, {6, 8, 5}},
    //     {1, {2, 4, 7}},
    //     {2, {1, 6, 9, 4, 7}},
    //     {3, {4, 6, 7, 8, 9}},
    //     {4, {1, 2, 3, 9}},
    //     {5, {0, 6}},
    //     {6, {0, 2, 3, 5, 8}},
    //     {7, {1, 2, 3, 8}},
    //     {8, {0, 3, 6, 7}},
    //     {9, {2, 3, 4}}
    // };

    map<int, unordered_set<int>> graph_data = generateRandomGraph();

    Graph graph(graph_data);
    vector<bool> visited(graph_data.size(), false);
    // vector<int> visited;
    map<int, int> parents;
    set<pair<int, int>> back_edge;
    int root = 0;

    auto CLOCK_START = chrono::high_resolution_clock::now();

    graph.dfs_recursive(root, visited, parents, back_edge);
    vector<pair<int, int>> back_edges;
    for (auto it = back_edge.begin(); it != back_edge.end(); it++)
        back_edges.push_back(*it);

    // auto INTERMEDIATE = chrono::high_resolution_clock::now();
    // double intermediate_time_taken = chrono::duration_cast<chrono::nanoseconds>(INTERMEDIATE - CLOCK_START).count();
    // intermediate_time_taken *= 1e-9;
    // cout << "\nTime taken in DFS : " << fixed << intermediate_time_taken << setprecision(9) << " sec" << endl;

    // graph.sort_back_edges(back_edges);

    // auto INTERMEDIATE2 = chrono::high_resolution_clock::now();
    // intermediate_time_taken = chrono::duration_cast<chrono::nanoseconds>(INTERMEDIATE2 - INTERMEDIATE).count();
    // intermediate_time_taken *= 1e-9;
    // cout << "\nTime taken in SORT : " << fixed << intermediate_time_taken << setprecision(9) << " sec" << endl;

    if (graph_data.size() == count(visited.begin(), visited.end(), true))
    {
        auto ears = GraphAnalysis::ear_decomposition(parents, back_edges);

        // cout << "Ears: " << endl;
        // for (const auto& ear : ears) {
        //     for (int node : ear) {
        //         cout << node << " ";
        //     }
        //     cout << endl;
        // }

        bool is_biconnected = GraphAnalysis::test_for_biconnectedness(graph_data, ears);
        cout << "Graph is " << (is_biconnected ? "biconnected" : "not biconnected") << endl;
    }
    else
    {
        cout << "Graph is not biconnected" << endl;
    }
    auto CLOCK_STOP = chrono::high_resolution_clock::now();
    double full_time_taken = chrono::duration_cast<chrono::nanoseconds>(CLOCK_STOP - CLOCK_START).count();
    full_time_taken *= 1e-9;
    cout << "Time taken in checking and all final : " << fixed << full_time_taken << setprecision(9) << " sec" << endl;

    return 0;
}
