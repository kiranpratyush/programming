#include <cstdio>
#include <vector>
#include <climits>
#define ll long long
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    ll n;
    scanf("%lld", &n);
    vector<ll> scores(n);
    for (int i = 0; i < n; ++i)
        scanf("%lld", &scores[i]);

    vector<vector<ll>> dp_first(n, vector<ll>(n, 0));  // when it's player's turn
    vector<vector<ll>> dp_second(n, vector<ll>(n, 0)); // when it's opponent's turn

    for (int len = 0; len < n; ++len)
    {
        for (int i = 0; i + len < n; ++i)
        {
            int j = i + len;

            if (i == j)
            {
                dp_first[i][j] = scores[i];
                dp_second[i][j] = 0;
            }
            else
            {
                // First player turn
                ll pick_i = scores[i] + dp_second[i + 1][j];
                ll pick_j = scores[j] + dp_second[i][j - 1];
                dp_first[i][j] = max(pick_i, pick_j);

                // Second player's move, minimizing first player's gain
                ll op_pick_i = dp_first[i + 1][j];
                ll op_pick_j = dp_first[i][j - 1];
                dp_second[i][j] = min(op_pick_i, op_pick_j);
            }
        }
    }

    printf("%lld", dp_first[0][n - 1]);
}
