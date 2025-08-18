#include <set>
#include <cstdio>
using namespace std;

int main()
{
    multiset<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(2);
    auto x = s.lower_bound(2);
    printf("%d", *(--x));
}