#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

/*
The recursive implementation of the segment tree is slow
1.Learn about the iterative implementation of the segment tree again
*/

// Timers for DFS traversals
int timer_lca = 0; // For LCA pre-computation (in/out times)
int timer_hld = 0; // For HLD (flattened position)

// --- Part 1: Preprocessing for LCA using Binary Lifting ---

/**
 * @brief Performs a DFS traversal to compute in/out times and build the 'up' table for LCA.
 * @param node The current node being processed.
 * @param parent The parent of the current node.
 * @param tree The adjacency list representation of the tree.
 * @param up The binary lifting table where up[i][j] is the 2^j-th ancestor of node i.
 * @param in The entry time of a node in the DFS traversal.
 * @param out The exit time of a node in the DFS traversal.
 * @param k The maximum power of 2 needed ceil(log2(n)).
 */
void preprocess_for_lca(int node, int parent, const vector<vector<int>> &tree, vector<vector<int>> &up, vector<int> &in, vector<int> &out, int k)
{
    in[node] = ++timer_lca;
    up[node][0] = parent;
    for (int i = 1; i <= k; i++)
    {
        up[node][i] = up[up[node][i - 1]][i - 1];
    }

    for (int child : tree[node])
    {
        if (child != parent)
        {
            preprocess_for_lca(child, node, tree, up, in, out, k);
        }
    }

    out[node] = ++timer_lca;
}

/**
 * @brief Checks if node u is an ancestor of node v.
 * @return True if u is an ancestor of v, false otherwise.
 */
bool is_ancestor(int u, int v, const vector<int> &in, const vector<int> &out)
{
    return in[u] <= in[v] && out[u] >= out[v];
}

/**
 * @brief Finds the Lowest Common Ancestor (LCA) of two nodes u and v.
 * @return The LCA of u and v.
 */
int lca(int u, int v, const vector<int> &in, const vector<int> &out, const vector<vector<int>> &up, int k)
{
    if (is_ancestor(u, v, in, out))
        return u;
    if (is_ancestor(v, u, in, out))
        return v;

    for (int i = k; i >= 0; i--)
    {
        if (!is_ancestor(up[u][i], v, in, out))
        {
            u = up[u][i];
        }
    }

    return up[u][0];
}

// --- Part 2: Heavy-Light Decomposition ---

/**
 * @brief A DFS to compute subtree sizes and identify the heavy child for each node.
 */
int subtree_calculation_dfs(int node, int parent, const vector<vector<int>> &tree, vector<int> &heavy, vector<int> &subtree_size, vector<int> &parent_map)
{
    subtree_size[node] = 1;
    parent_map[node] = parent;
    int max_child_size = 0;

    for (int child : tree[node])
    {
        if (child != parent)
        {
            int child_subtree_size = subtree_calculation_dfs(child, node, tree, heavy, subtree_size, parent_map);
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
int subtree_calculation_dfs2(int node, int parent, int d, const vector<vector<int>> &tree, vector<int> &heavy, vector<int> &subtree_size, vector<int> &parent_map, vector<int> &depth)
{
    subtree_size[node] = 1;
    parent_map[node] = parent;
    depth[node] = d; // Store the depth
    int max_child_size = 0;

    for (int child : tree[node])
    {
        if (child != parent)
        {
            // Pass d + 1 for the child's depth
            int child_subtree_size = subtree_calculation_dfs2(child, node, d + 1, tree, heavy, subtree_size, parent_map, depth);
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
 */
void hld_dfs(int node, int parent, const vector<vector<int>> &tree, vector<int> &pos, vector<int> &chain_head, const vector<int> &heavy)
{
    pos[node] = ++timer_hld;

    if (heavy[node] != -1)
    {
        chain_head[heavy[node]] = chain_head[node];
        hld_dfs(heavy[node], node, tree, pos, chain_head, heavy);
    }

    for (int child : tree[node])
    {
        if (child != parent && child != heavy[node])
        {
            chain_head[child] = child;
            hld_dfs(child, node, tree, pos, chain_head, heavy);
        }
    }
}

// --- Part 3: Segment Tree for Path Queries ---

/**
 * @brief Builds the segment tree from the base array.
 */
void build_segment_tree(const vector<int> &base_arr, vector<int> &seg_tree, int tree_idx, int start, int end)
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
 */
int query_segment_tree(const vector<int> &seg_tree, int tree_idx, int start, int end, int q_start, int q_end)
{
    if (start > q_end || end < q_start)
    {
        return 0; // Use 0 for max queries, can be -infinity
    }
    if (start >= q_start && end <= q_end)
    {
        return seg_tree[tree_idx];
    }
    int mid = start + (end - start) / 2;
    int left_max = query_segment_tree(seg_tree, 2 * tree_idx, start, mid, q_start, q_end);
    int right_max = query_segment_tree(seg_tree, 2 * tree_idx + 1, mid + 1, end, q_start, q_end);
    return max(left_max, right_max);
}

/**
 * @brief Point updates the segment tree
 */
void update_segment_tree(vector<int> &seg_tree, int tree_idx, int update_index, int update_value, int start, int end)
{
    if (start == end)
    {
        if (start == update_index)
        {
            seg_tree[tree_idx] = update_value;
            return;
        }
    }
    int mid = (start + end) / 2;
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

// --- Part 4: HLD Path Query (LCA-based method) ---

/**
 * @brief Helper function to query the path from node 'u' up to its 'ancestor'.
 */
int query_up(int u, int ancestor, int n, const vector<int> &seg_tree, const vector<int> &pos, const vector<int> &chain_head, const vector<int> &parent_map)
{
    int result = 0;
    while (chain_head[u] != chain_head[ancestor])
    {
        result = max(result, query_segment_tree(seg_tree, 1, 1, n, pos[chain_head[u]], pos[u]));
        u = parent_map[chain_head[u]];
    }
    // Final query on the same chain
    result = max(result, query_segment_tree(seg_tree, 1, 1, n, pos[ancestor], pos[u]));
    return result;
}

/**
 * @brief Queries the maximum value on the path between nodes u and v using your proposed LCA method.
 */
int query_path(int u, int v, int n,
               const vector<int> &seg_tree, const vector<int> &pos, const vector<int> &chain_head, const vector<int> &parent_map,
               // LCA-specific parameters
               const vector<int> &in, const vector<int> &out, const vector<vector<int>> &up, int k)
{
    int common_ancestor = lca(u, v, in, out, up, k);

    int max_u_to_lca = query_up(u, common_ancestor, n, seg_tree, pos, chain_head, parent_map);
    int max_v_to_lca = query_up(v, common_ancestor, n, seg_tree, pos, chain_head, parent_map);

    return max(max_u_to_lca, max_v_to_lca);
}
/**
 * @brief Queries the maximum value on the path between u and v using only HLD.
 * @return The maximum value on the path.
 */
int query_path_hld(int u, int v, int n,
                   const vector<int> &seg_tree, const vector<int> &pos,
                   const vector<int> &chain_head, const vector<int> &parent_map,
                   const vector<int> &depth)
{
    int result = 0;

    // Loop until u and v are on the same heavy chain
    while (chain_head[u] != chain_head[v])
    {
        // We want to move the node that is on a "deeper" chain.
        // We check the depth of the chain heads.
        if (depth[chain_head[u]] < depth[chain_head[v]])
        {
            swap(u, v);
        }
        // Query the segment from the head of u's chain to u
        result = max(result, query_segment_tree(seg_tree, 1, 1, n, pos[chain_head[u]], pos[u]));
        // Move u to the parent of its chain's head, effectively jumping to the next chain up
        u = parent_map[chain_head[u]];
    }

    // Now u and v are on the same chain. The path between them is contiguous.
    // The node with the smaller position is the ancestor.
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
    int num_nodes, num_queries;
    scanf("%d %d", &num_nodes, &num_queries);
    vector<vector<int>> tree(num_nodes + 1);
    vector<int> node_values(num_nodes + 1);
    for (int i = 1; i <= num_nodes; i++)
    {
        scanf("%d", &node_values[i]);
    }
    for (int i = 1; i < num_nodes; i++)
    {
        int start_node, end_node;
        scanf("%d %d", &start_node, &end_node);
        tree[start_node].push_back(end_node);
        tree[end_node].push_back(start_node);
    }
    int root = 1;
    int k = ceil(log2(num_nodes));

    // --- Data Structures Initialization ---
    vector<vector<int>> up(num_nodes + 1, vector<int>(k + 1));
    vector<int> in_time(num_nodes + 1), out_time(num_nodes + 1);
    vector<int> parent_map(num_nodes + 1, 0);
    vector<int> depth(num_nodes + 1, 0);

    vector<int> heavy(num_nodes + 1, -1);
    vector<int> subtree_size(num_nodes + 1, 0);

    vector<int> chain_head(num_nodes + 1);
    vector<int> pos_in_base_array(num_nodes + 1);

    vector<int> final_result;

    // --- Execution ---
    // preprocess_for_lca(root, root, tree, up, in_time, out_time, k);
    // subtree_calculation_dfs(root, root, tree, heavy, subtree_size, parent_map);
    subtree_calculation_dfs2(root, root, 0, tree, heavy, subtree_size, parent_map, depth);
    chain_head[root] = root;
    hld_dfs(root, root, tree, pos_in_base_array, chain_head, heavy);

    // --- Build Segment Tree ---
    vector<int> base_array(num_nodes + 1);
    for (int i = 1; i <= num_nodes; ++i)
    {
        base_array[pos_in_base_array[i]] = node_values[i];
    }

    vector<int> segment_tree(4 * num_nodes, 0);
    build_segment_tree(base_array, segment_tree, 1, 1, num_nodes);

    for (int i = 1; i <= num_queries; i++)
    {
        int query_type, start, end;
        scanf("%d %d %d", &query_type, &start, &end);
        if (query_type == 1)
        {
            int position = pos_in_base_array[start];
            update_segment_tree(segment_tree, 1, position, end, 1, num_nodes);
        }
        else
        {
            int result = query_path_hld(start, end, num_nodes, segment_tree, pos_in_base_array, chain_head, parent_map, depth);
            final_result.push_back(result);
        }
    }
    for (auto x : final_result)
    {
        printf("%d ", x);
    }
    return 0;
}
