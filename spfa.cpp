#include <iostream>
#include <vector>
#include <queue>
using namespace std;
const int M = 5e5 + 5;
const int INF = (1 << 31) - 1;
struct Edge{
    int to, next, w;
};
vector<int> head;
vector<Edge> edge;
vector<int> dis;
bool vis[M];
void add(int u, int v, int w){
    Edge now = {v, head[u], w};
    edge.push_back(now);
    head[u] = edge.size() - 1;
}
void spfa(int st){
    queue<int> que;
    que.push(st);
    while(!que.empty()){
        int now = que.front();
        que.pop(); vis[now] = 0;
        for(int i = head[now]; ~i; i = edge[i].next){
            int next = edge[i].to;
            if(dis[now] + edge[i].w < dis[next]){
                dis[next] = dis[now] + edge[i].w;
                if(!vis[next]){
                    que.push(next);
                    vis[next] = 1;
                }
            }
        }
    }
}
int main(){
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m, s;
    cin >> n >> m >> s;
    head.assign(n + 1, -1);
    dis.assign(n + 1, INF);
    dis[s] = 0;
    for(int i = 1; i <= m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        add(u, v, w);
    }
    spfa(s);
    for(int i = 1; i <= n; i++) cout << dis[i] << ' ';
    return 0;
}