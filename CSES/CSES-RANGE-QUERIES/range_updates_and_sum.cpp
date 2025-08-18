#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

struct SegmentTree
{
    ll n;
    vector<ll> sum, lazy_add, lazy_set;
    vector<bool> has_set;

    SegmentTree(const vector<ll> &arr)
    {
        n = arr.size();
        sum.assign(4 * n, 0);
        lazy_add.assign(4 * n, 0);
        lazy_set.assign(4 * n, 0);
        has_set.assign(4 * n, false);
        build(1, 0, n - 1, arr);
    }

    void build(ll node, ll l, ll r, const vector<ll> &arr)
    {
        if (l == r)
        {
            sum[node] = arr[l];
            return;
        }
        ll mid = (l + r) / 2;
        build(2 * node, l, mid, arr);
        build(2 * node + 1, mid + 1, r, arr);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }

    void push(ll node, ll l, ll r)
    {
        if (has_set[node])
        {
            sum[node] = (r - l + 1) * lazy_set[node];
            if (l != r)
            {
                apply_set(2 * node, lazy_set[node]);
                apply_set(2 * node + 1, lazy_set[node]);
            }
            has_set[node] = false;
            lazy_set[node] = 0;
            lazy_add[node] = 0;
        }
        else if (lazy_add[node] != 0)
        {
            sum[node] += (r - l + 1) * lazy_add[node];
            if (l != r)
            {
                lazy_add[2 * node] += lazy_add[node];
                lazy_add[2 * node + 1] += lazy_add[node];
            }
            lazy_add[node] = 0;
        }
    }

    void apply_set(ll node, ll value)
    {
        has_set[node] = true;
        lazy_set[node] = value;
        lazy_add[node] = 0;
    }

    void range_add(ll node, ll l, ll r, ll ql, ll qr, ll x)
    {
        push(node, l, r);
        if (qr < l || r < ql)
            return;
        if (ql <= l && r <= qr)
        {
            lazy_add[node] += x;
            push(node, l, r);
            return;
        }
        ll mid = (l + r) / 2;
        range_add(2 * node, l, mid, ql, qr, x);
        range_add(2 * node + 1, mid + 1, r, ql, qr, x);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }

    void range_set(ll node, ll l, ll r, ll ql, ll qr, ll x)
    {
        push(node, l, r);
        if (qr < l || r < ql)
            return;
        if (ql <= l && r <= qr)
        {
            apply_set(node, x);
            push(node, l, r);
            return;
        }
        ll mid = (l + r) / 2;
        range_set(2 * node, l, mid, ql, qr, x);
        range_set(2 * node + 1, mid + 1, r, ql, qr, x);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }

    ll range_sum(ll node, ll l, ll r, ll ql, ll qr)
    {
        push(node, l, r);
        if (qr < l || r < ql)
            return 0;
        if (ql <= l && r <= qr)
            return sum[node];
        ll mid = (l + r) / 2;
        return range_sum(2 * node, l, mid, ql, qr) +
               range_sum(2 * node + 1, mid + 1, r, ql, qr);
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;
    vector<ll> arr(n);
    for (ll &x : arr)
        cin >> x;

    SegmentTree st(arr);

    while (q--)
    {
        ll type, a, b;
        cin >> type >> a >> b;
        --a, --b; // convert to 0-based indexing
        if (type == 1)
        {
            ll x;
            cin >> x;
            st.range_add(1, 0, n - 1, a, b, x);
        }
        else if (type == 2)
        {
            ll x;
            cin >> x;
            st.range_set(1, 0, n - 1, a, b, x);
        }
        else if (type == 3)
        {
            cout << st.range_sum(1, 0, n - 1, a, b) << '\n';
        }
    }

    return 0;
}
