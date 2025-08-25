/*
In an array of length K , the smallest number
which is not present
can be 0,1,2,3,4,..K right
example an array of length 3
Example1:
0,1,2 : Then the smallest number not present is 3
0,5,6 : Then the smallest number not present is 1
To make sure the array contains the smallest number I have to be greedy and
pack as close as possible.
8 3
1 2 1 0 5 1 1 0
Hence keep a hash map which will store
the frequency of elements
initialize a sorted set from 0-K initially all are missing
when you add an element if it is between 0 and K check if the element is
present in the sorted set remove it
when we are removing an element from the window check that if frequency becomes 0
if yes and the element is between 0 and K then then I can add the element to the not visible element
*/

#include <cstdio>
#include <map>
#include <set>
#include <vector>

using namespace std;

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int num_elements, window_size;
    vector<int> result;
    map<int, int> hashmap;
    set<int> sorted_set;
    int start = 0;
    scanf("%d %d", &num_elements, &window_size);
    vector<int> array(num_elements);
    for (int i = 0; i < num_elements; i++)
    {
        scanf("%d", &array[i]);
    }
    for (int i = 0; i <= window_size; i++)
    {
        sorted_set.insert(i);
    }
    for (int i = 1; i <= num_elements; i++)
    {
        int element = array[i - 1];
        hashmap[element]++;
        if (sorted_set.find(element) != sorted_set.end())
        {
            sorted_set.erase(sorted_set.find(element));
        }
        if (i >= window_size)
        {
            result.push_back(*sorted_set.begin());
            int element = array[start];
            hashmap[element]--;
            if (hashmap[element] == 0 && element <= window_size)
            {
                sorted_set.insert(element);
            }
            start++;
        }
    }
    for (auto x : result)
    {
        printf("%d ", x);
    }
}