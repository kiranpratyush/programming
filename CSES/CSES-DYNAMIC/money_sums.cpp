#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_integers;
    scanf("%d", &num_integers);
    vector<int> money(num_integers, 0);
    for (int i = 0; i < num_integers; i++)
    {
        scanf("%d", &money[i]);
    }
    int total_sum = 0;
    for (int i = 0; i < num_integers; i++)
    {
        total_sum += money[i];
    }
    vector<vector<int>> dp(num_integers, vector<int>(total_sum + 1, 0));
    for (int row = 0; row < num_integers; row++)
    {
        dp[row][0] = 1;
    }
    for (int col = 1; col <= total_sum; col++)
    {
        if (money[0] == col)
        {
            dp[0][col] = 1;
        }
        else
        {
            dp[0][col] = 0;
        }
    }
    for (int row = 1; row < num_integers; row++)
    {
        for (int col = 1; col <= total_sum; col++)
        {
            int first = 0;
            int second = 0;
            if (money[row] <= col)
            {
                first = dp[row - 1][col - money[row]];
            }
            second = dp[row - 1][col];
            dp[row][col] = first || second;
        }
    }
    vector<int> sum_possible;
    int total_sum_possible = 0;
    for (int i = 1; i <= total_sum; i++)
    {
        if (dp[num_integers - 1][i] == 1)
        {
            total_sum_possible += 1;
            sum_possible.push_back(i);
        }
    }
    sort(sum_possible.begin(), sum_possible.end());
    printf("%d \n", total_sum_possible);
    for (auto &x : sum_possible)
    {
        printf("%d ", x);
    }
}