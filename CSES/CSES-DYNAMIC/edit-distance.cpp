#include <iostream>
#include <vector>
using namespace std;

int main()
{

    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    string first_string, second_string;
    cin >> first_string;
    cin >> second_string;
    int length1 = first_string.length();
    int length2 = second_string.length();
    vector<vector<int>> dp(length1 + 1, vector<int>(length2 + 1, 0));
    dp[length1][length2] = 0;
    for (int i = length1 - 1; i >= 0; i--)
    {
        dp[i][length2] = dp[i + 1][length2] + 1;
    }
    for (int i = length2 - 1; i >= 0; i--)
    {
        dp[length1][i] = dp[length1][i + 1] + 1;
    }
    for (int i = length1 - 1; i >= 0; i--)
    {
        for (int j = length2 - 1; j >= 0; j--)
        {
            if (first_string[i] == second_string[j])
            {
                dp[i][j] = dp[i + 1][j + 1];
            }
            else
            {
                int operation1 = dp[i + 1][j];
                int operation2 = dp[i][j + 1];
                int operation3 = dp[i + 1][j + 1];
                dp[i][j] = min(operation3, min(operation1, operation2)) + 1;
            }
        }
    }
    cout << dp[0][0] << endl;
}
