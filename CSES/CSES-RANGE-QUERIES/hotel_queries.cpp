#include <cstdio>
#include <vector>
#define vi vector<int>
using namespace std;
struct result
{
    int value;
    int index;
};

void build(vi &tree, vi &array, int index, int start_index, int end_index)
{
    if (start_index > end_index)
    {
        return;
    }
    if (start_index == end_index)
    {
        // This is a leaf node
        tree[index] = array[start_index];
    }
    else
    {
        int mid = (start_index + end_index) / 2;
        build(tree, array, 2 * index, start_index, mid);
        build(tree, array, 2 * index + 1, mid + 1, end_index);
        tree[index] = max(tree[2 * index], tree[2 * index + 1]);
    }
}
struct result query(vi &tree, int index, int start_index, int end_index, int value)
{
    if (tree[index] < value)
    {
        return {-1, -1};
    }
    if (start_index == end_index)
    {
        if (tree[index] >= value)
        {
            return {tree[index], start_index};
        }
        return {-1, -1};
    }
    int mid = (start_index + end_index) / 2;
    auto left_result = query(tree, 2 * index, start_index, mid, value);
    if (left_result.value != -1)
    {
        return left_result;
    }
    return query(tree, 2 * index + 1, mid + 1, end_index, value);
}
void update(vi &tree, vi &array, int value, int update_index, int start_index, int end_index, int index)
{
    if (start_index == end_index && start_index == update_index)
    {
        tree[index] = value;
        return;
    }
    else
    {
        // find the mid
        int mid = (start_index + end_index) / 2;
        if (update_index <= mid)
        {
            update(tree, array, value, update_index, start_index, mid, 2 * index);
        }
        else
        {
            update(tree, array, value, update_index, mid + 1, end_index, 2 * index + 1);
        }
    }
    tree[index] = max(tree[2 * index], tree[2 * index + 1]);
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_hotels, num_groups;
    scanf("%d %d", &num_hotels, &num_groups);
    vi tree(4 * num_hotels, 0);
    vi array(num_hotels, 0);
    vi result_array(num_groups, 0);
    for (int i = 0; i < num_hotels; i++)
    {
        scanf("%d", &array[i]);
    }
    build(tree, array, 1, 0, num_hotels - 1);
    for (int i = 0; i < num_groups; i++)
    {
        int value;
        scanf("%d", &value);
        auto result = query(tree, 1, 0, num_hotels - 1, value);
        if (result.value == -1)
        {
            result_array[i] = 0;
        }
        else
        {
            result_array[i] = result.index + 1;
            int new_value = result.value - value;
            update(tree, array, new_value, result.index, 0, num_hotels - 1, 1);
        }
    }
    for (int i = 0; i < num_groups; i++)
    {
        printf("%d ", result_array[i]);
    }
}