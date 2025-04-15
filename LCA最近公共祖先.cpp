#include <iostream>
#include <vector>
using namespace std;
struct Graph{
    vector<vector<int>> edge;
    int size; 
    Graph(int _size) : size(_size){
        edge.resize(size + 1);
        fa.resize(31, vector<int>(size + 1));
        depth.resize(size + 1);
    }
    void add(int u, int to){
        edge[u].push_back(to);
    }
    vector<vector<int>> fa;
    vector<int> depth;
    void lcadfs(int now, int nowfa){
        fa[0][now] = nowfa;
        depth[now] = depth[nowfa] + 1;
        for(int i = 1; (1 << i) <= depth[now]; i++){
            fa[i][now] = fa[i - 1][fa[i - 1][now]];
        }
        for(auto &i : edge[now]){
            if(i != nowfa)
                lcadfs(i, now);
        }
    }
    void lcamake(int s){
        lcadfs(s, 0);
    }
    int lcaquery(int x, int y){
        if(depth[x] > depth[y]) swap(x, y);
        for(int i = 30; i >= 0; i--){
            if((1 << i) <= depth[y] - depth[x]){
                y = fa[i][y];
            }
        }
        if(y == x) return x;
        for(int i = 30; i >= 0; i--){
            if(fa[i][y] != fa[i][x]){
                x = fa[i][x];
                y = fa[i][y];
            }
        }
        return fa[0][x];
    }
};
int main(){
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);
    //freopen("testin.txt", "r", stdin);
    int n, m, s;
    cin >> n >> m >> s;
    Graph graph(n);
    for(int i = 1; i < n; i++){
        int x, y;
        cin >> x >> y;
        graph.add(x, y);
        graph.add(y, x);
    }
    graph.lcamake(s);
    while(m--){
        int a, b;
        cin >> a >> b;
        cout << graph.lcaquery(a, b) << '\n';
    }
    return 0;
}