#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
using i64 = long long;
const int MAXN = 40000000;
class SegTree {
    struct Node {
        int val;
        int ls, rs;
    };
    static const int range = int(1e9);
    static int count;
    static Node tree[MAXN];
    int insert_new_node() {
        tree[count] = {0, -1, -1};
        return count++;
    }
    int query(int p, int l, int r, int fl, int fr) {
        if (p == -1) return 0;
        if (fl <= l && r <= fr) {
            return tree[p].val;
        }
        int mid = (l + r) >> 1;
        int res = 0;
        if (fl <= mid) {
            res += query(tree[p].ls, l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(tree[p].rs, mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int p, int l, int r, int pos, int val) {
        tree[p].val += val;
        if (l == r) {
            return ;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            if (tree[p].ls == -1) {
                tree[p].ls = insert_new_node();
            }
            update(tree[p].ls, l, mid, pos, val);
        }
        if (mid < pos) {
            if (tree[p].rs == -1) {
                tree[p].rs = insert_new_node();
            }
            update(tree[p].rs, mid + 1, r, pos, val);
        }
    }
    static int query_kth_from_all(std::vector<int>& p_vec_l, std::vector<int>& p_vec_r, int l, int r, int k) {
        int lsize = 0, nowsize = 0;
        for (auto p : p_vec_r) {
            if (p == -1) continue;
            nowsize += tree[p].val;
            lsize += (tree[p].ls == -1 ? 0ll : tree[tree[p].ls].val);
        }
        for (auto p : p_vec_l) {
            if (p == -1) continue;
            nowsize -= tree[p].val;
            lsize -= (tree[p].ls == -1 ? 0ll : tree[tree[p].ls].val);
        }
        if (nowsize == 0) return -1;
        if (l == r) return l;
        int mid = (l + r) >> 1;
        
        if (lsize >= k) {
            for (auto& p : p_vec_l) {
                if (p != -1) {
                    p = tree[p].ls;
                }
            }
            for (auto& p : p_vec_r) {
                if (p != -1) {
                    p = tree[p].ls;
                }
            }
            return query_kth_from_all(p_vec_l, p_vec_r, l, mid, k);
        } else {
            for (auto& p : p_vec_l) {
                if (p != -1) {
                    p = tree[p].rs;
                }
            }
            for (auto& p : p_vec_r) {
                if (p != -1) {
                    p = tree[p].rs;
                }
            }
            return query_kth_from_all(p_vec_l, p_vec_r, mid + 1, r, k - lsize);
        }
    }
public:
    int root;
    SegTree() {
        root = insert_new_node();
    }
    int query(int l, int r) {
        return query(root, 0, range, l, r);
    }
    void update(int pos, i64 val) {
        update(root, 0, range, pos, val);
    } 
    static int query_kth_from_all(std::vector<int>& p_vec_l, std::vector<int>& p_vec_r, int k) {
        return query_kth_from_all(p_vec_l, p_vec_r, 0, range, k);
    }
};
SegTree::Node SegTree::tree[MAXN];
int SegTree::count = 0;
class Seg_in_Fenwick {
    std::vector<SegTree> tree;
    int range;
    static int lowbit(int x) {
        return x & -x;
    }
    auto get_tree_node(int ind) {
        std::vector<int> p;
        while (ind) {
            p.push_back(tree[ind].root);
            ind -= lowbit(ind);
        }
        return p;
    }
public:
    Seg_in_Fenwick(int size) : range(size), tree(size + 1) {}
    void update(int ind, int seg_ind, int x) {
        for (int i = ind; i <= range; i += lowbit(i)) {
            tree[i].update(seg_ind, x);
        }
    }
    int query_kth_range(int l, int r, int k) {
        auto pl = get_tree_node(l - 1), pr = get_tree_node(r);
        return SegTree::query_kth_from_all(pl, pr, k);
    }
};
/* use std::list ver.
class SegTree {
    struct Node {
        int val;
        int ls, rs;
    };
    static const int range = int(1e9);
    static int count;
    static Node tree[MAXN];
    int insert_new_node() {
        tree[count] = {0, -1, -1};
        return count++;
    }
    int query(int p, int l, int r, int fl, int fr) {
        if (p == -1) return 0;
        if (fl <= l && r <= fr) {
            return tree[p].val;
        }
        int mid = (l + r) >> 1;
        int res = 0;
        if (fl <= mid) {
            res += query(tree[p].ls, l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(tree[p].rs, mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int p, int l, int r, int pos, int val) {
        tree[p].val += val;
        if (l == r) {
            return ;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            if (tree[p].ls == -1) {
                tree[p].ls = insert_new_node();
            }
            update(tree[p].ls, l, mid, pos, val);
        }
        if (mid < pos) {
            if (tree[p].rs == -1) {
                tree[p].rs = insert_new_node();
            }
            update(tree[p].rs, mid + 1, r, pos, val);
        }
    }
    static int query_kth_from_all(std::list<int>& p_vec_l, std::list<int>& p_vec_r, int l, int r, int k) {
        int lsize = 0, nowsize = 0;
        for (auto p : p_vec_r) {
            nowsize += tree[p].val;
            lsize += (tree[p].ls == -1 ? 0ll : tree[tree[p].ls].val);
        }
        for (auto p : p_vec_l) {
            nowsize -= tree[p].val;
            lsize -= (tree[p].ls == -1 ? 0ll : tree[tree[p].ls].val);
        }
        if (nowsize == 0) return -1;
        if (l == r) return l;
        int mid = (l + r) >> 1;
        for (auto it = p_vec_l.begin(); it != p_vec_l.end(); ) {
            int next_p = lsize >= k ? tree[*it].ls : tree[*it].rs;
            if (next_p == -1) {
                it = p_vec_l.erase(it);
            } else {
                *it = next_p;
                ++it;
            }
        }
        for (auto it = p_vec_r.begin(); it != p_vec_r.end(); ) {
            int next_p = lsize >= k ? tree[*it].ls : tree[*it].rs;
            if (next_p == -1) {
                it = p_vec_r.erase(it);
            } else {
                *it = next_p;
                ++it;
            }
        }
        if (lsize >= k) {
            return query_kth_from_all(p_vec_l, p_vec_r, l, mid, k);
        }
        return query_kth_from_all(p_vec_l, p_vec_r, mid + 1, r, k - lsize);
    }
public:
    int root;
    SegTree() {
        root = insert_new_node();
    }
    int query(int l, int r) {
        return query(root, 0, range, l, r);
    }
    void update(int pos, i64 val) {
        update(root, 0, range, pos, val);
    } 
    static int query_kth_from_all(std::list<int>& p_vec_l, std::list<int>& p_vec_r, int k) {
        return query_kth_from_all(p_vec_l, p_vec_r, 0, range, k);
    }
};
*/
