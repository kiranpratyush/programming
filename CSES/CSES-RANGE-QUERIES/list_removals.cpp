#include <cstdio>
#include <vector>
using namespace std;

void update(vector<int> &tree, int max_size, int index, int value)
{
    while (index <= max_size)
    {
        tree[index] += value;
        index += (index & -index);
    }
}

int read(vector<int> &tree, int index)
{
    int value = 0;
    while (index >= 1)
    {
        value += tree[index];
        index -= (index & -index);
    }
    return value;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int total_integers;
    scanf("%d", &total_integers);

    vector<int> array(total_integers);
    vector<int> tree(total_integers + 2, 0); // +2 for safety in 1-based index

    for (int i = 0; i < total_integers; i++)
    {
        scanf("%d", &array[i]);
        update(tree, total_integers, i + 1, 1); // Mark each as alive
    }

    for (int i = 0; i < total_integers; i++)
    {
        int index;
        scanf("%d", &index); // 1-based position to remove

        int low = 1, high = total_integers;
        while (low <= high)
        {
            int mid = (low + high) / 2;
            int static_sum = read(tree, mid);
            int prev_static_sum = read(tree, mid - 1);
            if (static_sum == index && prev_static_sum != index)
            {
                printf("%d ", array[mid - 1]);         // array is 0-based
                update(tree, total_integers, mid, -1); // Remove this
                break;
            }
            else if (static_sum >= index)
            {
                high = mid - 1;
            }
            else
            {
                low = mid + 1;
            }
        }
    }

    return 0;
}
