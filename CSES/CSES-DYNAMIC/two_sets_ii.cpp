#include <cstdio>
#include <vector>
#define ll long long
#define mod 1000000007
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num;
    scanf("%d", &num);
    ll sum = (num * (num + 1)) / 2;
    if (sum % 2 != 0)
    {
        printf("%d", 0);
    }
    else
    {
        sum = sum / 2;
        vector<vector<ll>> dp(num + 1, vector<ll>(sum + 1, 0));
        // fill the first colum
        for (int i = 0; i <= num; i++)
        {
            dp[i][0] = 1;
        }
        dp[1][1] = 1;
        for (int i = 2; i <= num; i++)
        {
            for (int s = 1; s <= sum; s++)
            {
                // first choice : if the number is greater than equal to s then subtract it with the previous index
                // second choice : check with the previous row
                ll first = 0;
                ll second = 0;
                if (s >= i)
                {
                    first = dp[i - 1][s - i];
                }
                second = dp[i - 1][s];
                dp[i][s] = (first + second) % mod;
            }
        }
        ll inv2 = 500000004; // modular inverse of 2 mod 1e9+7
        printf("%lld", (dp[num][sum] * inv2) % mod);
    }
}