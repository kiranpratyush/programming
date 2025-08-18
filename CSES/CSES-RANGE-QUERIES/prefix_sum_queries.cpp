/*
segment tree
typedef sumValue,prefix
*/
#include <cstdio>
#include <vector>
using namespace std;
#define ll long long
typedef struct
{
    ll sum_value;
    ll prefix;
} tree_item_t;

void build(vector<tree_item_t> &tree, vector<int> &array, int index, int start_value, int end_value)
{
    if (start_value > end_value)
    {
        return;
    }
    if (start_value == end_value)
    {
        tree[index] = {array[start_value], max(0, array[end_value])};
        return;
    }
    int mid = (start_value + end_value) / 2;
    build(tree, array, 2 * index, start_value, mid);
    build(tree, array, 2 * index + 1, mid + 1, end_value);
    auto left_value = tree[2 * index];
    auto right_value = tree[2 * index + 1];
    ll max_prefix_value = max(0ll, max(left_value.sum_value + right_value.sum_value, max(left_value.prefix, left_value.sum_value + right_value.prefix)));
    tree[index] = {left_value.sum_value + right_value.sum_value, max_prefix_value};
}
void update(vector<tree_item_t> &tree, int index, int update_index, int value, int start_value, int end_value)
{
    if (start_value > end_value)
    {
        return;
    }
    if (start_value == end_value)
    {
        if (start_value == update_index)
        {
            tree[index] = {value, max(0, value)};
        }
        return;
    }
    int mid = (start_value + end_value) / 2;
    if (update_index > mid)
    {
        update(tree, 2 * index + 1, update_index, value, mid + 1, end_value);
    }
    else
    {
        update(tree, 2 * index, update_index, value, start_value, mid);
    }
    auto left_value = tree[2 * index];
    auto right_value = tree[2 * index + 1];
    ll max_prefix_value = max(0ll, max(left_value.sum_value + right_value.sum_value, max(left_value.prefix, left_value.sum_value + right_value.prefix)));
    tree[index] = {left_value.sum_value + right_value.sum_value, max_prefix_value};
}
tree_item_t query(vector<tree_item_t> &tree, int index, int start_range, int end_range, int start_value, int end_value)
{
    // if  start_value,end_value is fully inside the start_range and end_range return the current value;
    if (start_value > end_range || end_value < start_range)
    {
        return {0, 0};
    }
    if (start_range <= start_value && end_value <= end_range)
    {
        return tree[index];
    }
    int mid = (start_value + end_value) / 2;
    auto left_value = query(tree, 2 * index, start_range, end_range, start_value, mid);
    auto right_value = query(tree, 2 * index + 1, start_range, end_range, mid + 1, end_value);
    ll max_prefix_value = max(0ll, max(left_value.sum_value + right_value.sum_value, max(left_value.prefix, left_value.sum_value + right_value.prefix)));
    return {left_value.sum_value + right_value.sum_value, max_prefix_value};
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_values, num_queries;
    scanf("%d %d", &num_values, &num_queries);
    vector<int> array(num_values, 0);
    vector<tree_item_t> tree(4 * num_values, {0, 0});
    for (int i = 0; i < num_values; i++)
    {
        scanf("%d", &array[i]);
    }
    build(tree, array, 1, 0, num_values - 1);
    for (int i = 0; i < num_queries; i++)
    {
        int query_type, start, end;
        scanf("%d %d %d", &query_type, &start, &end);
        if (query_type == 1)
        {
            update(tree, 1, start - 1, end, 0, num_values - 1);
        }
        else
        {
            auto x = query(tree, 1, start - 1, end - 1, 0, num_values - 1);
            printf("%lld\n", x.prefix);
        }
    }
}