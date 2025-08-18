#include <cstdio>
#include <vector>
using namespace std;
vector<int> tree[200001];
bool processed[200001];
int subtree_size[200001];
vector<int> depth_count(200001, 0);

int getsubtree_size(int node, int parent)
{
    subtree_size[node] = 1;
    for (auto x : tree[node])
    {
        if (x != parent && !processed[x])
        {
            subtree_size[node] += getsubtree_size(x, node);
        }
    }
    return subtree_size[node];
}

int get_centroid(int node, int parent, int allowed_subtreesize)
{
    for (auto x : tree[node])
    {
        if (x != parent && !processed[x] && subtree_size[x] >= allowed_subtreesize)
        {
            return get_centroid(x, node, allowed_subtreesize);
        }
    }
    return node;
}

void dfs(int node, int parent, int &max_depth, bool is_fill, int k, long long &ans, int depth)
{
    if (depth > k)
    {
        return;
    }
    max_depth = max(max_depth, depth);
    if (is_fill)
    {
        depth_count[depth]++;
    }
    else
    {
        ans += depth_count[k - depth];
    }
    for (auto x : tree[node])
    {
        if (x != parent && !processed[x])
        {
            // Pass the new vector to the recursive calls
            dfs(x, node, max_depth, is_fill, k, ans, depth + 1);
        }
    }
}
void centroid_decomp(int node, int parent, long long &ans, int k)
{
    int treesize = getsubtree_size(node, parent);
    int centroid = get_centroid(node, parent, treesize >> 1);
    processed[centroid] = true;

    int max_depth = 0;
    depth_count[0] = 1;

    for (auto x : tree[centroid])
    {
        if (!processed[x])
        {
            dfs(x, centroid, max_depth, false, k, ans, 1);

            // The 'fill' DFS will populate our modified_depths vector
            dfs(x, centroid, max_depth, true, k, ans, 1);
        }
    }

    // --- EFFICIENT RESET ---
    // Instead of a loop or fill, only reset the indices we actually touched.
    // This is much faster if the number of unique depths is smaller than max_depth.
    for (int i = 0; i <= max_depth; i++)
    {
        depth_count[i] = 0;
    }
    depth_count[0] = 1;

    for (auto x : tree[centroid])
    {
        if (!processed[x])
        {
            centroid_decomp(x, centroid, ans, k);
        }
    }
}
int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, path_length;
    scanf("%d %d", &num_nodes, &path_length);
    for (int i = 1; i < num_nodes; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }
    long long ans = 0;
    centroid_decomp(1, 1, ans, path_length);
    printf("%lld", ans);
    return 0;
}