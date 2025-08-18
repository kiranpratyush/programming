#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

int timer = 0;

void dfs(int node, int parent, vector<vector<int>> &tree, vector<vector<int>> &binary_lifting, vector<int> &in, vector<int> &out, int l)
{
    in[node] = ++timer;
    binary_lifting[node][0] = parent;
    for (int i = 1; i <= l; i++)
    {
        binary_lifting[node][i] = binary_lifting[binary_lifting[node][i - 1]][i - 1];
    }
    for (auto x : tree[node])
    {
        if (x != parent)
        {
            dfs(x, node, tree, binary_lifting, in, out, l);
        }
    }
    out[node] = ++timer;
}
bool is_ancestor(int u, int v, vector<int> &in, vector<int> &out)
{
    return in[u] <= in[v] && out[u] >= out[v];
}

int query(int u, int v, vector<vector<int>> &binary_lifting, vector<int> &in, vector<int> &out, int l)
{
    if (is_ancestor(u, v, in, out))
    {
        return u;
    }
    if (is_ancestor(v, u, in, out))
    {
        return v;
    }
    for (int i = l; i >= 0; i--)
    {
        if (!is_ancestor(binary_lifting[u][i], v, in, out))
        {
            u = binary_lifting[u][i];
        }
    }
    return binary_lifting[u][0];
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, num_queries;
    scanf("%d %d", &num_nodes, &num_queries);
    vector<vector<int>> tree(num_nodes + 1);
    for (int i = 2; i <= num_nodes; i++)
    {
        int parent;
        scanf("%d", &parent);
        tree[parent].push_back(i);
    }
    int l = ceil(log2(num_nodes));
    vector<vector<int>> binary_lifting(num_nodes + 1, vector<int>(l + 1));
    vector<int> in(num_nodes + 1);
    vector<int> out(num_nodes + 1);
    vector<int> result;
    dfs(1, 1, tree, binary_lifting, in, out, l);
    while (num_queries > 0)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        int r = query(u, v, binary_lifting, in, out, l);
        result.push_back(r);
        num_queries--;
    }
    for (auto x : result)
    {
        printf("%d \n", x);
    }
}