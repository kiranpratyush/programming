#include <cstdio>
#include <set>
#include <vector>
using namespace std;

/*
1. Main idea : Keep the data structure (set) at each node
2. Merge process: Always merge the smaller one to larger one
3. Here for set the swap of c++ is O(1) complexity
4. Let the size of the smaller set is x , after merging the size of the resultant
set will be atleast 2x (this is for each merge) , if it merged 2nd time it will be
2*2x like this

let's say we merged y times total size will be 2^y*x or 2^y

Now the total size is always less than N ( number of nodes)

2^y < N
y<log2(N)

now for each node merging log(N)*log(N) this is for total number of merging
and then for N node : N*log(N)^2 Time complexity
*/

void dfs(int curr_node, int parent_node, vector<vector<int>> &tree, vector<set<int>> &colors, vector<int> &result)
{
    for (auto x : tree[curr_node])
    {
        if (x != parent_node)
        {
            dfs(x, curr_node, tree, colors, result);
            if (colors[curr_node].size() < colors[x].size())
            {
                swap(colors[curr_node], colors[x]);
                // now the curr_node has larger values and x has smaller values
                // always merge from smaller values to the larger values
            }
            for (auto y : colors[x])
            {
                colors[curr_node].insert(y);
            }
        }
    }
    result[curr_node] = colors[curr_node].size();
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int num_nodes;
    scanf("%d", &num_nodes);
    vector<set<int>> colors(num_nodes + 1);
    for (int i = 1; i <= num_nodes; i++)
    {
        int color;
        scanf("%d", &color);
        colors[i].insert(color);
    }
    vector<vector<int>> tree(num_nodes + 1);
    for (int i = 1; i < num_nodes; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }
    vector<int> result(num_nodes + 1);
    dfs(1, 1, tree, colors, result);
    for (int i = 1; i <= num_nodes; i++)
    {
        printf("%d ", result[i]);
    }
}