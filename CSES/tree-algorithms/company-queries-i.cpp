#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

void dfs(int node, int parent, vector<vector<int>> &tree, vector<vector<int>> &binary_lifting, int l) {
    binary_lifting[node][0] = parent;
    for (int i = 1; i <= l; i++) {
        if (binary_lifting[node][i - 1] != -1)
            binary_lifting[node][i] = binary_lifting[binary_lifting[node][i - 1]][i - 1];
        else
            binary_lifting[node][i] = -1;
    }
    for (auto x : tree[node]) {
        if (x != parent)
            dfs(x, node, tree, binary_lifting, l);
    }
}

int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int num_employees, queries;
    scanf("%d %d", &num_employees, &queries);

    vector<vector<int>> tree(num_employees + 1);
    for (int i = 2; i <= num_employees; i++) {
        int parent;
        scanf("%d", &parent);
        tree[parent].push_back(i);
    }

    int l = ceil(log2(num_employees));
    vector<vector<int>> binary_lifting(num_employees + 1, vector<int>(l + 1, -1));
    dfs(1, -1, tree, binary_lifting, l);

    vector<int> result(queries);
    for (int qi = 0; qi < queries; qi++) {
        int employee, level;
        scanf("%d %d", &employee, &level);

        int r = employee;
        for (int i = 0; level > 0 && r != -1; i++) {
            if (level & 1)
                r = binary_lifting[r][i];
            level >>= 1;
        }
        result[qi] = r;
    }

    for (int x : result)
        printf("%d\n", x);

    return 0;
}
