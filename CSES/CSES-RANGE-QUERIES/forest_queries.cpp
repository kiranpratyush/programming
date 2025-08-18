
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;
#define ll long long

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ll num_size, num_queries;
    scanf("%lld %lld", &num_size, &num_queries);
    vector<vector<ll>> dp(num_size + 1, vector<ll>(num_size + 1, 0));
    char *values = (char *)malloc(sizeof(char) * (num_size + 1));
    for (int i = 1; i < num_size + 1; i++)
    {
        scanf("%s", values);
        for (int col = 0; col < num_size; col++)
        {
            char value = values[col];
            if (value == '*')
            {
                dp[i][col + 1] = 1;
            }
            else
            {
                dp[i][col + 1] = 0;
            }
        }
    }
    // // build the prefix sum
    for (int row = 1; row <= num_size; row++)
    {
        for (int col = 1; col <= num_size; col++)
        {
            dp[row][col] = dp[row][col] + dp[row][col - 1] + dp[row - 1][col] - dp[row - 1][col - 1];
        }
    }
    for (int query = 0; query < num_queries; query++)
    {
        int y1, x1, y2, x2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        ll value = dp[x2][y2] - dp[x1 - 1][y2] - dp[x2][y1 - 1] + dp[x1 - 1][y1 - 1];
        printf("%lld\n", value);
    }
}