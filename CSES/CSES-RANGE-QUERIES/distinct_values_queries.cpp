#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
#define ll long long
using namespace std;
typedef struct
{
    vector<ll> values;
} tree_struct_t;

void build(vector<tree_struct_t> &tree, vector<ll> &array, ll index, ll left, ll right)
{
    if (left == right)
    {
        tree[index].values.push_back(array[left]);
        return;
    }
    ll mid = (left + right) / 2;
    build(tree, array, 2 * index, left, mid);
    build(tree, array, 2 * index + 1, mid + 1, right);
    vector<ll> &left_array = tree[2 * index].values;
    vector<ll> &right_array = tree[2 * index + 1].values;
    vector<ll> &current_array = tree[index].values;
    ll first_pointer = 0;
    ll second_pointer = 0;
    while (first_pointer < left_array.size() && second_pointer < right_array.size())
    {
        if (left_array[first_pointer] < right_array[second_pointer])
        {
            current_array.push_back(left_array[first_pointer]);
            first_pointer++;
        }
        else if (left_array[first_pointer] == right_array[second_pointer])
        {
            current_array.push_back(left_array[first_pointer]);
            first_pointer++;
            current_array.push_back(right_array[second_pointer]);
            second_pointer++;
        }
        else
        {
            current_array.push_back(right_array[second_pointer]);
            second_pointer++;
        }
    }
    while (first_pointer < left_array.size())
    {
        current_array.push_back(left_array[first_pointer]);
        first_pointer++;
    }
    while (second_pointer < right_array.size())
    {
        current_array.push_back(right_array[second_pointer]);
        second_pointer++;
    }
}
ll query(vector<tree_struct_t> &tree, ll index, ll start_range, ll end_range, ll start, ll end)
{
    // check if the range lies inside the start and end if return upper_bound() and find the distance
    if (start_range <= start && end <= end_range)
    {
        auto upper_index = upper_bound(tree[index].values.begin(), tree[index].values.end(), end_range);
        if (upper_index != (tree[index].values.end()))
        {
            ll minimum_index = upper_index - (tree[index].values.begin());
            ll total_unique_values = tree[index].values.size() - minimum_index;
            return total_unique_values;
        }
        return 0;
    }
    // if the range is fully outside the start_value
    if (end < start_range || end_range < start)
    {
        return 0;
    }
    ll mid = (start + end) / 2;
    ll left_count = query(tree, 2 * index, start_range, end_range, start, mid);
    ll right_count = query(tree, 2 * index + 1, start_range, end_range, mid + 1, end);
    return left_count + right_count;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int array_size, num_queries;
    scanf("%d %d", &array_size, &num_queries);
    vector<ll> array(array_size, 0);
    vector<tree_struct_t> tree(4 * array_size);
    vector<ll> right_index(array_size, array_size);
    vector<ll> result;
    map<ll, ll> mapper;
    for (ll i = 0; i < array_size; i++)
    {
        scanf("%lld", &array[i]);
    }
    for (ll i = array_size - 1; i >= 0; i--)
    {
        ll element = array[i];
        auto existing_element_it = mapper.find(element);
        if (existing_element_it != mapper.end())
        {
            right_index[i] = existing_element_it->second;
        }
        else
        {
            right_index[i] = array_size;
        }
        mapper[element] = i;
    }
    build(tree, right_index, 1, 0, array_size - 1);
    for (ll i = 0; i < num_queries; i++)
    {
        ll start_range, end_range;
        scanf("%lld %lld", &start_range, &end_range);
        ll r = query(tree, 1, start_range - 1, end_range - 1, 0, array_size - 1);
        result.push_back(r);
    }
    for (ll i = 0; i < num_queries; i++)
    {
        printf("%lld \n", result[i]);
    }
}