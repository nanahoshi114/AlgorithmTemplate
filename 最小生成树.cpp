#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
#include <ranges>
using namespace std;
using ll = long long;
struct DSU{
    vector<int> pa;
    int size;
    DSU(int _size) : size(_size){
        pa.resize(size + 1);
        iota(pa.begin(), pa.end(), 0);
    }
    int find(int x){
        return pa[x] == x ? x : pa[x] = find(pa[x]);
    }
    void unite(int x, int y){
        pa[find(x)] = find(y);
    }
};
struct Graph{
    struct Edge{
        int u, to, dis;
    };
    vector<Edge> edge;
    int size;
    DSU dsu;
    Graph(int _size) : size(_size), dsu(size) {}
    void add(int u, int to, int dis){
        edge.push_back({u, to, dis});
    }
    int kruscal(){
        ranges::sort(edge, [](const Edge &_l, const Edge &_r){
            return _l.dis < _r.dis;
        });
        int ans = 0, edgecnt = 0;
        for(auto &i : edge){
            if(dsu.find(i.u) != dsu.find(i.to)){
                ans += i.dis;
                edgecnt++;
                dsu.unite(i.u, i.to);
            }
            if(edgecnt >= size - 1){
                return ans;
            }
        }
        return -1;
    }
};
int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    Graph graph(n);
    for(int i = 1; i <= m; i++){
        int x, y, l;
        cin >> x >> y >> l;
        graph.add(x, y, l);
        graph.add(y, x, l);
    }
    int ans = graph.kruscal();
    if(ans != -1){
        cout << ans << '\n';
    }else{
        cout << "orz\n";
    }
    return 0;
}