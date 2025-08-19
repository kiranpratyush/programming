#include <set>
#include <map>
#include <cstdio>
#include <vector>

using namespace std;

// Helper function to remove an element from a frequency bucket
void remove_from_bucket(int element, int frequency,
                        map<int, multiset<int>> &frequency_to_multiset,
                        multiset<int> &max_frequency)
{
    if (frequency > 0)
    {
        // Erase the element from its old frequency multiset
        frequency_to_multiset[frequency].erase(frequency_to_multiset[frequency].find(element));
        // If the multiset for this frequency is now empty, remove the map key
        if (frequency_to_multiset[frequency].empty())
        {
            frequency_to_multiset.erase(frequency);
        }
        // Erase one instance of the old frequency from the max_frequency tracker
        max_frequency.erase(max_frequency.find(-frequency));
    }
}

// Helper function to add an element to a frequency bucket
void add_to_bucket(int element, int frequency,
                   map<int, multiset<int>> &frequency_to_multiset,
                   multiset<int> &max_frequency)
{
    frequency_to_multiset[frequency].insert(element);
    max_frequency.insert(-frequency);
}

// Correctly INCREMENTS an element's frequency
void increment(int element,
               map<int, int> &element_to_frequency,
               map<int, multiset<int>> &frequency_to_multiset,
               multiset<int> &max_frequency)
{
    int old_frequency = element_to_frequency[element]; // Defaults to 0 if not present
    remove_from_bucket(element, old_frequency, frequency_to_multiset, max_frequency);

    int new_frequency = old_frequency + 1;
    element_to_frequency[element] = new_frequency;
    add_to_bucket(element, new_frequency, frequency_to_multiset, max_frequency);
}

// Correctly DECREMENTS an element's frequency
void decrement(int element,
               map<int, int> &element_to_frequency,
               map<int, multiset<int>> &frequency_to_multiset,
               multiset<int> &max_frequency)
{
    int old_frequency = element_to_frequency[element];
    remove_from_bucket(element, old_frequency, frequency_to_multiset, max_frequency);

    int new_frequency = old_frequency - 1;
    element_to_frequency[element] = new_frequency;
    if (new_frequency > 0)
    {
        add_to_bucket(element, new_frequency, frequency_to_multiset, max_frequency);
    }
    else
    {
        // If frequency becomes 0, remove it completely
        element_to_frequency.erase(element);
    }
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int num_elements, window_size;
    scanf("%d %d", &num_elements, &window_size);

    vector<int> array(num_elements);
    for (int i = 0; i < num_elements; i++)
    {
        scanf("%d", &array[i]);
    }

    map<int, int> element_to_frequency;
    map<int, multiset<int>> frequency_to_multiset;
    multiset<int> max_frequency; // Stores negative frequencies to find max easily
    vector<int> result;

    for (int i = 0; i < num_elements; i++)
    {
        // 1. Add the new (right) element to the window
        increment(array[i], element_to_frequency, frequency_to_multiset, max_frequency);

        // 2. If the window is full, process it
        if (i >= window_size - 1)
        {
            // Find the maximum frequency. It's the first element because we store negatives.
            int max_freq = -(*max_frequency.begin());

            // Find the smallest element with that frequency. It's the first in the multiset.
            result.push_back(*frequency_to_multiset[max_freq].begin());

            // 3. Remove the (left) element that is sliding out of the window
            int left_element = array[i - (window_size - 1)];
            decrement(left_element, element_to_frequency, frequency_to_multiset, max_frequency);
        }
    }

    for (auto x : result)
    {
        printf("%d ", x);
    }

    return 0;
}