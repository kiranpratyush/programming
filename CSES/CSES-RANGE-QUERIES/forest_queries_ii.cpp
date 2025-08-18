#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define ll long long

void update(vector<vector<ll>> &bit, ll x, ll y, ll forest_size, ll value)
{
    ll original_y = y; // Save the original y value
    while (x <= forest_size)
    {
        y = original_y; // Reset y for each iteration of x
        while (y <= forest_size)
        {
            bit[x][y] += value;
            y += (y & -y);
        }
        x += (x & -x);
    }
}

ll query(vector<vector<ll>> &bit, ll x, ll y)
{
    ll total_value = 0;
    ll original_y = y; // Save the original y value
    while (x >= 1)
    {
        y = original_y; // Reset y for each iteration of x
        while (y >= 1)
        {
            total_value += bit[x][y];
            y -= (y & -y);
        }
        x -= (x & -x);
    }
    return total_value;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ll forest_size, num_queries;
    cin >> forest_size >> num_queries;
    vector<vector<ll>> forest(forest_size, vector<ll>(forest_size, 0));
    vector<vector<ll>> bit(forest_size + 1, vector<ll>(forest_size + 1, 0));
    for (int i = 0; i < forest_size; i++)
    {
        string s;
        cin >> s;
        for (int j = 0; j < forest_size; j++)
        {
            forest[i][j] = (s[j] == '*') ? 1 : 0;
        }
    }
    for (int i = 0; i < forest_size; i++)
    {
        for (int j = 0; j < forest_size; j++)
        {
            if (forest[i][j] == 1)
            {
                update(bit, i + 1, j + 1, forest_size, 1);
            }
        }
    }
    for (int q = 0; q < num_queries; q++)
    {
        int query_type;
        cin >> query_type;
        if (query_type == 1)
        {
            ll start, end;
            cin >> start >> end;
            int original_value = forest[start - 1][end - 1];
            if (original_value == 1)
            {
                forest[start - 1][end - 1] = 0;
                update(bit, start, end, forest_size, -1);
            }
            else
            {
                forest[start - 1][end - 1] = 1;
                update(bit, start, end, forest_size, 1);
            }
        }
        else
        {
            ll x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            ll value1 = query(bit, x2, y2);
            ll value2 = query(bit, x1 - 1, y2);
            ll value3 = query(bit, x2, y1 - 1);
            ll value4 = query(bit, x1 - 1, y1 - 1);
            printf("%lld \n", value1 - value2 - value3 + value4);
        }
    }
}
