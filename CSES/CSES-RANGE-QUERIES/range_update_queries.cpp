/*
How to update a value in a sub array range
If I can find for a particular element how much increment I can do then I can find the updated
value.
The idea of difference array is to mark where the update starts and where the update ends
*/
#include <cstdio>
#include <vector>
#define ll long long
using namespace std;

void add(vector<ll> &tree, ll max_size, ll index, ll value)
{
    while (index <= max_size)
    {
        tree[index] += value;
        index += (index & -index);
    }
}
ll read(vector<ll> &tree, ll index)
{
    ll sum = 0;
    while (index > 0)
    {
        sum += tree[index];
        index -= (index & -index);
    }
    return sum;
}
int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ll num_values, num_queries;
    scanf("%lld %lld", &num_values, &num_queries); // 8 3
    vector<ll> array(num_values, 0);
    vector<ll> tree(num_values + 2, 0);
    for (ll i = 0; i < num_values; i++)
    {
        scanf("%lld", &array[i]);
    }
    for (ll i = 0; i < num_queries; i++)
    {
        ll query_type;
        scanf("%lld", &query_type);
        if (query_type == 1)
        {
            // here build the tree
            ll start_range, end_range, value;
            scanf("%lld %lld %lld", &start_range, &end_range, &value);
            // add the values to the tree in the start_range and the end_range
            add(tree, num_values + 1, start_range, value);
            add(tree, num_values + 1, end_range + 1, -value);
        }
        else if (query_type == 2)
        {
            // Build answer here
            ll index_to_read;
            scanf("%lld", &index_to_read);
            // read from the tree;
            ll tree_update = read(tree, index_to_read);
            ll array_value = array[index_to_read - 1];
            printf("%lld\n", array_value + tree_update);
        }
    }
}