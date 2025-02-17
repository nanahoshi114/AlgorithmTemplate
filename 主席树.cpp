#include <vector>
using namespace std;
struct persistentSegmentTree{ //查找第k小的数
    struct Node{
        int ls, rs, sum; //sum：该点对应的范围[l, r]有几个数出现了
        Node(){
            ls = rs = sum = 0;
        }
        Node(int _ls, int _rs, int _sum){
            ls = _ls, rs = _rs, sum = _sum;
        }
    };
    vector<Node> tree;
    vector<int> root; //root[i]：第i个版本对应的[1, size]根节点
    int size;
    int build(int l, int r){ //建立一个空树
        int root = tree.size();
        tree.emplace_back();
        int mid = (l + r) >> 1;
        if(l != r){
            tree[root].ls = build(l, mid);
            tree[root].rs = build(mid + 1, r);
        }
        return root;
    }
    int update(int l, int r, int pre, int x){ 
        int root = tree.size();
        tree.emplace_back(tree[pre].ls, tree[pre].rs, tree[pre].sum + 1);
        if(l < r){
            int mid = (l + r) >> 1;
            if(x <= mid)
                tree[root].ls = update(l, mid, tree[pre].ls, x);
            else
                tree[root].rs = update(mid + 1, r, tree[pre].rs, x);
        }
        return root;
    }
    int query(int root, int l, int r, int k){
        if(r <= k){
            return tree[root].sum;
        }
        if(l > k){
            return 0;
        }
        int mid = (l + r) >> 1, res = 0;
        if(k > mid){
            res += query(tree[root].rs, mid + 1, r, k);
        }
        res += query(tree[root].ls, l, mid, k);
        return res;
    }
    persistentSegmentTree(int _size) : size(_size){
        root.push_back(build(1, size));
    }
    void update(int x){
        root.push_back(update(1, size, root.back(), x));
    }
    int query(int l, int r, int k){
        return query(root[r], 1, size, k) - query(root[l - 1], 1, size, k);
    }
};