#include <cstdio>
#include <vector>
using namespace std;

/*


Main idea:
1. In first pass of dfs calculate the total number of nodes in the subtree of the node.
2. In the second pass for a current node calculate the sum contributed by the parent node.






*/

void dfs(vector<vector<int>> &tree, int start_node, int parent, vector<long long> &subtree_size, vector<long long> &subtree_sum)
{
    subtree_size[start_node] = 1;
    subtree_sum[start_node] = 0;
    for (auto x : tree[start_node])
    {
        if (x != parent)
        {
            dfs(tree, x, start_node, subtree_size, subtree_sum);
            subtree_size[start_node] += subtree_size[x];
            subtree_sum[start_node] += (subtree_sum[x] + subtree_size[x]);
        }
    }
}

void dfs2(vector<vector<int>> &tree, int start_node, int parent, vector<long long> &subtree_size, vector<long long> &answer, int n)
{
    for (auto x : tree[start_node])
    {
        if (x != parent)
        {
            // Re-root from start_node to x
            answer[x] = answer[start_node] - subtree_size[x] + (n - subtree_size[x]);
            dfs2(tree, x, start_node, subtree_size, answer, n);
        }
    }
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes;
    scanf("%d", &num_nodes);
    vector<vector<int>> tree(num_nodes + 1);
    vector<long long> subtree_size(num_nodes + 1);
    vector<long long> subtree_sum(num_nodes + 1);
    vector<long long> answer(num_nodes + 1);

    for (int i = 1; i < num_nodes; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }

    // First DFS to calculate subtree sums from root 1
    dfs(tree, 1, 0, subtree_size, subtree_sum);
    answer[1] = subtree_sum[1];

    // Second DFS to re-root and calculate answers for all nodes
    dfs2(tree, 1, 0, subtree_size, answer, num_nodes);

    for (int i = 1; i <= num_nodes; i++)
    {
        printf("%lld ", answer[i]);
    }

    return 0;
}