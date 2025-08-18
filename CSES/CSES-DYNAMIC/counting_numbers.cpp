#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

typedef long long ll;

ll dp[20][11][2][2];

ll count_digits(int index, int max_length, const vector<int> &digits, int edge, int last_digit, int started)
{
    if (index == max_length)
    {
        return started; // Only count if number has started
    }
    if (dp[index][last_digit + 1][edge][started] != -1)
    {
        return dp[index][last_digit + 1][edge][started];
    }
    int max_digit = (edge ? digits[index] : 9);
    ll count = 0;
    for (int i = 0; i <= max_digit; i++)
    {
        if (!started && i == 0)
        {
            // Still leading zeros, last_digit stays -1
            int new_edge = (edge && (i == max_digit)) ? 1 : 0;
            count += count_digits(index + 1, max_length, digits, new_edge, -1, 0);
        }
        else
        {
            if (i != last_digit)
            {
                int new_edge = (edge && (i == max_digit)) ? 1 : 0;
                count += count_digits(index + 1, max_length, digits, new_edge, i, 1);
            }
        }
    }
    dp[index][last_digit + 1][edge][started] = count;
    return count;
}

vector<int> get_digits(ll num)
{
    vector<int> digits;
    if (num == 0)
    {
        digits.push_back(0);
        return digits;
    }
    while (num > 0)
    {
        digits.push_back(num % 10);
        num = num / 10;
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

ll solve(ll num)
{
    if (num < 0)
        return 0;
    vector<int> digits = get_digits(num);
    memset(dp, -1, sizeof(dp));
    return count_digits(0, digits.size(), digits, 1, -1, 0);
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    ll a,
        b;
    scanf("%lld %lld", &a, &b);
    ll count1 = solve(a - 1);
    ll count2 = solve(b);
    if (a - 1 >= 0)
    {
        count1 += 1;
    }
    if (b >= 0)
    {
        count2 += 1;
    }
    ll ans = count2 - count1;
    printf("%lld\n", ans);
}
