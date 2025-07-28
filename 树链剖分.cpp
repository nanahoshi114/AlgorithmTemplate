#include <iostream>
#include <vector>
using GraphEdge = std::vector<std::vector<int>>;
using i64 = long long;
struct SegTree {
    std::vector<i64> tree, tag;
    int size;
    SegTree(int size) : size(size), tree(size << 2), tag(size << 2) {}
    #define ls(x) (x << 1)
    #define rs(x) (x << 1 | 1)
    void pushup(int p) {
        tree[p] = tree[ls(p)] + tree[rs(p)];
    }
    void pushdown(int p, int l, int r) {
        if (tag[p] && l != r) {
            int mid = (l + r) >> 1;
            tree[ls(p)] += tag[p] * (mid - l + 1);
            tree[rs(p)] += tag[p] * (r - mid);
            tag[ls(p)] += tag[p];
            tag[rs(p)] += tag[p];
            tag[p] = 0;
        }
    }
    void update(int p, int l, int r, int fl, int fr, i64 x) {
        if (fl <= l && r <= fr) {
            tree[p] += x * (r - l + 1);
            tag[p] += x;
            return ;
        }
        pushdown(p, l, r);
        int mid = (l + r) >> 1;
        if (fl <= mid) {
            update(ls(p), l, mid, fl, fr, x);
        }
        if (mid < fr) {
            update(rs(p), mid + 1, r, fl, fr, x);
        }
        pushup(p);
    } 
    i64 query(int p, int l, int r, int fl, int fr) {
        if (fl <= l && r <= fr) {
            return tree[p];
        }
        pushdown(p, l, r);
        int mid = (l + r) >> 1;
        i64 res = 0;
        if (fl <= mid) {
            res += query(ls(p), l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(rs(p), mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int l, int r, i64 x) {
        update(1, 1, size, l, r, x);
    }
    i64 query(int l, int r) {
        return query(1, 1, size, l, r);
    }
    #undef ls
    #undef rs
};
struct TreeDecomposition {
    const GraphEdge& org_tree;
    const std::vector<i64>& org_seq;
    std::vector<int> fa, dep, size, hson, top, dfn, rnk;
    int n, dfs_cnt, root;
    SegTree sgt;
    int tree_build(int u, int nowdepth, int pre) {
        dep[u] = nowdepth;
        size[u] = 1;
        for (auto v : org_tree[u]) {
            if (v == pre) continue;
            size[u] += tree_build(v, nowdepth + 1, u);
            fa[v] = u;
            if (size[v] > size[hson[u]]) {
                hson[u] = v;
            }
        }
        return size[u];
    }
    void tree_get_top(int u, int nowtop, int pre) {
        top[u] = nowtop;
        dfn[u] = ++dfs_cnt;
        rnk[dfs_cnt] = u;
        if (hson[u])
            tree_get_top(hson[u], nowtop, u);
        for (auto v : org_tree[u]) {
            if (v == pre || v == hson[u]) continue;
            tree_get_top(v, v, u);
        }
    }
    TreeDecomposition(const GraphEdge& tree, int n, int root, const std::vector<i64>& org_seq) : 
    org_tree(tree), n(n), fa(n + 1), dep(n + 1), size(n + 1), hson(n + 1), top(n + 1), dfn(n + 1), rnk(n + 1), dfs_cnt(0), root(root), sgt(n), org_seq(org_seq)
    {
        tree_build(root, 0, 0);
        tree_get_top(root, root, 0);
        for (int i = 1; i <= n; ++i) {
            sgt.update(dfn[i], dfn[i], org_seq[i]);
        }
    }
    i64 tree_path_sum(int u, int v) {
        i64 res = 0;
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                std::swap(u, v);
            }
            res += sgt.query(dfn[top[u]], dfn[u]);
            u = fa[top[u]];
        }
        int up = std::min(dfn[u], dfn[v]), down = std::max(dfn[u], dfn[v]);
        res += sgt.query(up, down);
        return res;
    }
    void tree_path_update(int u, int v, i64 x) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                std::swap(u, v);
            }
            sgt.update(dfn[top[u]], dfn[u], x);
            u = fa[top[u]];
        }
        int up = std::min(dfn[u], dfn[v]), down = std::max(dfn[u], dfn[v]);
        sgt.update(up, down, x);
    }
    i64 subtree_sum(int u) {
        return sgt.query(dfn[u], dfn[u] + size[u] - 1);
    }
    void subtree_update(int u, i64 x) {
        sgt.update(dfn[u], dfn[u] + size[u] - 1, x);
    }
};
int main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n, m, root;
    i64 mod;
    std::cin >> n >> m >> root >> mod;
    GraphEdge edge(n + 1);
    std::vector<i64> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> a[i];
    }
    for (int i = 1; i < n; ++i) {
        int u, v;
        std::cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    TreeDecomposition treedec(edge, n, root, a);
    while (m--) {
        int opt, x, y;
        i64 z;
        std::cin >> opt;
        if (opt == 1) {
            std::cin >> x >> y >> z;
            treedec.tree_path_update(x, y, z);
        } else if (opt == 2) {
            std::cin >> x >> y;
            std::cout << treedec.tree_path_sum(x, y) % mod << '\n';
        } else if (opt == 3) {
            std::cin >> x >> z;
            treedec.subtree_update(x, z);
        } else {
            std::cin >> x;
            std::cout << treedec.subtree_sum(x) % mod << '\n';
        }
    }
    return 0;
}