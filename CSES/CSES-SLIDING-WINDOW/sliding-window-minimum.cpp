#include <iostream>
#include <vector>
#include <deque> // For std::deque
#include <cstdio>

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    long long num_elements, window_size;
    scanf("%lld %lld", &num_elements, &window_size);

    long long x, a, b, c;
    scanf("%lld %lld %lld %lld", &x, &a, &b, &c);

    // Store the generated numbers to access them by index
    std::vector<long long> nums(num_elements);
    for (int i = 0; i < num_elements; ++i)
    {
        nums[i] = x;
        x = (a * x + b) % c;
    }

    // Deque will store INDICES of elements in the current window
    std::deque<long long> window_indices;
    long long result = 0;

    for (long long i = 0; i < num_elements; ++i)
    {
        // 1. Remove indices from the front that are no longer in the window
        if (!window_indices.empty() && window_indices.front() <= i - window_size)
        {
            window_indices.pop_front();
        }

        // 2. Maintain monotonic property. Remove indices from the back
        // that correspond to values greater than the current value.
        while (!window_indices.empty() && nums[window_indices.back()] >= nums[i])
        {
            window_indices.pop_back();
        }

        // 3. Add the current element's index to the back
        window_indices.push_back(i);

        // 4. If the window is full, the front of the deque is the minimum
        if (i >= window_size - 1)
        {
            result ^= nums[window_indices.front()];
        }
    }

    printf("%lld\n", result);

    return 0;
}