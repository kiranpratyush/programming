#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

#define ll long long

int main()
{

    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    ll array_size, target_sum;
    scanf("%lld %lld", &array_size, &target_sum);
    vector<pair<ll, ll>> array;
    for (ll i = 0; i < array_size; i++)
    {
        ll value;
        scanf("%lld", &value);
        array.push_back({value, i});
    }
    sort(array.begin(), array.end());
    if (array_size < 4)
    {
        printf("IMPOSSIBLE");
        return 0;
    }
    for (int i = 0; i < array_size - 3; i++)
    {
        for (int j = i+1; j< array_size - 2; j++)
        {
            auto first_value = array[i];
            auto second_value = array[j];
            auto required_sum = target_sum - (first_value.first+second_value.first);
            ll start_pointer = j+ 1;
            ll end_pointer = array_size - 1;
            while (start_pointer < end_pointer)
            {
                auto sum = array[start_pointer].first + array[end_pointer].first;
                if (sum == required_sum)
                {
                    printf("%lld %lld %lld %lld", first_value.second + 1, second_value.second+1,array[start_pointer].second + 1, array[end_pointer].second + 1);
                    return 0;
                }
                else if (sum > required_sum)
                {
                    end_pointer--;
                }
                else
                {
                    start_pointer++;
                }
            }
        }
    }

    printf("IMPOSSIBLE");
}
