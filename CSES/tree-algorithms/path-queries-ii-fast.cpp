#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Use the std namespace for cin, cout, etc.
using namespace std;

// Timer for HLD path decomposition
int timer_hld = 0;

// --- Part 1: Heavy-Light Decomposition Preprocessing ---

/**
 * @brief A DFS to compute subtree sizes, depths, and identify the heavy child for each node.
 * @param node The current node being processed.
 * @param parent The parent of the current node.
 * @param d The depth of the current node.
 * @param tree The adjacency list representation of the tree.
 * @param heavy Output vector to store the heavy child of each node.
 * @param subtree_size Output vector to store the size of the subtree for each node.
 * @param parent_map Output vector to store the parent of each node.
 * @param depth Output vector to store the depth of each node.
 * @return The size of the subtree rooted at 'node'.
 */
int subtree_calculation_dfs(int node, int parent, int d, const vector<vector<int>> &tree, vector<int> &heavy, vector<int> &subtree_size, vector<int> &parent_map, vector<int> &depth)
{
    subtree_size[node] = 1;
    parent_map[node] = parent;
    depth[node] = d;
    int max_child_size = 0;

    for (int child : tree[node])
    {
        if (child != parent)
        {
            int child_subtree_size = subtree_calculation_dfs(child, node, d + 1, tree, heavy, subtree_size, parent_map, depth);
            subtree_size[node] += child_subtree_size;
            if (child_subtree_size > max_child_size)
            {
                heavy[node] = child;
                max_child_size = child_subtree_size;
            }
        }
    }
    return subtree_size[node];
}

/**
 * @brief The main HLD function. Decomposes the tree into heavy and light paths.
 * Assigns a position in the flattened array to each node.
 * @param node The current node being processed.
 * @param parent The parent of the current node.
 * @param tree The adjacency list representation of the tree.
 * @param pos Output vector storing the position of each node in the flattened array.
 * @param chain_head Output vector storing the head of the heavy chain for each node.
 * @param heavy The precomputed heavy child for each node.
 */
void hld_dfs(int node, int parent, const vector<vector<int>> &tree, vector<int> &pos, vector<int> &chain_head, const vector<int> &heavy)
{
    pos[node] = ++timer_hld;

    // Process the heavy child first to keep the heavy path contiguous
    if (heavy[node] != -1)
    {
        chain_head[heavy[node]] = chain_head[node];
        hld_dfs(heavy[node], node, tree, pos, chain_head, heavy);
    }

    // Process light children. Each starts a new chain.
    for (int child : tree[node])
    {
        if (child != parent && child != heavy[node])
        {
            chain_head[child] = child;
            hld_dfs(child, node, tree, pos, chain_head, heavy);
        }
    }
}

// --- Part 2: Segment Tree for Path Queries ---

/**
 * @brief Builds the segment tree from the base array.
 * @param base_arr The flattened array of node values.
 * @param seg_tree The segment tree to be built.
 * @param tree_idx The current index in the segment tree.
 * @param start The start index of the current segment in base_arr.
 * @param end The end index of the current segment in base_arr.
 */
void build_segment_tree(const vector<int> &base_arr, vector<long long> &seg_tree, int tree_idx, int start, int end)
{
    if (start == end)
    {
        seg_tree[tree_idx] = base_arr[start];
        return;
    }
    int mid = start + (end - start) / 2;
    build_segment_tree(base_arr, seg_tree, 2 * tree_idx, start, mid);
    build_segment_tree(base_arr, seg_tree, 2 * tree_idx + 1, mid + 1, end);
    seg_tree[tree_idx] = max(seg_tree[2 * tree_idx], seg_tree[2 * tree_idx + 1]);
}

/**
 * @brief Queries the segment tree for the maximum value in a given range [q_start, q_end].
 * @return The maximum value in the queried range.
 */
long long query_segment_tree(const vector<long long> &seg_tree, int tree_idx, int start, int end, int q_start, int q_end)
{
    if (start > q_end || end < q_start)
    {
        return 0; // Return 0 as the identity for max operation on non-negative values
    }
    if (start >= q_start && end <= q_end)
    {
        return seg_tree[tree_idx];
    }
    int mid = start + (end - start) / 2;
    long long left_max = query_segment_tree(seg_tree, 2 * tree_idx, start, mid, q_start, q_end);
    long long right_max = query_segment_tree(seg_tree, 2 * tree_idx + 1, mid + 1, end, q_start, q_end);
    return max(left_max, right_max);
}

/**
 * @brief Updates a single value in the segment tree.
 * @param update_index The 1-based index in the base_array to update.
 * @param update_value The new value.
 */
void update_segment_tree(vector<long long> &seg_tree, int tree_idx, int update_index, int update_value, int start, int end)
{
    if (start == end)
    {
        seg_tree[tree_idx] = update_value;
        return;
    }
    int mid = start + (end - start) / 2;
    if (update_index <= mid)
    {
        update_segment_tree(seg_tree, 2 * tree_idx, update_index, update_value, start, mid);
    }
    else
    {
        update_segment_tree(seg_tree, 2 * tree_idx + 1, update_index, update_value, mid + 1, end);
    }
    seg_tree[tree_idx] = max(seg_tree[2 * tree_idx], seg_tree[2 * tree_idx + 1]);
}

// --- Part 3: HLD Path Query ---

/**
 * @brief Queries the maximum value on the path between nodes u and v using HLD.
 * @return The maximum value on the path.
 */
long long query_path_hld(int u, int v, int n,
                         const vector<long long> &seg_tree, const vector<int> &pos,
                         const vector<int> &chain_head, const vector<int> &parent_map,
                         const vector<int> &depth)
{
    long long result = 0;

    // Loop until u and v are on the same heavy chain
    while (chain_head[u] != chain_head[v])
    {
        // Move the node that is on a "deeper" chain up.
        // We determine this by comparing the depth of the chain heads.
        if (depth[chain_head[u]] < depth[chain_head[v]])
        {
            swap(u, v);
        }
        // Query the segment from the head of u's chain to u itself
        result = max(result, query_segment_tree(seg_tree, 1, 1, n, pos[chain_head[u]], pos[u]));
        // Jump to the parent of the chain's head, moving to the next chain up
        u = parent_map[chain_head[u]];
    }

    // Now u and v are on the same chain, so the path is contiguous in our segment tree.
    // The node with the smaller position is the ancestor of the other.
    if (pos[u] > pos[v])
    {
        swap(u, v);
    }
    result = max(result, query_segment_tree(seg_tree, 1, 1, n, pos[u], pos[v]));

    return result;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // --- Fast I/O Setup ---
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int num_nodes, num_queries;
    cin >> num_nodes >> num_queries;

    vector<vector<int>> tree(num_nodes + 1);
    vector<int> node_values(num_nodes + 1);
    for (int i = 1; i <= num_nodes; i++)
    {
        cin >> node_values[i];
    }
    for (int i = 1; i < num_nodes; i++)
    {
        int start_node, end_node;
        cin >> start_node >> end_node;
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }

    int root = 1;

    // --- Data Structures Initialization ---
    vector<int> parent_map(num_nodes + 1, 0);
    vector<int> depth(num_nodes + 1, 0);
    vector<int> heavy(num_nodes + 1, -1);
    vector<int> subtree_size(num_nodes + 1, 0);
    vector<int> chain_head(num_nodes + 1);
    vector<int> pos_in_base_array(num_nodes + 1);
    vector<int> final_result;

    // --- HLD Execution ---
    subtree_calculation_dfs(root, root, 0, tree, heavy, subtree_size, parent_map, depth);
    chain_head[root] = root;
    hld_dfs(root, root, tree, pos_in_base_array, chain_head, heavy);

    // --- Build Segment Tree ---
    vector<int> base_array(num_nodes + 1);
    for (int i = 1; i <= num_nodes; ++i)
    {
        base_array[pos_in_base_array[i]] = node_values[i];
    }

    vector<long long> segment_tree(4 * num_nodes, 0);
    build_segment_tree(base_array, segment_tree, 1, 1, num_nodes);

    // --- Process Queries ---
    for (int i = 0; i < num_queries; i++)
    {
        int query_type, start, end;
        cin >> query_type >> start >> end;
        if (query_type == 1)
        {
            int position = pos_in_base_array[start];
            update_segment_tree(segment_tree, 1, position, end, 1, num_nodes);
        }
        else
        {
            long long result = query_path_hld(start, end, num_nodes, segment_tree, pos_in_base_array, chain_head, parent_map, depth);
            final_result.push_back(result);
        }
    }
    for (auto x : final_result)
    {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
