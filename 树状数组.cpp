#include <vector>
using namespace std;
using ll = long long;
class binaryIndexedTree{
    ll lowbit(ll x){
        return x & -x;
    }
    vector<ll> *org;
    vector<ll> tree;
    int size;
    ll query(int x){
        ll ans = 0;
        while(x){
            ans += tree[x];
            x -= lowbit(x); 
        }
        return ans;
    }
public:
    binaryIndexedTree(vector<ll> *_org, int _size) : tree(_size + 5){ //initialize
        org = _org;
        size = _size;
        for(int i = 1; i <= size; i++){
            for(int j = i; j <= size; j += lowbit(j)){ //the father of tree[j] is tree[j + lowbit(j)]
                tree[j] += org->at(i);
            }
        }
    }
    ll query(int l, int r){ //turn [l, r] into [1, r] - [1, l - 1]
        return query(r) - query(l - 1);
    }
    void add(ll val, int ind){
        for(int i = ind; i <= size; i += lowbit(i)){
            tree[i] += val;
        }
    }
};