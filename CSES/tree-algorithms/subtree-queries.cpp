#include <cstdio>
#include <vector>
#include <map>
using namespace std;

typedef struct
{
    int start_index;
    int end_index;
} start_end_pos;

int timer = 0;

void dfs(int node, int parent, vector<vector<int>> &tree, map<int, start_end_pos> &position)
{
    position[node] = {++timer, 0};
    for (auto child : tree[node])
    {
        if (child != parent)
        {
            dfs(child, node, tree, position);
        }
    }
    position[node].end_index = timer;
}

void update(vector<long long> &tree, int index, int max_index, long long value)
{
    while (index <= max_index)
    {
        tree[index] += value;
        index += (index & -index);
    }
}

long long read(vector<long long> &tree, int target_index)
{
    long long result = 0;
    while (target_index >= 1)
    {
        result += tree[target_index];
        target_index -= (target_index & -target_index);
    }
    return result;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes, num_queries;
    scanf("%d %d", &num_nodes, &num_queries);
    vector<long long> node_values(num_nodes + 1);
    for (int i = 1; i <= num_nodes; i++)
    {
        long long node_value;
        scanf("%lld", &node_value);
        node_values[i] = node_value;
    }
    vector<vector<int>> tree(num_nodes + 1);
    for (int i = 0; i < num_nodes - 1; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }
    map<int, start_end_pos> position;
    vector<long long> prefix_sum(num_nodes + 1);
    vector<long long> result;
    dfs(1, 1, tree, position);
    for (int i = 1; i <= num_nodes; i++)
    {
        auto node_position = position[i];
        update(prefix_sum, node_position.start_index, num_nodes, node_values[i]);
    }
    for (int i = 0; i < num_queries; i++)
    {
        int query_type;
        scanf("%d", &query_type);
        if (query_type == 1)
        {
            int node;
            long long new_value;
            scanf("%d %lld", &node, &new_value);
            long long old_value = node_values[node];
            long long increment = new_value - old_value;
            node_values[node] = new_value;
            auto node_position = position[node];
            update(prefix_sum, node_position.start_index, num_nodes, increment);
        }
        else
        {
            int node;
            scanf("%d", &node);
            auto node_position = position[node];
            long long first_sum = read(prefix_sum, node_position.end_index);
            long long second_sum = read(prefix_sum, node_position.start_index - 1);
            result.push_back(first_sum - second_sum);
        }
    }
    for (auto x : result)
    {
        printf("%lld \n", x);
    }
}