#include <cstdio>
#include <vector>
using namespace std;

/*
What is the main idea
Array: [3,0,1,8,2,4,7,6]
Window size:5
1st Window index: [0,1,2,3,4]
1st Window:       [3,0,1,8,2]
2nd Window index: [1,2,3,4,5]
2nd Window:       [0,1,8,2,4]
3rd Window index: [2,3,4,5,6]
3rd Window:       [1,8,2,4,7]
4th Window index: [3,4,5,6,7]
4th Window:       [20,21,22,23,24]

1.At each index we may be entering into the next window.
2.In the current window we need the result of the current window (The suffix of it )
3.If we are entering into the next window we need the prefix of the result.
Algorithm:
1. Calculate the prefix and suffix or  in the window size of given window (K)
2. Always start the prefix or at the start of the window
3. Always end the suffix or at the start of the window.
4. For result calculation check how much distance we are entering into the next window,then take the prefix or of the next window
*/

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    long long num_elements, window_size, x, a, b, c;
    scanf("%lld %lld", &num_elements, &window_size);
    scanf("%lld %lld %lld %lld", &x, &a, &b, &c);
    vector<long long> elements(num_elements, 0);
    for (int i = 0; i < num_elements; i++)
    {
        elements[i] = x;
        x = (x * a + b) % c;
    }
    vector<long long> prefix_or(num_elements, 0);
    vector<long long> suffix_or(num_elements, 0);
    long long partial_prefix_or = 0;
    long long partial_suffix_or = 0;
    // calculate the prefix or
    for (int i = 0; i < num_elements; i++)
    {
        if (i % window_size == 0)
        {
            partial_prefix_or = 0;
        }
        partial_prefix_or |= elements[i];
        prefix_or[i] = partial_prefix_or;
    }
    // calculate suffix or
    for (int i = num_elements - 1; i >= 0; i--)
    {
        partial_suffix_or |= elements[i];
        suffix_or[i] = partial_suffix_or;
        if (i % window_size == 0)
        {
            partial_suffix_or = 0;
        }
    }
    long long result = 0;
    for (int i = 0; i <= (num_elements - window_size); i++)
    {
        long long r = suffix_or[i];
        long long remainder = i % window_size;
        if (remainder > 0)
        {
            r |= prefix_or[i + window_size - 1];
        }
        result ^= r;
    }
    printf("%lld", result);
}