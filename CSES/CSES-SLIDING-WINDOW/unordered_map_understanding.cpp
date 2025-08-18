#include <cstdio>
#include <unordered_map>
using namespace std;

int main()
{
    unordered_map<int, int> um = {{1, 1}, {1, 2}, {2, 3}};
    for (auto i : um)
    {
        printf("%d %d", i.first, i.second);
    }
}