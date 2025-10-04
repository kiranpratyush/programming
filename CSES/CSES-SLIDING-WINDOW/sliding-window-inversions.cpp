#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

// Fenwick Tree (or Binary Indexed Tree) implementation
// It supports point updates and prefix sum queries in O(log N) time.
// We use 1-based indexing for the tree.
struct FenwickTree {
    std::vector<int> bit;
    int size;

    FenwickTree(int sz) : size(sz), bit(sz + 1, 0) {}

    // Adds 'delta' to the element at index 'idx'.
    void add(int idx, int delta) {
        for (; idx <= size; idx += idx & -idx) {
            bit[idx] += delta;
        }
    }

    // Queries the sum of elements in the range [1, idx].
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += bit[idx];
        }
        return sum;
    }
};

int main() {
    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    fast_io();
    
    int n;
    long long k;
    std::cin >> n >> k;

    std::vector<int> x(n);
    std::vector<int> sorted_unique_vals;
    for (int i = 0; i < n; ++i) {
        std::cin >> x[i];
        sorted_unique_vals.push_back(x[i]);
    }

    // --- Step 1: Coordinate Compression ---
    // The values can be up to 10^9, which is too large for a Fenwick tree's indices.
    // We map each unique value to a smaller integer in the range [1, m],
    // where m is the number of unique values.
    std::sort(sorted_unique_vals.begin(), sorted_unique_vals.end());
    sorted_unique_vals.erase(std::unique(sorted_unique_vals.begin(), sorted_unique_vals.end()), sorted_unique_vals.end());

    std::vector<int> compressed_x(n);
    for (int i = 0; i < n; ++i) {
        // Find the position of x[i] in the sorted unique list and add 1 for 1-based indexing.
        compressed_x[i] = std::lower_bound(sorted_unique_vals.begin(), sorted_unique_vals.end(), x[i]) - sorted_unique_vals.begin() + 1;
    }

    int max_val = sorted_unique_vals.size();
    FenwickTree bit(max_val);
    long long current_inversions = 0;

    // --- Step 2: Calculate Inversions for the First Window ---
    for (int i = 0; i < k; ++i) {
        int val = compressed_x[i];
        // An inversion is a pair (a, b) where index(a) < index(b) but value(a) > value(b).
        // For the current element `val`, we count how many elements already in the window are larger than it.
        // Number of elements processed so far = i
        // Number of elements seen that are <= val = bit.query(val)
        // Number of elements seen that are > val = i - bit.query(val)
        current_inversions += i - bit.query(val);
        bit.add(val, 1);
    }

    // Print the result for the first window
    std::cout << current_inversions;

    // --- Step 3: Slide the Window ---
    for (int i = k; i < n; ++i) {
        // The element leaving the window from the left
        int elem_out = compressed_x[i - k];
        // The new element entering the window from the right
        int elem_in = compressed_x[i];

        // --- Simplified Update Logic ---
        
        // 1. Remove inversions involving the outgoing element.
        // `elem_out` was the leftmost element in the previous window. It formed inversions
        // with any element to its right that was smaller. The number of such elements
        // is exactly bit.query(elem_out - 1). We subtract these lost inversions.
        current_inversions -= bit.query(elem_out - 1);
        
        // Update the Fenwick tree by removing the count of the outgoing element.
        bit.add(elem_out, -1);

        // 2. Add inversions involving the incoming element.
        // `elem_in` is the rightmost element in the new window. It forms inversions
        // with any element to its left (now in the window) that is larger.
        // The number of such elements is (k-1) - bit.query(elem_in).
        current_inversions += (k - 1) - bit.query(elem_in);

        // Update the Fenwick tree by adding the count of the incoming element.
        bit.add(elem_in, 1);

        std::cout << " " << current_inversions;
    }
    std::cout << std::endl;

    return 0;
}
