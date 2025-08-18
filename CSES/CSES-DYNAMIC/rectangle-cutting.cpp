#include <cstdio>
#include <vector>
#include <climits>
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int width, height;
    scanf("%d %d", &width, &height);
    vector<vector<int>> dp(width + 1, vector<int>(height + 1, INT_MAX));
    dp[1][1] = 0;
    // fill the row wise
    for (int i = 2; i <= height; i++)
    {
        dp[1][i] = dp[1][i - 1] + 1;
    }
    // fill the colum wise
    for (int i = 2; i <= width; i++)
    {
        dp[i][1] = dp[i - 1][1] + 1;
    }
    // fill the diagonal
    for (int i = 2; i <= width; i++)
    {
        for (int j = 2; j <= height; j++)
        {
            if (i == j)
            {
                dp[i][j] = 0;
            }
            else
            {
                // break by width
                int min_value = INT_MAX;
                for (int b = 1; b < i; b++)
                {
                    int second_break = i - b;
                    min_value = min(min_value, (dp[b][j] + dp[second_break][j] + 1));
                }
                // break by height
                for (int b = 1; b < j; b++)
                {
                    int second_break = j - b;
                    min_value = min(min_value, (dp[i][b] + dp[i][second_break] + 1));
                }
                dp[i][j] = min_value;
            }
        }
    }
    printf("%d", dp[width][height]);
}