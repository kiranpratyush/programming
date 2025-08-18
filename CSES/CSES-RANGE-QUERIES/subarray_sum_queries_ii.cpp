#include <cstdio>
#include <vector>
#define ll long long
using namespace std;
typedef struct
{
    ll left_sum;
    ll right_sum;
    ll max_sum;
    ll sum;
} tree_t;
void build(vector<tree_t> &tree, vector<int> &array, int index, int left, int right)
{
    if (left == right)
    {
        ll sum = array[left];
        ll left_sum = max(0ll, sum);
        ll right_sum = max(0ll, sum);
        ll max_sum = max(0ll, sum);
        tree[index] = {left_sum, right_sum, max_sum, sum};
        return;
    }
    int mid = (left + right) / 2;
    build(tree, array, 2 * index, left, mid);
    build(tree, array, 2 * index + 1, mid + 1, right);
    auto left_tree = tree[2 * index];
    auto right_tree = tree[2 * index + 1];
    ll left_sum = max(0ll, max(left_tree.left_sum, left_tree.sum + right_tree.left_sum));
    ll right_sum = max(0ll, max(right_tree.right_sum, right_tree.sum + left_tree.right_sum));
    ll sum = left_tree.sum + right_tree.sum;
    ll max_sum = max(0ll, max(left_tree.max_sum, max(right_tree.max_sum, left_tree.right_sum + right_tree.left_sum)));
    tree[index] = {left_sum, right_sum, max_sum, sum};
}
tree_t query(vector<tree_t> &tree, int index, int left_range, int right_range, int left, int right)
{
    if (left_range <= left && right_range >= right)
    {
        return tree[index];
    }
    if (right < left_range || left > right_range)
    {
        return {0, 0, 0, 0};
    }
    int mid = (left + right) / 2;
    auto left_tree = query(tree, 2 * index, left_range, right_range, left, mid);
    auto right_tree = query(tree, 2 * index + 1, left_range, right_range, mid + 1, right);
    ll left_sum = max(0ll, max(left_tree.left_sum, left_tree.sum + right_tree.left_sum));
    ll right_sum = max(0ll, max(right_tree.right_sum, right_tree.sum + left_tree.right_sum));
    ll sum = left_tree.sum + right_tree.sum;
    ll max_sum = max(0ll, max(left_tree.max_sum, max(right_tree.max_sum, left_tree.right_sum + right_tree.left_sum)));
    return {left_sum, right_sum, max_sum, sum};
}
int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_elements, num_queries;
    scanf("%d %d", &num_elements, &num_queries);
    vector<int> array(num_elements, 0);
    vector<ll> result;
    vector<tree_t> tree(3 * num_elements, {0});
    for (int i = 0; i < num_elements; i++)
    {
        scanf("%d", &array[i]);
    }
    build(tree, array, 1, 0, num_elements - 1);
    for (int i = 0; i < num_queries; i++)
    {
        int start_range, end_range;
        scanf("%d %d", &start_range, &end_range);
        auto x = query(tree, 1, start_range - 1, end_range - 1, 0, num_elements - 1);
        result.push_back(x.max_sum);
    }
    for (int i = 0; i < num_queries; i++)
    {
        printf("%lld \n", result[i]);
    }
}