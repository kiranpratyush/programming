// This approach has O((n+q) log n) complexity
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
#define LOCAL_DEBUG

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef LOCAL_DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    int n, q;
    cin >> n >> q;

    // Read array values
    vector<int> values(n);
    for (int i = 0; i < n; i++)
        cin >> values[i];

    // Read queries
    vector<pair<int, int>> pos_ranges(q);
    vector<pair<int, int>> val_ranges(q);
    for (int i = 0; i < q; i++)
    {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        pos_ranges[i] = {--a, --b};
        val_ranges[i] = {c, d};
    }

    // Process queries by sorting elements by value
    vector<int> result(q, 0);
    vector<pair<int, int>> elements; // {value, position}

    for (int i = 0; i < n; i++)
    {
        elements.push_back({values[i], i});
    }
    sort(elements.begin(), elements.end());

    // For each query, binary search to find elements in value range
    for (int i = 0; i < q; i++)
    {
        int a = pos_ranges[i].first;
        int b = pos_ranges[i].second;
        int c = val_ranges[i].first;
        int d = val_ranges[i].second;

        auto it_low = lower_bound(elements.begin(), elements.end(), make_pair(c, 0));
        auto it_high = upper_bound(elements.begin(), elements.end(), make_pair(d, n));

        // Count elements in position range [a,b]
        for (auto it = it_low; it != it_high; ++it)
        {
            int pos = it->second;
            if (a <= pos && pos <= b)
            {
                result[i]++;
            }
        }
    }

    // Print results
    for (int i = 0; i < q; i++)
    {
        cout << result[i] << '\n';
    }
}