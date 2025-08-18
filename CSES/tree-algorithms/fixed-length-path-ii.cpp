#include <array>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

struct FenwickTree
{
    static const int N = 200000;
    array<int, N + 1> tree;
    void add(int k, int d)
    {
        for (int i = k + 1; i <= N; i += i & -i)
        {
            tree[i] += d;
        }
    }
    int sum(int k)
    {
        int s = 0;
        for (int i = k; i > 0; i -= i & -i)
        {
            s += tree[i];
        }
        return s;
    }
    int sum(int l, int r) { return sum(r + 1) - sum(l); }
} tree;

const int N = 200001;
bool deleted[N];
int n, k1, k2, sizes[N];
vector<int> g[N];
ll answer;

void dfs_sizes(int node, int parent)
{
    sizes[node] = 1;
    for (int child : g[node])
    {
        if (child == parent || deleted[child])
            continue;
        dfs_sizes(child, node);
        sizes[node] += sizes[child];
    }
}

int find_centroid(int node, int parent, int n)
{
    for (int child : g[node])
    {
        if (child != parent && !deleted[child] && sizes[child] > n / 2)
        {
            return find_centroid(child, node, n);
        }
    }
    return node;
}

void dfs_add(int node, int parent, int depth, int change)
{
    tree.add(depth, change);
    for (int child : g[node])
    {
        if (child == parent || deleted[child])
            continue;
        dfs_add(child, node, depth + 1, change);
    }
}

void dfs_query(int node, int parent, int depth)
{
    answer += tree.sum(k1 - depth, k2 - depth);
    for (int child : g[node])
    {
        if (child == parent || deleted[child])
            continue;
        dfs_query(child, node, depth + 1);
    }
}

void recurse(int node)
{
    dfs_sizes(node, -1);
    int r = find_centroid(node, -1, sizes[node]);
    tree.add(0, 1);
    for (int child : g[r])
    {
        if (!deleted[child])
        {
            dfs_query(child, r, 1);
            dfs_add(child, r, 1, 1);
        }
    }
    dfs_add(r, -1, 0, -1);
    deleted[r] = true;
    for (int child : g[r])
    {
        if (!deleted[child])
        {
            recurse(child);
        }
    }
}

int main()
{
    cin >> n >> k1 >> k2;

    for (int i = 1; i <= n - 1; ++i)
    {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    recurse(1);
    cout << answer << '\n';
}