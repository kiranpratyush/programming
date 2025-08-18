#include <cstdio>
#include <vector>
#include <climits>
using namespace std;

typedef struct weighted_node
{
    int destination_node;
    int weight;
} weighted_node;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, num_edges;
    scanf("%d %d", &num_nodes, &num_edges);
    int num_subsets = (1 << num_nodes);
    vector<vector<weighted_node>> graph(num_nodes + 1);
    vector<vector<int>> dp(num_nodes + 1, vector<int>(num_subsets + 1, INT_MAX));
    for (int i = 0; i < num_edges; i++)
    {
        int start_node, end_node, weight;
        scanf("%d %d %d", &start_node, &end_node, &weight);
        weighted_node node1 = {end_node - 1, weight};
        weighted_node node2 = {start_node - 1, weight};
        graph[start_node - 1].push_back(node1);
        graph[end_node - 1].push_back(node2);
    }
    // fill the only one node
    for (int i = 0; i < num_nodes; i++)
    {
        dp[i][1 << i] = 0;
    }

    // go for each subsets
    for (int subset = 1; subset < num_subsets; subset++)
    {
        // iterate over all the nodes
        for (int node = 0; node < num_nodes; node++)
        {
            // check if the node is present in the current subset
            if ((subset & (1 << node)) && dp[node][subset] != INT_MAX)
            {
                for (auto x : graph[node])
                {
                    int weight = x.weight;
                    int destination_node = x.destination_node;
                    // if the destination node not in the existing subset
                    if (!(subset & (1 << destination_node)))
                    { // add the new value into the subset
                        int new_subset = (subset | (1 << destination_node));
                        dp[destination_node][new_subset] = min(dp[destination_node][new_subset], dp[node][subset] + weight);
                    }
                }
            }
        }
    }
    // now calculate the min_cost

    int min_cost = INT_MAX;
    for (int i = 0; i < num_nodes; i++)
    {
        min_cost = min(min_cost, dp[i][(1 << num_nodes) - 1]);
    }
    printf("%d", min_cost);
}