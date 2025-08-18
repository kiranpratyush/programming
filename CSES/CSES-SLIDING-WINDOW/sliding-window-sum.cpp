#include <cstdio>
using namespace std;
#define ll long long;

int main()
{

    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    long long num_elements, window_size, x, a, b, c;
    scanf("%lld %lld", &num_elements, &window_size);
    scanf("%lld %lld %lld %lld", &x, &a, &b, &c);
    long long start = x;
    long long i = 1;
    long long sum = 0;
    long long result = 0;
    while (i <= num_elements)
    {
        sum += x;
        x = (a * x + b) % c;
        if (i >= window_size)
        {
            result ^= sum;
            long long next = (a * start + b) % c;
            sum -= start;
            start = next;
        }
        i++;
    }
    printf("%lld", result);
}