#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

#define ll long long
void add(vector<ll> &tree, ll index, ll max_length)
{
    while (index <= max_length)
    {
        tree[index] += 1;
        index += (index & -index);
    }
}
void remove(vector<ll> &tree, ll index, ll max_length)
{
    while (index <= max_length)
    {
        tree[index] -= 1;
        index += (index & -index);
    }
}
ll read(vector<ll> &tree, ll index)
{
    ll cummulative_sum = 0;
    while (index >= 1)
    {
        cummulative_sum += tree[index];
        index -= (index & -index);
    }
    return cummulative_sum;
}

int main()
{ // Read the input
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ll num_children, num_skip;
    scanf("%lld %lld", &num_children, &num_skip);
    vector<ll> tree(num_children + 1, 0);
    vector<ll> result;
    for (ll i = 1; i <= num_children; i++)
    {
        add(tree, i, num_children);
    }
    /*
    Iterate over the num_children
    Calculate the result index
        i. find the max cummulative sum
        ii. new_position = (old_position%max_sum+num_skip)%max_sum
        ii. if new_position is zero the postion becomes the max_sum
        iii. Now find the lowest  index where the cummulative sum is equal to the new_position

    */
    ll old_sum = 1;
    for (ll i = 0; i < num_children; i++)
    {
        ll max_sum = read(tree, num_children);
        ll new_position = (old_sum % max_sum + num_skip % max_sum) % max_sum;
        if (new_position == 0)
        {
            new_position = max_sum;
        }
        ll start = 1;
        ll end = num_children;
        while (start <= end)
        {
            ll mid = (start + end) / 2;
            ll curr_cummulative_sum = read(tree, mid);
            ll prev_cummulative_sum = read(tree, mid - 1);
            if (curr_cummulative_sum == new_position && prev_cummulative_sum < new_position)
            {
                result.push_back(mid);
                remove(tree, mid, num_children);
                break;
            }
            else if (curr_cummulative_sum < new_position)
            {
                start = mid + 1;
            }
            else
            {
                end = mid - 1;
            }
        }
        old_sum = new_position;
    }
    for (int i = 0; i < num_children; i++)
    {
        printf("%lld ", result[i]);
    }
}