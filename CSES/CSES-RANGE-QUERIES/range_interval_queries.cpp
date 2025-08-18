#include <bits/stdc++.h>
using namespace std;

int n, q;
vector<vector<int>> tree;
vector<int> a;

void build(int node, int l, int r)
{
    if (l == r)
    {
        tree[node] = {a[l]};
        return;
    }
    int mid = (l + r) / 2;
    build(2 * node, l, mid);
    build(2 * node + 1, mid + 1, r);
    merge(tree[2 * node].begin(), tree[2 * node].end(),
          tree[2 * node + 1].begin(), tree[2 * node + 1].end(),
          back_inserter(tree[node]));
}

int query(int node, int l, int r, int ql, int qr, int c, int d)
{
    if (qr < l || r < ql)
        return 0; // no overlap
    if (ql <= l && r <= qr)
    { // total overlap
        return upper_bound(tree[node].begin(), tree[node].end(), d) -
               lower_bound(tree[node].begin(), tree[node].end(), c);
    }
    // partial overlap
    int mid = (l + r) / 2;
    return query(2 * node, l, mid, ql, qr, c, d) +
           query(2 * node + 1, mid + 1, r, ql, qr, c, d);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;
    a.resize(n);
    for (int i = 0; i < n; ++i)
        cin >> a[i];

    int size = 4 * n;
    tree.resize(size);

    build(1, 0, n - 1);

    while (q--)
    {
        int l, r, c, d;
        cin >> l >> r >> c >> d;
        --l, --r;
        int ans = query(1, 0, n - 1, l, r, c, d);
        cout << ans << '\n';
    }

    return 0;
}
