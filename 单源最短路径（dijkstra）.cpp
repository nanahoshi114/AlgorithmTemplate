#include <iostream>
#include <queue>
#include <cstring>
#include <vector>
using namespace std;
const int M = 2e5 + 5;
const int INF = 2e9;
struct node{
    int pos, dis;
    bool operator<(const node & r)const{
        return dis > r.dis;
    }
};
struct Edge{
    int to, next, v;
}edge[M];
int head[M], cnt, n, m, s, dis[M];
bool vis[M];
void add(int u, int d, int v){
    edge[cnt].next = head[u];
    edge[cnt].to = d;
    edge[cnt].v = v;
    head[u] = cnt++;
}
priority_queue<node> que;
void bfs(){
    que.push({s, 0});
    while(!que.empty()){
        node now = que.top();
        que.pop();
        if(vis[now.pos]) continue;
        vis[now.pos] = 1;
        for(int i = head[now.pos]; ~i; i = edge[i].next){
            node next = {edge[i].to, now.dis + edge[i].v};
            if(dis[next.pos] > next.dis){
                dis[next.pos] = next.dis;
                if(!vis[next.pos]) que.push(next);
            }
        }
    }
}
int main(){
    cin >> n >> m >> s;
    for(int i = 1; i <= n; i++){
        dis[i] = INF;
        head[i] = -1;
    }
    dis[s] = 0;
    while(m--){
        int u, d, v;
        cin >> u >> d >> v;
        add(u, d, v);
    }
    bfs();
    for(int i = 1; i <= n; i++){
        cout << dis[i] << ' ';
    }
    return 0;
}