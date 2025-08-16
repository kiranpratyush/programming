#include <algorithm>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct Interval {
    int start;
    int end;
    int index;
};

bool compare_by_end(const Interval &v1, const Interval &v2) {
    // Sort by end in ascending order (and by start if ends are equal)
    if (v1.end != v2.end)
        return v1.end < v2.end;
    return v1.start > v2.start;
}

bool compare_by_start(const Interval &v1, const Interval &v2) {
    // Sort by start in ascending order (and by end if starts are equal)
    if (v1.start != v2.start)
        return v1.start < v2.start;
    return v1.end >v2.end;
}

// Fenwick tree update: we pass the tree's size (which is the compressed range)
void update(vector<int>& tree, int index, int value, int tree_size) {
    while (index <= tree_size) {
        tree[index] += value;
        index += (index & -index);
    }
}

int read(vector<int>& tree, int index) {
    int value = 0;
    while (index > 0) {
        value += tree[index];
        index -= (index & -index);
    }
    return value;
}

int main() {
    // freopen("../input.txt", "r", stdin);
    // freopen("../output.txt", "w", stdout);
    
    int num_queries;
    scanf("%d", &num_queries);
    
    // Read intervals and insert coordinates into a set to get unique sorted values.
    vector<Interval> intervals(num_queries);
    set<int> coordSet;
    for (int i = 0; i < num_queries; i++) {
        int s, e;
        scanf("%d %d", &s, &e);
        intervals[i] = { s, e, i };
        coordSet.insert(s);
        coordSet.insert(e);
    }
    
    // Build the compression mapping (1-indexed) by iterating over the set.
    map<int, int> comp;
    int idx = 1;
    for (int coord : coordSet) {
        comp[coord] = idx++;
    }
    int comp_size = comp.size();
    
    // Replace interval endpoints with their compressed values.
    for (int i = 0; i < num_queries; i++) {
        intervals[i].start = comp[intervals[i].start];
        intervals[i].end = comp[intervals[i].end];
    }
    
    // Create two vectors for processing in different orders.
    vector<Interval> vector_by_end = intervals;
    vector<Interval> vector_by_start = intervals;
    sort(vector_by_end.begin(), vector_by_end.end(), compare_by_end);
    sort(vector_by_start.begin(), vector_by_start.end(), compare_by_start);
    
    // Allocate Fenwick trees with size equal to the number of unique coordinates.
    vector<int> tree_by_start(comp_size + 1, 0);
    vector<int> tree_by_end(comp_size + 1, 0);
    vector<int> result_by_start(num_queries, 0);
    vector<int> result_by_end(num_queries, 0);
    
    int count_by_end = 0;
    int count_by_start = 0;
    
    // Process intervals sorted by end.
    for (int i = 0; i < num_queries; i++) {
        Interval temp = vector_by_end[i];
        // Count how many intervals with a lower start value have been seen.
        int count = count_by_end - read(tree_by_end, temp.start-1);
        result_by_end[temp.index] = count;
        update(tree_by_end, temp.start, 1, comp_size);
        count_by_end++;
    }
    
    // Process intervals sorted by start.
    for (int i = 0; i < num_queries; i++) {
        Interval temp = vector_by_start[i];
        // Count how many intervals with a lower end value have been seen.
        int count = count_by_start - read(tree_by_start, temp.end-1);
        result_by_start[temp.index] = count;
        update(tree_by_start, temp.end, 1, comp_size);
        count_by_start++;
    }
    
    // Output the results.
    for (int i = 0; i < num_queries; i++) {
        printf("%d ", result_by_end[i]);
    }
    printf("\n");
    for (int i = 0; i < num_queries; i++) {
        printf("%d ", result_by_start[i]);
    }
    printf("\n");
    
    return 0;
}
