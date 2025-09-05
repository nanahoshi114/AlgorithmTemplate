#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
using i64 = long long;
int lowbit(int x) {
    return x & -x;
}
class SegTree {
    struct Node {
        i64 val;
        int ls = -1, rs = -1;
    };
    int root;
    static std::vector<Node> tree;
    int insert_new_node() {
        tree.push_back({0, -1, -1});
        return tree.size() - 1;
    }
    i64 query(int p, int l, int r, int fl, int fr) {
        if (p == -1) return 0;
        if (fl <= l && r <= fr) {
            return tree[p].val;
        }
        int mid = (l + r) >> 1;
        i64 res = 0;
        if (fl <= mid) {
            res += query(tree[p].ls, l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(tree[p].rs, mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int p, int l, int r, int fl, int fr, i64 x) {
        ++tree[p].val;
        if (fl <= l && r <= fr) {
            return ;
        }
        int mid = (l + r) >> 1;
        if (fl <= mid) {
            if (tree[p].ls == -1) {
                tree[p].ls = insert_new_node();
            }
            update(tree[p].ls, l, mid, fl, fr, x);
        }
        if (mid < fr) {
            if (tree[p].rs == -1) {
                tree[p].rs = insert_new_node();
            }
            update(tree[p].rs, mid + 1, r, fl, fr, x);
        }
    }
public:
    static int range;
    SegTree() {
        root = insert_new_node();
    }
    i64 query(int l, int r) {
        return query(root, 1, range, l, r);
    }
    void update(int l, int r, i64 x) {
        update(root, 1, range, l, r, x);
    } 
};
int SegTree::range;
std::vector<SegTree::Node> SegTree::tree;
class Seg_in_Fenwick {
    std::vector<SegTree> tree;
    int range;
    i64 query(int x_ind, int y_l, int y_r) {
        i64 res = 0;
        while (x_ind) {
            res += tree[x_ind].query(y_l, y_r);
            x_ind -= lowbit(x_ind);
        }
        return res;
    }
public:
    Seg_in_Fenwick(int size, int sgt_size) : range(size), tree(size + 1) {
        SegTree::range = sgt_size;
    }
    void update(int x_ind, int y_l, int y_r, i64 x) {
        for (int i = x_ind; i <= range; i += lowbit(i)) {
            tree[i].update(y_l, y_r, x);
        }
    }
    i64 query(int x_l, int x_r, int y_l, int y_r) {
        return query(x_r, y_l, y_r) - query(x_l, y_l, y_r);
    }
};