#include <cstdio>
#include <vector>
#include <unordered_map>
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_elements, window_size;
    scanf("%d %d", &num_elements, &window_size);
    vector<int> array;
    for (int i = 0; i < num_elements; i++)
    {
        int data;
        scanf("%d", &data);
        array.push_back(data);
    }
    vector<int> result;
    int distinct_element = 0;
    unordered_map<int, int> hash_map;
    int start = 0;
    for (int i = 0; i < num_elements; i++)
    {
        // check if the current element is not present
        int element = array[i];
        auto it = hash_map.find(element);
        if (it != hash_map.end())
        {
            hash_map[element] = (it->second) + 1;
        }
        else
        {
            hash_map[element] = 1;
            distinct_element += 1;
        }

        // if the element is present increment the count
        if (i >= window_size - 1)
        {
            result.push_back(distinct_element);
            int first_element = array[start];
            hash_map[first_element] -= 1;
            if (hash_map[first_element] == 0)
            {
                hash_map.erase(first_element);
                distinct_element -= 1;
            }
            start += 1;
        }
    }
    for (auto x : result)
    {
        printf("%d ", x);
    }
}