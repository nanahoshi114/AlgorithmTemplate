#include <iostream>
#include <vector>
using namespace std;
class segtree{
private:
    vector<int> sed;
    vector<int> *org;
    void build(int s, int t, int p){
        if(s == t){
            sed[p] = (*org)[s];
            return ;
        }
        int mid = s + ((t - s) >> 1);
        build(s, mid, p * 2);
        build(mid + 1, t, p * 2 + 1);
        sed[p] = min(sed[p * 2], sed[p * 2 + 1]);
    }
    int getmin(int l, int r, int s, int t, int p){
        if(l <= s && t <= r){
            return sed[p];
        }
        int minn = 2e9;
        int mid = s + ((t - s) >> 1);
        if(l <= mid){
            minn = min(minn, getmin(l, r, s, mid, p * 2));
        }
        if(mid < r){
            minn = min(minn, getmin(l, r, mid + 1, t, p * 2 + 1));
        }
        return minn;
    }
public:
    explicit segtree(vector<int> *_org) : sed(_org->size() * 4 + 5){
        org = _org;
        build(0, org->size() - 1, 1);
    }
    int getmin(int l, int r){
        return getmin(l - 1, r - 1, 0, org->size() - 1, 1);
    }
};
int main(){
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, m;
    cin >> n >> m;
    vector<int> vec;
    while(n--){
        int t; cin >> t; vec.push_back(t);
    }
    segtree sgt(&vec);
    while(m--){
        int l, r;
        cin >> l >> r;
        cout << sgt.getmin(l, r) << ' ';
    }
    return 0;
}