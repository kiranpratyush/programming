#include<algorithm>
#include<cstdio>
#include<vector>
#include<set>
using namespace std;

// Implement BIT ( Binary indexed tree range 1 - 200000)
// insert the value ( with update 1)
// get the value which is less than some value that is calculate the cummulative frequency of the values from 1 to n;


typedef struct {
	int start;
	int end;
	int index;
} Interval;

bool compare_by_end(Interval v1, Interval v2) {
	if (v1.end != v2.end) {
		return v2.end > v1.end; // First sort the end in the ascending order
	}
	return v1.start > v2.start; // Second sort the start in the ascending order
}

bool compare_by_start(Interval v1, Interval v2) {
	if (v1.start != v2.start) {
		return v2.start > v1.start;
	}
	return v1.end > v2.end;
}

int main() {
	/*freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);*/
	int num_queries;
	scanf("%d", &num_queries);
	vector<Interval>vector_by_end;
	vector<Interval>vector_by_start;
	multiset<int>set_by_end;
	multiset<int>set_by_start;
	vector<int>result_by_start(num_queries,0);
	vector<int>result_by_end(num_queries,0);
	for (int i = 0; i < num_queries; i++) {
		int start, end;
		scanf("%d %d", &start, &end);
		Interval interval;
		interval.start = start;
		interval.end = end;
		interval.index = i;
		vector_by_start.push_back(interval);
		vector_by_end.push_back(interval);
	}
	// sort the vector_by_end ( which will contain how many it contains)
	sort(vector_by_end.begin(), vector_by_end.end(), compare_by_end);
	// sort the vector_by_start ( which will calculate how many other ranges contain it)
	sort(vector_by_start.begin(), vector_by_start.end(), compare_by_start);
	for (int i = 0; i < num_queries; i++) {
		Interval temp = vector_by_end[i];
		// now check for the temp how many starts are present with lowerbound of start
		int count = distance(set_by_end.lower_bound(temp.start), set_by_end.end());
		result_by_end[temp.index] = count;
		// put the current start into the set
		set_by_end.insert(temp.start);
	}
	for (int i = 0; i < num_queries; i++) {
		Interval temp = vector_by_start[i];
		// now check for the temp how many ends are present with the lowerbound of end
		int count = distance(set_by_start.lower_bound(temp.end), set_by_start.end());
		result_by_start[temp.index] = count;
		set_by_start.insert(temp.end);
	}
	// print here for how many it  contains
	for (int i = 0; i < num_queries; i++) {
		printf("%d ", result_by_end[i]);
	}
	printf("\n");
	// print here for how many contains it 
	for (int i = 0; i < num_queries; i++) {
		printf("%d ", result_by_start[i]);
	}
	printf("\n");

}

// Use policy based data structure


// Use a Binary Indexed tree ( range is 200000 ) 