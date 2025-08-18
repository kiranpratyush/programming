#include <cstdio>
#include <vector>
using namespace std;

#define ll long long

int timer = 0;

void build(int index, int max_value, int value, vector<ll> &prefix_sum)
{
    while (index <= max_value)
    {
        prefix_sum[index] += value;
        index += (index & -index);
    }
}

ll get_values(int index, vector<ll> &prefix_sum)
{
    ll sum = 0;
    while (index >= 1)
    {
        sum += prefix_sum[index];
        index -= (index & -index);
    }
    return sum;
}
void dfs(int node, int parent, vector<vector<int>> &tree, vector<int> &in, vector<int> &out)
{
    in[node] = ++timer;
    for (auto x : tree[node])
    {
        if (x != parent)
        {
            dfs(x, node, tree, in, out);
        }
    }
    out[node] = timer;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, num_queries;
    scanf("%d %d", &num_nodes, &num_queries);
    vector<int> values(num_nodes + 1);
    vector<int> in(num_nodes + 1);
    vector<int> out(num_nodes + 1);
    vector<vector<int>> tree(num_nodes + 1);
    vector<ll> prefix_sum(num_nodes + 1);
    vector<ll> result;
    for (int i = 1; i <= num_nodes; i++)
    {
        scanf("%d", &values[i]);
    }
    for (int i = 1; i < num_nodes; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }
    dfs(1, 1, tree, in, out);
    for (int i = 1; i <= num_nodes; i++)
    {
        build(in[i], num_nodes, values[i], prefix_sum);
        build(out[i] + 1, num_nodes, -values[i], prefix_sum);
    }
    for (int i = 1; i <= num_queries; i++)
    {
        int query_type;
        scanf("%d", &query_type);
        if (query_type == 1)
        {
            int node, new_value;
            scanf("%d %d", &node, &new_value);
            int increment = new_value - values[node];
            values[node] = new_value;
            build(in[node], num_nodes, increment, prefix_sum);
            build(out[node] + 1, num_nodes, -increment, prefix_sum);
        }
        else
        {
            int node;
            scanf("%d", &node);
            ll r = get_values(in[node], prefix_sum);
            result.push_back(r);
        }
    }
    for (auto x : result)
    {
        printf("%lld\n", x);
    }
}