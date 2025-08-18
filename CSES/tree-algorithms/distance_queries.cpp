#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

int timer = 0;

void calc_distance(vector<vector<int>> &tree, vector<int> &dist, int node, int parent, int distance)
{
    dist[node] = distance;
    for (auto x : tree[node])
    {
        if (x != parent)
        {
            calc_distance(tree, dist, x, node, distance + 1);
        }
    }
}
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
        if (x != parent)
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
int lca(vector<int> &in, vector<int> &out, vector<vector<int>> &up, int u, int v, int l)
{
    if (is_ancestor(in, out, u, v))
    {
        return u;
    }
    if (is_ancestor(in, out, v, u))
    {
        return v;
    }
    for (int i = l; i >= 0; i--)
    {
        if (!is_ancestor(in, out, up[u][i], v))
        {
            u = up[u][i];
        }
    }
    return up[u][0];
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, num_queries;
    scanf("%d %d", &num_nodes, &num_queries);
    vector<vector<int>> tree(num_nodes);
    int num_edges = num_nodes - 1;
    while (num_edges > 0)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node - 1].push_back(end_node - 1);
        tree[end_node - 1].push_back(start_node - 1);
        num_edges--;
    }
    vector<int> in(num_nodes);
    vector<int> out(num_nodes);
    int l = ceil(log2(num_nodes));
    vector<vector<int>> up(num_nodes, vector<int>(l + 1));
    vector<int> dist(num_nodes);
    calc_distance(tree, dist, 0, 0, 0);
    preprocess(tree, in, out, up, 0, 0, l);
    vector<int> distance_array;
    while (num_queries > 0)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        u -= 1;
        v -= 1;
        int find_lca = lca(in, out, up, u, v, l);
        int distance = dist[u] + dist[v] - 2 * dist[find_lca];
        distance_array.push_back(distance);
        num_queries--;
    }
    for (auto x : distance_array)
    {
        printf("%d\n", x);
    }
}
