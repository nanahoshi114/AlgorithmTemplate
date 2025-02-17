#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
class dsu{
private:
    vector<size_t> pa;
public:
    explicit dsu(size_t size) : pa(size + 1){
        iota(pa.begin(), pa.end(), 0);
    }
    size_t find(size_t x){
        return pa[x] == x ? x : pa[x] = find(pa[x]);
    }
    void unite(size_t x, size_t y){
        pa[find(x)] = find(y);
    }
};
int main(){
    int n, m;
    cin >> n >> m;
    dsu ds(n);
    while(m--){
        int z, x, y;
        cin >> z >> x >> y;
        x--, y--;
        if(z == 1){
            ds.unite(x, y);
        }else{
            if(ds.find(x) == ds.find(y)){
                cout << "Y\n";
            }else{
                cout << "N\n";
            }
        }
    }
    return 0;
}