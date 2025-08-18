#include <cstdio>
#include <vector>
#include <algorithm>
#define ll long long
using namespace std;

typedef struct
{
    vector<ll> value;
} tree_node_t;
typedef struct
{
    ll max_value;
    ll count;
} result;

void build(vector<tree_node_t> &tree, vector<ll> &array, int index, int left, int right)
{
    if (left == right)
    {
        tree[index].value.push_back(array[left]);
        return;
    }
    ll mid = (left + right) / 2;
    build(tree, array, 2 * index, left, mid);
    build(tree, array, 2 * index + 1, mid + 1, right);
    // fill the left_tree
    for (auto x : tree[2 * index].value)
    {
        tree[index].value.push_back(x);
    }
    // now for the right tree if the value is greater than the last value then add it
    for (auto y : tree[2 * index + 1].value)
    {
        if (y > tree[index].value.back())
        {
            tree[index].value.push_back(y);
        }
    }
}

result query(vector<tree_node_t> &tree, ll index, ll start_range, ll end_range, ll left, ll right, ll running_max)
{

    if (start_range <= left && right <= end_range)
    {
        auto it = upper_bound(tree[index].value.begin(), tree[index].value.end(), running_max);
        int idx = it - tree[index].value.begin();
        ll total_value = tree[index].value.size() - idx;
        running_max = max(running_max, tree[index].value.back());
        return {running_max, total_value};
    }
    if (right < start_range || end_range < left)
    {
        return {running_max, 0};
    }

    ll mid = (left + right) / 2;
    auto left_result = query(tree, 2 * index, start_range, end_range, left, mid, running_max);
    auto right_result = query(tree, 2 * index + 1, start_range, end_range, mid + 1, right, left_result.max_value);
    return {right_result.max_value, left_result.count + right_result.count};
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int n, q;
    scanf("%d %d", &n, &q);

    vector<ll> arr(n);
    for (int i = 0; i < n; i++)
    {
        scanf("%lld", &arr[i]);
    }
    vector<tree_node_t> tree(4 * n);
    build(tree, arr, 1, 0, n - 1);
    vector<ll> result;
    // Process queries
    for (int i = 0; i < q; i++)
    {
        int a, b;
        scanf("%d %d", &a, &b);
        // Convert to 0-indexed
        a--;
        b--;

        // Query for minimum operations needed
        ll r = query(tree, 1, a, b, 0, n - 1, 0).count;
        result.push_back(r);
    }
    for (auto x : result)
    {
        printf("%lld\n", x);
    }

    return 0;
}