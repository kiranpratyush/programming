/*
values{
    index,
    min_value
}
left segment tree
right segment tree
*/
#include <cstdio>
#include <vector>
using namespace std;
#define ll long long
// left=>right
void left_build(vector<ll> &tree, vector<int> array, int index, int start_value, int end_value)
{
    if (start_value == end_value)
    {
        tree[index] = array[start_value];
        return;
    }
    int mid = (start_value + end_value) / 2;
    left_build(tree, array, 2 * index, start_value, mid);
    left_build(tree, array, 2 * index + 1, mid + 1, end_value);
    auto left_tree = tree[2 * index];
    auto right_tree = tree[2 * index + 1];
    tree[index] = min(right_tree, left_tree + (end_value - mid));
}
// left<= right
void right_build(vector<ll> &tree, vector<int> array, int index, int start_value, int end_value)
{
    if (start_value == end_value)
    {
        tree[index] = array[start_value];
        return;
    }
    int mid = (start_value + end_value) / 2;
    right_build(tree, array, 2 * index, start_value, mid);
    right_build(tree, array, 2 * index + 1, mid + 1, end_value);
    auto left_tree = tree[2 * index];
    auto right_tree = tree[2 * index + 1];
    tree[index] = min(left_tree, right_tree + (mid + 1 - start_value));
}
void left_update(vector<ll> &tree, int index, int update_index, int value, int start_value, int end_value)
{
    if (start_value > end_value)
    {
        return;
    }
    if (start_value == end_value)
    {
        if (start_value == update_index)
        {
            tree[index] = value;
        }
        return;
    }
    int mid = (start_value + end_value) / 2;
    if (update_index > mid)
    {
        left_update(tree, 2 * index + 1, update_index, value, mid + 1, end_value);
    }
    else
    {
        left_update(tree, 2 * index, update_index, value, start_value, mid);
    }
    auto left_tree = tree[2 * index];
    auto right_tree = tree[2 * index + 1];
    tree[index] = min(right_tree, left_tree + (end_value - mid));
}
void right_update(vector<ll> &tree, int index, int update_index, int value, int start_value, int end_value)
{
    if (start_value > end_value)
    {
        return;
    }
    if (start_value == end_value)
    {
        if (start_value == update_index)
        {
            tree[index] = value;
        }
        return;
    }
    int mid = (start_value + end_value) / 2;
    if (update_index > mid)
    {
        right_update(tree, 2 * index + 1, update_index, value, mid + 1, end_value);
    }
    else
    {
        right_update(tree, 2 * index, update_index, value, start_value, mid);
    }
    auto left_tree = tree[2 * index];
    auto right_tree = tree[2 * index + 1];
    tree[index] = min(left_tree, right_tree + (mid + 1 - start_value));
}
ll left_query(vector<ll> &tree, int index, int start_range, int end_range, int start_value, int end_value)
{
    if (start_range > end_range || start_value > end_value || end_range < start_value || start_range > end_value)
    {
        return -1;
    }
    if (start_value >= start_range && end_value <= end_range)
    {
        return tree[index];
    }
    int mid = (start_value + end_value) / 2;
    ll left_value = left_query(tree, 2 * index, max(start_range, start_value), min(end_range, mid), start_value, mid);
    ll right_value = left_query(tree, 2 * index + 1, max(start_range, mid + 1), min(end_range, end_value), mid + 1, end_value);
    if (left_value == -1)
    {
        return right_value;
    }
    if (right_value == -1)
    {
        return left_value;
    }
    return min(right_value, left_value + (end_range - mid));
}
ll right_query(vector<ll> &tree, int index, int start_range, int end_range, int start_value, int end_value)
{
    if (start_range > end_range || start_value > end_value || end_range < start_value || start_range > end_value)
    {
        return -1;
    }

    if (start_range > end_range)
    {
        return -1;
    }
    if (start_value >= start_range && end_value <= end_range)
    {
        return tree[index];
    }
    int mid = (start_value + end_value) / 2;
    ll left_value = right_query(tree, 2 * index, max(start_range, start_value), min(end_range, mid), start_value, mid);
    ll right_value = right_query(tree, 2 * index + 1, max(start_range, mid + 1), min(end_range, end_value), mid + 1, end_value);
    if (left_value == -1)
    {
        return right_value;
    }
    if (right_value == -1)
    {
        return left_value;
    }
    return min(left_value, right_value + (mid + 1 - start_range));
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_buildings, num_queries;
    scanf("%d %d", &num_buildings, &num_queries);
    vector<int> array(num_buildings, 0);
    vector<ll> left_tree(num_buildings * 4, 0);
    vector<ll> right_tree(num_buildings * 4, 0);
    for (int i = 0; i < num_buildings; i++)
    {
        scanf("%d", &array[i]);
    }
    left_build(left_tree, array, 1, 0, num_buildings - 1);
    right_build(right_tree, array, 1, 0, num_buildings - 1);

    for (int i = 0; i < num_queries; i++)
    {
        int query_type;
        scanf("%d", &query_type);
        if (query_type == 1)
        {
            int building_number, price;
            scanf("%d %d", &building_number, &price);
            left_update(left_tree, 1, building_number - 1, price, 0, num_buildings - 1);
            right_update(right_tree, 1, building_number - 1, price, 0, num_buildings - 1);
        }
        else
        {
            int building_number;
            scanf("%d", &building_number);
            ll value1 = left_query(left_tree, 1, 0, building_number - 1, 0, num_buildings - 1);
            ll value2 = right_query(right_tree, 1, building_number - 1, num_buildings - 1, 0, num_buildings - 1);
            printf("%lld \n", min(value1, value2));
        }
    }
}