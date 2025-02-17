#include <iostream>
using namespace std;
using ll = long long;
const ll INF = 0xfffffffffff;
ll fl[105][105];
int main(){
    ll n, m, u, v, w;
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            if(i != j){
                fl[i][j] = fl[j][i] = INF;
            }
        }
    }
    while(m--){
        cin >> u >> v >> w;
        fl[u][v] = fl[v][u] = min(fl[u][v], w);
    }
    for(int k = 1; k <= n; k++){
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                fl[i][j] = min(fl[i][j], fl[i][k] + fl[k][j]);
            }
        }
    }
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cout << fl[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}