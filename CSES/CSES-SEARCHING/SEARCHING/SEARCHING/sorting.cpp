#include<algorithm>
#include<vector>
#include<cstdio>

using namespace std;

bool compare(int x, int y) {
	return x>y;
}


int main() {
	freopen("./input.txt", "r", stdin);
	freopen("./output.txt", "w", stdout);
	vector<int>v = { 1,3,5,45,6 };
	for (auto i = v.begin(); i != v.end(); i++) {
		printf("%d \n", (*i));

	}
	printf("__________\n");
	sort(v.begin(), v.end(),compare);

	for (auto i = v.begin(); i != v.end(); i++) {
		printf("%d \n", *i);
	}
}