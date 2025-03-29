#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
struct Line{
    ll l, r, h, mark;
};
struct SegTree{
    #define ls(p) (p << 1)
    #define rs(p) (p << 1 | 1)
    struct Node{
        ll l, r, cnt, len;
    };
    vector<Node> tree;
    vector<ll> &vecx;
    int range;
    void build(int p, int l, int r){
        tree[p].l = l; tree[p].r = r;
        if(l == r){
            return ;
        }
        int mid = (l + r) >> 1;
        build(ls(p), l, mid);
        build(rs(p), mid + 1, r);
    }
    SegTree(vector<ll> &_vecx) : vecx(_vecx){
        auto it = unique(vecx.begin() + 1, vecx.end());
        range = it - vecx.begin() - 2;
        tree.assign(range << 2, {});
        build(1, 1, range);
    }
    void pushup(int p){
        if(tree[p].cnt)
            tree[p].len = vecx[tree[p].r + 1] - vecx[tree[p].l];
        else if(tree[p].l != tree[p].r)
            tree[p].len = tree[ls(p)].len + tree[rs(p)].len;
        else
            tree[p].len = 0;
    }
    void update(int p, ll L, ll R, ll flag){
        if(vecx[tree[p].r + 1] <= L || R <= vecx[tree[p].l]){
            return ;
        }
        if(L <= vecx[tree[p].l] && vecx[tree[p].r + 1] <= R){
            tree[p].cnt += flag;
            pushup(p);
            return ;
        }
        update(ls(p), L, R, flag);
        update(rs(p), L, R, flag);
        pushup(p);
    }
    void update(ll l, ll r, ll val){
        update(1, l, r, val);
    }
    ll query_total(){
        return tree[1].len;
    }
    #undef ls
    #undef rs
};
int main(){
    //freopen("testin.txt", "r", stdin);
    cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    cin >> n;
    vector<Line> line;
    vector<ll> vecx{0};
    ll x1, y1, x2, y2;
    for(int i = 1; i <= n; i++){
        cin >> x1 >> y1 >> x2 >> y2;
        vecx.push_back(x1); vecx.push_back(x2);
        line.push_back({x1, x2, y1, 1ll});
        line.push_back({x1, x2, y2, -1ll});
    }
    ranges::sort(vecx);
    SegTree sgt(vecx);
    ranges::sort(line, [](const Line &_l, const Line &_r){
        return _l.h < _r.h;
    });
    ll ans = 0;
    for(int i = 0; i < line.size() - 1; i++){
        sgt.update(line[i].l, line[i].r, line[i].mark);
        ans += sgt.query_total() * (line[i + 1].h - line[i].h);
        //cout << line[i].l << ' ' << line[i].r << ' ' << line[i].h << ' ' << line[i + 1].h << ' ' << sgt.query_total() << '\n';
    }
    cout << ans << '\n';
    return 0;
}