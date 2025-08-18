#include <cstdio>
#include <vector>
#include <algorithm> // For std::max and std::fill
using namespace std;

#define MAX_POSSIBLE_NODES 200001

/*
 * ALGORITHM EXPLANATION:
 * This program solves the "Fixed-Length Paths II" problem using Centroid Decomposition.
 * The core idea is to count paths of length between `a` and `b`.
 *
 * 1.  Centroid Decomposition: We recursively break down the tree. At each step, we find the
 * "centroid" (a node that, when removed, splits the tree into small subtrees). We then
 * count only the paths that pass *through* this centroid.
 *
 * 2.  Counting Paths at a Centroid: For a centroid `c`, a path through it is formed by two nodes
 * `u` and `v` in different child subtrees. If `dist(u, c) = d1` and `dist(v, c) = d2`,
 * the total path length is `d1 + d2`. We need to find pairs where `a <= d1 + d2 <= b`.
 *
 * 3.  O(1) Sliding Window Sum: A naive solution would be too slow. We process the centroid's
 * child subtrees one by one. For each node in the *current* subtree, we need to count valid
 * "partner" nodes in the *previously processed* subtrees. Instead of re-calculating this
 * count every time, we use a sliding window sum. As we consider nodes at depth `d`, then
 * `d+1`, the required range of partner depths shifts by one. We can update the sum of
 * partners in O(1) by adding the new element that enters the range and subtracting the
 * one that leaves. This keeps the work at each centroid linear, for a total complexity of O(N log N).
 */

// --- Global Variables ---
int num_nodes, start_range, end_range;
long long answer = 0;

// `total_count[d]` stores the number of nodes at depth `d` from the current centroid,
// accumulated from all *previously processed* child subtrees.
int total_count[MAX_POSSIBLE_NODES];

// `cnt[d]` is a temporary array to store node counts for the *single* child subtree
// we are currently processing.
int cnt[MAX_POSSIBLE_NODES];

// `subtree_depth` is the max depth in the current child subtree.
// `max_depth` is the max depth found across all children of the current centroid, used for cleanup.
int subtree_depth, max_depth;

vector<int> tree[MAX_POSSIBLE_NODES];
int subtree_sizes[MAX_POSSIBLE_NODES];
bool processed[MAX_POSSIBLE_NODES]; // Marks centroids that have been processed.

/**
 * @brief Calculates the size of each subtree in the current component.
 * @param node The current node.
 * @param parent The parent node (to prevent going backwards).
 * This is used to find the centroid. The `!processed[child]` check ensures we only
 * traverse the part of the tree we are currently decomposing.
 */
int calculate_subtree_sizes(int node, int parent)
{
    subtree_sizes[node] = 1;
    for (auto child : tree[node])
    {
        if (child != parent && !processed[child])
        {
            subtree_sizes[node] += calculate_subtree_sizes(child, node);
        }
    }
    return subtree_sizes[node];
}

/**
 * @brief Finds the centroid of the current component.
 * @param desired The desired size for subtrees (half of the component size).
 * @param node The current node.
 * @param parent The parent node.
 * A centroid is a node whose largest remaining subtree is at most half the component size.
 */
int get_centroid(int desired, int node, int parent)
{
    for (int i : tree[node])
        if (!processed[i] && i != parent && subtree_sizes[i] >= desired)
            return get_centroid(desired, i, node);
    return node;
}

/**
 * @brief Performs a DFS from a child of the centroid to calculate path lengths.
 * @param node The current node in the DFS.
 * @param parent The parent node in the DFS.
 * @param depth The current path length from the centroid.
 * Fills the temporary `cnt` array with counts of nodes at each depth.
 */
void calculate_depth_count(int node, int parent, int depth = 1)
{
    if (depth > end_range)
    {
        return;
    }
    subtree_depth = max(subtree_depth, depth);
    cnt[depth]++;
    for (auto child : tree[node])
    {
        if (child != parent && !processed[child])
        {
            calculate_depth_count(child, node, depth + 1);
        }
    }
}

/**
 * @brief The main recursive function for Centroid Decomposition.
 * @param node The entry node of the current component to be decomposed.
 * @param parent The parent of the entry node from the previous decomposition level (use 0 for root).
 */
void solve(int node, int parent)
{
    // 1. Find the centroid `c` for the current component.
    int component_size = calculate_subtree_sizes(node, parent);
    int c = get_centroid(component_size >> 1, node, parent);

    // --- Start processing paths through centroid `c` ---
    processed[c] = true;
    total_count[0] = 1; // The centroid itself is at depth 0.
    max_depth = 0;

    // This variable holds the sum of valid partners for a node at depth 1.
    // It's updated after each child to include that child's contributions for the *next* child.
    long long initial_sum = (start_range == 1) ? 1ll : 0ll;

    // 2. Iterate through each child of the centroid.
    for (auto child : tree[c])
    {
        if (!processed[child])
        {
            // This is the sliding window sum. It's initialized for depth 1.
            long long sum = initial_sum;
            subtree_depth = 0;

            // 2a. Get all path lengths from `c` for this specific child's subtree.
            calculate_depth_count(child, c);

            // 2b. Use the sliding window to count paths between the current child
            //     and all previously processed children.
            for (int depth = 1; depth <= subtree_depth; depth++)
            {
                // `sum` holds the number of valid partners for a node at `depth`.
                // `cnt[depth]` is the number of nodes at this `depth`.
                answer += cnt[depth] * sum;

                // Update the sliding window sum for the *next* depth (`depth + 1`).
                // A partner for `depth` was in range `[a-d, b-d]`.
                // A partner for `depth+1` will be in range `[a-(d+1), b-(d+1)]`.

                // Remove the element that falls off the end of the window.
                int dremove = end_range - depth;
                if (dremove >= 0)
                    sum -= total_count[dremove];

                // Add the new element that enters the window.
                int dadd = start_range - (depth + 1);
                if (dadd >= 0)
                    sum += total_count[dadd];
            }

            // 2c. Update `initial_sum` with the current child's data. This prepares it
            //     for the next child in the loop.
            for (int depth = start_range - 1; depth <= end_range - 1 && depth <= subtree_depth; depth++)
                initial_sum += cnt[depth];

            // 2d. Merge the current child's counts into `total_count` so it can be
            //     used by subsequent children.
            for (int depth = 1; depth <= subtree_depth; depth++)
                total_count[depth] += cnt[depth];

            max_depth = max(max_depth, subtree_depth);

            // 2e. Clear the temporary `cnt` array for the next child.
            fill(cnt, cnt + subtree_depth + 1, 0);
        }
    }

    // 3. Cleanup: Reset `total_count` for the next level of decomposition.
    fill(total_count + 1, total_count + max_depth + 1, 0);

    // 4. Recurse on the smaller, unprocessed components.
    for (auto child : tree[c])
    {
        if (!processed[child])
        {
            // The new parent is the centroid we just processed.
            solve(child, c);
        }
    }
}

int main()
{
    scanf("%d %d %d", &num_nodes, &start_range, &end_range);
    for (int i = 1; i < num_nodes; i++)
    {
        int start, end;
        scanf("%d %d", &start, &end);
        tree[start].push_back(end);
        tree[end].push_back(start);
    }

    // Initial call to start the decomposition.
    // The parent is 0, a non-existent node, to ensure the whole tree is processed.
    solve(1, 0);
    printf("%lld\n", answer);

    return 0;
}