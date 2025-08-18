#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    long long n, k, x, a, b, c;
    scanf("%lld %lld", &n, &k);
    scanf("%lld %lld %lld %lld", &x, &a, &b, &c);
    long long start, result, intermediate_xor;
    start = x;
    intermediate_xor = 0;
    result = 0;
    long long i = 1;
    while (i <= n)
    {
        intermediate_xor ^= x;
        x = (a * x + b) % c;
        if (i >= k)
        {
            result ^= intermediate_xor;
            intermediate_xor ^= start;
            start = (a * start + b) % c;
        }
        i++;
    }
    printf("%lld\n", result);
    printf("%lf", pow(2, 30));
}