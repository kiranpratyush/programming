#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve()
{
    int n, k;
    cin >> n >> k;
    int x, a, b, c;
    cin >> x >> a >> b >> c;
    int or_val = x, num = x;
    int bits[30] = {0};
    for (int i = 0; i < 30; i++)
    {
        if ((x >> i) & 1)
            bits[i] += 1;
    }
    for (int i = 0; i < k - 1; i++)
    {
        num = (num * a + b) % c;
        or_val |= num;
        int curr = num;
        while (curr)
        {
            int pos = __builtin_ctz(curr);
            bits[pos]++;
            curr = curr & (curr - 1);
        }
    }
    int res = or_val;
    int rem = x;
    for (int i = 0; i < n - k; i++)
    {
        num = (num * a + b) % c;
        int curr = num;
        while (curr)
        {
            int pos = __builtin_ctz(curr);
            if (bits[pos] == 0)
                or_val |= (1 << pos);
            bits[pos]++;
            curr = curr & (curr - 1);
        }
        curr = rem;
        while (curr)
        {
            int pos = __builtin_ctz(curr);
            if (bits[pos] == 1)
                or_val ^= (1 << pos);
            bits[pos]--;
            curr = curr & (curr - 1);
        }
        res ^= or_val;
        rem = (rem * a + b) % c;
    }
    cout << res << "\n";
    return;
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // cin>>t;
    while (t--)
    {
        solve();
    }
    return 0;
}