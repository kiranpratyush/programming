#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <climits>
#define ll long long
using namespace std;

struct Node
{
    ll min_next_occurrence;
};

void build(vector<Node> &tree, vector<ll> &next_occurrence, int node, int start, int end)
{
    if (start == end)
    {
        tree[node].min_next_occurrence = next_occurrence[start];
        return;
    }

    int mid = (start + end) / 2;
    build(tree, next_occurrence, 2 * node, start, mid);
    build(tree, next_occurrence, 2 * node + 1, mid + 1, end);

    tree[node].min_next_occurrence = min(tree[2 * node].min_next_occurrence, tree[2 * node + 1].min_next_occurrence);
}

void update_tree(vector<Node> &tree, int node, int start, int end, int idx, ll val)
{
    if (start == end)
    {
        tree[node].min_next_occurrence = val;
        return;
    }

    int mid = (start + end) / 2;
    if (idx <= mid)
        update_tree(tree, 2 * node, start, mid, idx, val);
    else
        update_tree(tree, 2 * node + 1, mid + 1, end, idx, val);

    tree[node].min_next_occurrence = min(tree[2 * node].min_next_occurrence, tree[2 * node + 1].min_next_occurrence);
}

ll query(vector<Node> &tree, int node, int start, int end, int l, int r)
{
    if (r < start || end < l)
        return INT_MAX;

    if (l <= start && end <= r)
        return tree[node].min_next_occurrence;

    int mid = (start + end) / 2;
    ll left_min = query(tree, 2 * node, start, mid, l, r);
    ll right_min = query(tree, 2 * node + 1, mid + 1, end, l, r);

    return min(left_min, right_min);
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

    // For each value, store positions where it appears
    map<ll, vector<int>> positions;
    for (int i = 0; i < n; i++)
    {
        positions[arr[i]].push_back(i);
    }

    // Calculate next occurrence array
    vector<ll> next_occurrence(n, n);
    for (auto &entry : positions)
    {
        for (int i = 0; i < entry.second.size() - 1; i++)
        {
            next_occurrence[entry.second[i]] = entry.second[i + 1];
        }
    }

    // Build segment tree
    vector<Node> tree(4 * n);
    build(tree, next_occurrence, 1, 0, n - 1);

    while (q--)
    {
        int type, a, b;
        scanf("%d %d %d", &type, &a, &b);

        if (type == 1)
        {
            // Update value at position a-1 to b
            int pos = a - 1;
            ll old_val = arr[pos];
            ll new_val = b;

            if (old_val == new_val)
                continue;

            // Remove position from old_val's position list
            auto &old_positions = positions[old_val];
            for (int i = 0; i < old_positions.size(); i++)
            {
                if (old_positions[i] == pos)
                {
                    if (i > 0)
                    {
                        // Update previous position's next occurrence
                        update_tree(tree, 1, 0, n - 1, old_positions[i - 1],
                                    i + 1 < old_positions.size() ? old_positions[i + 1] : n);
                    }
                    old_positions.erase(old_positions.begin() + i);
                    break;
                }
            }

            // Add position to new_val's position list
            auto &new_positions = positions[new_val];
            auto it = lower_bound(new_positions.begin(), new_positions.end(), pos);
            int insert_idx = it - new_positions.begin();
            new_positions.insert(it, pos);

            // Update this position's next occurrence
            ll next = (insert_idx + 1 < new_positions.size()) ? new_positions[insert_idx + 1] : n;
            update_tree(tree, 1, 0, n - 1, pos, next);

            // Update previous position's next occurrence if exists
            if (insert_idx > 0)
            {
                update_tree(tree, 1, 0, n - 1, new_positions[insert_idx - 1], pos);
            }

            arr[pos] = new_val;
        }
        else
        {
            // Query if range [a-1,b-1] has all distinct values
            int left = a - 1, right = b - 1;

            ll min_next = query(tree, 1, 0, n - 1, left, right);
            if (min_next > right)
            {
                printf("YES\n");
            }
            else
            {
                printf("NO\n");
            }
        }
    }

    return 0;
}