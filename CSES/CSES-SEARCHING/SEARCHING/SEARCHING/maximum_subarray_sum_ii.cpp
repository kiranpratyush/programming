#include <set>
#include <vector>
#include <cstdio>
#include <climits>
using namespace std;
#define ll long long

int main()
{

    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int size_of_array, min_length, max_length;
    scanf("%d %d %d", &size_of_array, &min_length, &max_length);
    vector<int> array(size_of_array, 0);
    vector<ll> prefix_sum_array;
    for (int i = 0; i < size_of_array; i++)
    {
        int array_element;
        scanf("%d", &array_element);
        array[i] = array_element;
    }
    // calculate the prefix sum
    ll sum = 0;
    for (int i = 0; i < size_of_array; i++)
    {

        sum += array[i];
        prefix_sum_array.push_back(sum);
    }
    int start_index = min_length - 1;
    int end_index = 0;
    ll max_sum = LLONG_MIN;
    multiset<ll> min_sums;
    while (start_index < size_of_array)
    {
        if (start_index > max_length)
        {
            // erase the prefix sum from the min_sums
            min_sums.erase(min_sums.find(prefix_sum_array[start_index - max_length - 1]));
        }

        if (start_index <= max_length - 1)
        {
            max_sum = max(max_sum, prefix_sum_array[start_index]);
        }

        if (start_index >= min_length)
        {
            // Here we need to subtract the prefix sum;
            max_sum = max(max_sum, prefix_sum_array[start_index] - (*min_sums.begin()));
        }

        // push the current prefix_sum into the min_sums
        min_sums.insert(prefix_sum_array[end_index]);
        end_index += 1;
        start_index += 1;
    }
    printf("%lld", max_sum);
}