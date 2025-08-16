#include<map>
#include<vector>
#include<cstdio>
using namespace std;

#define ll long long


int main(){
    // freopen("../input.txt","r",stdin);
    // freopen("../output.txt","w",stdout);
    ll num_integers;
    scanf("%lld", &num_integers);
 
    vector<ll> arrays;
    for (int i = 0; i < num_integers; i++)
    {
        ll value;
        scanf("%lld", &value);
        arrays.push_back(value);
    }
 
    map<ll, ll> store_values;
    store_values[0] = 1;
 
    ll sum = 0;
    ll total = 0;
 
    for (auto element : arrays)
    {
        sum += element;
        ll required_sum = (sum%num_integers+num_integers)%num_integers;
 
        if (store_values.find(required_sum) != store_values.end())
        {
            total += store_values[required_sum];
        }
 
        store_values[required_sum]++;
    }
 
    printf("%lld\n", total);
    return 0;

}
