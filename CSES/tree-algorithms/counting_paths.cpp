#include <cstdio>
#include <vector>
#include <cmath> // ✅ Required for log2 and ceil
using namespace std;

int timer = 0;

void preprocess(vector<vector<int>> &tree, vector<int> &in, vector<int> &out, vector<vector<int>> &up, int node, int parent, int l)
{
    in[node] = ++timer;
    up[node][0] = parent;
    for (int i = 1; i <= l; i++)
    {
        up[node][i] = up[up[node][i - 1]][i - 1];
    }
    for (auto x : tree[node])
    {
        if (x != parent) // ✅ Corrected from x != node
        {
            preprocess(tree, in, out, up, x, node, l);
        }
    }
    out[node] = ++timer;
}

bool is_ancestor(vector<int> &in, vector<int> &out, int u, int v)
{
    return in[u] <= in[v] && out[u] >= out[v];
}

int lca(int u, int v, vector<vector<int>> &up, vector<int> &in, vector<int> &out, int l)
{
    if (is_ancestor(in, out, u, v))
        return u;
    if (is_ancestor(in, out, v, u))
        return v;
    for (int i = l; i >= 0; i--)
    {
        if (!is_ancestor(in, out, up[u][i], v))
        {
            u = up[u][i];
        }
    }
    return up[u][0];
}

int dfs(vector<vector<int>> &tree, vector<int> &result, int node, int parent)
{
    int value = 0;
    for (auto x : tree[node])
    {
        if (x != parent)
        {
            value += dfs(tree, result, x, node);
        }
    }
    result[node] += value;
    return result[node]; // ✅ Fix: return value
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, num_paths;
    scanf("%d %d", &num_nodes, &num_paths);
    vector<vector<int>> tree(num_nodes);
    int num_edges = num_nodes - 1;
    while (num_edges-- > 0)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node - 1].push_back(end_node - 1);
        tree[end_node - 1].push_back(start_node - 1);
    }

    int l = 0;
    while ((1 << l) <= num_nodes)
        l++; // ✅ Better way to compute log2(n)

    vector<vector<int>> up(num_nodes, vector<int>(l + 1));
    vector<int> in(num_nodes);
    vector<int> out(num_nodes);
    preprocess(tree, in, out, up, 0, 0, l);

    vector<int> result(num_nodes, 0);

    while (num_paths-- > 0) // ✅ Fix: decrement in loop condition
    {
        int start_path, end_path;
        scanf("%d %d", &start_path, &end_path);
        start_path--;
        end_path--;

        result[start_path] += 1;
        result[end_path] += 1;
        int lca_result = lca(start_path, end_path, up, in, out, l);
        result[lca_result] -= 1;

        if (lca_result != 0) // ✅ Don't subtract parent of root
            result[up[lca_result][0]] -= 1;
    }

    dfs(tree, result, 0, 0);
    for (auto x : result)
    {
        printf("%d ", x);
    }
}
