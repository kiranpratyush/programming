#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

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
 * @param k The maximum power of 2 needed (log2(n)).
 */
void preprocess_for_lca(int node, int parent, const vector<vector<int>> &tree, vector<vector<int>> &up, vector<int> &in, vector<int> &out, int k) {
    in[node] = ++timer_lca;
    up[node][0] = parent;
    for (int i = 1; i <= k; i++) {
        up[node][i] = up[up[node][i - 1]][i - 1];
    }

    for (int child : tree[node]) {
        if (child != parent) {
            preprocess_for_lca(child, node, tree, up, in, out, k);
        }
    }

    out[node] = ++timer_lca;
}

/**
 * @brief Checks if node u is an ancestor of node v.
 * @return True if u is an ancestor of v, false otherwise.
 */
bool is_ancestor(int u, int v, const vector<int> &in, const vector<int> &out) {
    return in[u] <= in[v] && out[u] >= out[v];
}

/**
 * @brief Finds the Lowest Common Ancestor (LCA) of two nodes u and v.
 * @return The LCA of u and v.
 */
int lca(int u, int v, const vector<int> &in, const vector<int> &out, const vector<vector<int>> &up, int k) {
    if (is_ancestor(u, v, in, out)) return u;
    if (is_ancestor(v, u, in, out)) return v;

    for (int i = k; i >= 0; i--) {
        if (!is_ancestor(up[u][i], v, in, out)) {
            u = up[u][i];
        }
    }

    return up[u][0];
}

// --- Part 2: Heavy-Light Decomposition ---

/**
 * @brief A DFS to compute subtree sizes and identify the heavy child for each node.
 */
int subtree_calculation_dfs(int node, int parent, const vector<vector<int>> &tree, vector<int> &heavy, vector<int> &subtree_size, vector<int>& parent_map) {
    subtree_size[node] = 1;
    parent_map[node] = parent;
    int max_child_size = 0;

    for (int child : tree[node]) {
        if (child != parent) {
            int child_subtree_size = subtree_calculation_dfs(child, node, tree, heavy, subtree_size, parent_map);
            subtree_size[node] += child_subtree_size;
            if (child_subtree_size > max_child_size) {
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
void hld_dfs(int node, int parent, const vector<vector<int>> &tree, vector<int> &pos, vector<int> &chain_head, const vector<int> &heavy) {
    pos[node] = ++timer_hld; 

    if (heavy[node] != -1) {
        chain_head[heavy[node]] = chain_head[node];
        hld_dfs(heavy[node], node, tree, pos, chain_head, heavy);
    }

    for (int child : tree[node]) {
        if (child != parent && child != heavy[node]) {
            chain_head[child] = child;
            hld_dfs(child, node, tree, pos, chain_head, heavy);
        }
    }
}

// --- Part 3: Segment Tree for Path Queries ---

/**
 * @brief Builds the segment tree from the base array.
 */
void build_segment_tree(const vector<int>& base_arr, vector<int>& seg_tree, int tree_idx, int start, int end) {
    if (start == end) {
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
int query_segment_tree(const vector<int>& seg_tree, int tree_idx, int start, int end, int q_start, int q_end) {
    if (start > q_end || end < q_start) {
        return 0; // Use 0 for max queries, can be -infinity
    }
    if (start >= q_start && end <= q_end) {
        return seg_tree[tree_idx];
    }
    int mid = start + (end - start) / 2;
    int left_max = query_segment_tree(seg_tree, 2 * tree_idx, start, mid, q_start, q_end);
    int right_max = query_segment_tree(seg_tree, 2 * tree_idx + 1, mid + 1, end, q_start, q_end);
    return max(left_max, right_max);
}

// --- Part 4: HLD Path Query (LCA-based method) ---

/**
 * @brief Helper function to query the path from node 'u' up to its 'ancestor'.
 */
int query_up(int u, int ancestor, int n, const vector<int>& seg_tree, const vector<int>& pos, const vector<int>& chain_head, const vector<int>& parent_map) {
    int result = 0;
    while (chain_head[u] != chain_head[ancestor]) {
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
              const vector<int>& seg_tree, const vector<int>& pos, const vector<int>& chain_head, const vector<int>& parent_map,
              // LCA-specific parameters
              const vector<int>& in, const vector<int>& out, const vector<vector<int>>& up, int k)
{
    int common_ancestor = lca(u, v, in, out, up, k);
    
    int max_u_to_lca = query_up(u, common_ancestor, n, seg_tree, pos, chain_head, parent_map);
    int max_v_to_lca = query_up(v, common_ancestor, n, seg_tree, pos, chain_head, parent_map);

    return max(max_u_to_lca, max_v_to_lca);
}


int main() {
    // --- Setup ---
    int n = 9; // Number of nodes
    vector<vector<int>> tree(n + 1);
    vector<pair<int, int>> edges = {{1, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}, {3, 7}, {4, 8}, {4, 9}};
    for (const auto& edge : edges) {
        tree[edge.first].push_back(edge.second);
        tree[edge.second].push_back(edge.first);
    }
    vector<int> node_values = {0, 10, 5, 20, 8, 12, 15, 30, 2, 25}; // 1-indexed

    int root = 1;
    int k = ceil(log2(n));

    // --- Data Structures Initialization ---
    vector<vector<int>> up(n + 1, vector<int>(k + 1));
    vector<int> in_time(n + 1), out_time(n + 1);
    vector<int> parent_map(n + 1, 0);
    
    vector<int> heavy(n + 1, -1);
    vector<int> subtree_size(n + 1, 0);
    
    vector<int> chain_head(n + 1);
    vector<int> pos_in_base_array(n + 1);

    // --- Execution ---
    preprocess_for_lca(root, root, tree, up, in_time, out_time, k);
    subtree_calculation_dfs(root, root, tree, heavy, subtree_size, parent_map);
    chain_head[root] = root;
    hld_dfs(root, root, tree, pos_in_base_array, chain_head, heavy);

    // --- Build Segment Tree ---
    vector<int> base_array(n + 1);
    for (int i = 1; i <= n; ++i) {
        base_array[pos_in_base_array[i]] = node_values[i];
    }
    
    vector<int> segment_tree(4 * n, 0);
    build_segment_tree(base_array, segment_tree, 1, 1, n);

    // --- Output and Verification ---
    cout << "Heavy-Light Decomposition Results:" << endl;
    cout << "----------------------------------" << endl;
    cout << "Node\tValue\tChain Head\tPosition" << endl;
    for (int i = 1; i <= n; ++i) {
        cout << i << "\t"
             << node_values[i] << "\t"
             << chain_head[i] << "\t\t"
             << pos_in_base_array[i] << endl;
    }
    
    // --- Path Query Demonstration ---
    cout << "\n--- Path Queries (LCA-based method) ---" << endl;
    
    // Query path from 6 to 7. Path is 6-2-1-3-7. Values: {15, 5, 10, 20, 30}. Max should be 30.
    int u = 6, v = 7;
    int max_val = query_path(u, v, n, segment_tree, pos_in_base_array, chain_head, parent_map, in_time, out_time, up, k);
    cout << "Max value on path (" << u << " -> " << v << "): " << max_val << endl;

    // Query path from 5 to 9. Path is 5-2-1-4-9. Values: {12, 5, 10, 8, 25}. Max should be 25.
    u = 5, v = 9;
    max_val = query_path(u, v, n, segment_tree, pos_in_base_array, chain_head, parent_map, in_time, out_time, up, k);
    cout << "Max value on path (" << u << " -> " << v << "): " << max_val << endl;

    return 0;
}
