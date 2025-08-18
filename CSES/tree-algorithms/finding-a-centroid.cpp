#include <cstdio>
#include <vector>

using namespace std;

/*
Definition of a centroid: The centroid is a node on the tree , if we remove it all the subtree component
will have nodes <= N/2 ceil .
Main idea is calculate the sub tree size of each node , which trying to find the centroid move the direction
where the size of the subtree is greater than > N/2 ceil
remember that the number of subtree component which will have the num_nodes greater than > N/2 will be one
( Verify Chatgpt!)



*/

int dfs(int start_node, int parent, vector<vector<int>> &tree, vector<int> &subtree_size)
{
    subtree_size[start_node] = 1;
    for (auto x : tree[start_node])
    {
        if (x != parent)
        {
            subtree_size[start_node] += dfs(x, start_node, tree, subtree_size);
        }
    }
    return subtree_size[start_node];
}

int centroid(int start_node, int parent, vector<vector<int>> &tree, vector<int> &subtree_size, int num_nodes)
{
    for (auto x : tree[start_node])
    {
        if (x != parent)
        {
            if (subtree_size[x] > (num_nodes / 2))
            {
                return centroid(x, start_node, tree, subtree_size, num_nodes);
            }
        }
    }
    return start_node;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes;
    scanf("%d", &num_nodes);
    vector<vector<int>> tree(num_nodes + 1);
    for (int i = 1; i <= num_nodes - 1; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }
    vector<int> subtree_size(num_nodes + 1);
    dfs(1, 1, tree, subtree_size);
    int x = centroid(1, 1, tree, subtree_size, num_nodes);
    printf("%d", x);
}