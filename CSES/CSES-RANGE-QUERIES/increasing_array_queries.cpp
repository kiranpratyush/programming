#include <cstdio>
#include <vector>
#include <algorithm>
#define ll long long
using namespace std;

// Structure for each node in the segment tree
struct Node
{
    vector<ll> array;      // Increasing array in this segment
    vector<ll> prefix_sum; // Prefix sums for cost calculation
    ll cost;               // Operations needed for this segment
};
struct result
{
    ll cost;
    ll max_value;
};

// Build the segment tree
void build(vector<Node> &tree, vector<ll> &arr, int node, int start, int end)
{
    if (start == end)
    {
        // Leaf node - just store the original value
        tree[node].array = {arr[start]};
        tree[node].prefix_sum = {arr[start]};
        tree[node].cost = 0;
        return;
    }

    int mid = (start + end) / 2;
    build(tree, arr, 2 * node, start, mid);
    build(tree, arr, 2 * node + 1, mid + 1, end);

    // Merge the two children
    tree[node].array.clear();
    tree[node].prefix_sum.clear();

    // Get the maximum value in the left child
    ll max_left = tree[2 * node].array.back();

    // Merge left child's array (already increasing)
    for (ll val : tree[2 * node].array)
    {
        tree[node].array.push_back(val);
    }

    // Add right child's array, ensuring it's increasing
    ll extra_cost = 0;
    for (ll val : tree[2 * node + 1].array)
    {
        if (val < max_left)
        {
            // Need to increase this element
            extra_cost += (max_left - val);
            tree[node].array.push_back(max_left);
        }
        else
        {
            tree[node].array.push_back(val);
            max_left = val; // Update max_left for future elements
        }
    }

    // Calculate prefix sums
    ll sum = 0;
    for (ll val : tree[node].array)
    {
        sum += val;
        tree[node].prefix_sum.push_back(sum);
    }

    // Total cost = left child cost + right child cost + extra cost
    tree[node].cost = tree[2 * node].cost + tree[2 * node + 1].cost + extra_cost;
}

// Query function to find minimum operations needed
struct result query(vector<Node> &tree, int node, int start, int end, int left, int right, ll running_max = 0)
{
    // No overlap with query range
    if (end < left || right < start)
    {
        return {0, running_max};
    }

    // Complete overlap with query range
    if (left <= start && end <= right)
    {
        // If running_max is 0, no constraint from left
        if (running_max == 0)
        {
            return {tree[node].cost, max(running_max, tree[node].array.back())};
        }

        // Find how many elements need to be increased to match running_max
        auto it = lower_bound(tree[node].array.begin(), tree[node].array.end(), running_max);
        int idx = it - tree[node].array.begin();

        // Cost to increase elements to running_max
        ll additional_cost = 0;
        if (idx > 0)
        {
            additional_cost = running_max * idx - tree[node].prefix_sum[idx - 1];
        }

        return {tree[node].cost + additional_cost, max(running_max, tree[node].array.back())};
    }

    // Partial overlap - divide and conquer
    int mid = (start + end) / 2;

    // Query left child
    auto left_cost = query(tree, 2 * node, start, mid, left, right, running_max);

    // Find new running_max after processing left child

    // Query right child with updated running_max
    auto right_cost = query(tree, 2 * node + 1, mid + 1, end, left, right, left_cost.max_value);

    return {left_cost.cost + right_cost.cost, max(left_cost.max_value, right_cost.max_value)};
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

    // Build segment tree
    vector<Node> tree(4 * n);
    build(tree, arr, 1, 0, n - 1);

    // Process queries
    for (int i = 0; i < q; i++)
    {
        int a, b;
        scanf("%d %d", &a, &b);
        // Convert to 0-indexed
        a--;
        b--;

        // Query for minimum operations needed
        ll result = query(tree, 1, 0, n - 1, a, b).cost;
        printf("%lld\n", result);
    }

    return 0;
}