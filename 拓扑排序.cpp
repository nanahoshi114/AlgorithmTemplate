#include <iostream>
#include <vector>
#include <queue>
using namespace std;
const int M = 105;
struct Edge{
    int to, next;
};
vector<int> head;
vector<Edge> edge;
int ent[M], n;
void add(int u, int v){
    edge.push_back({v, head[u]});
    head[u] = edge.size() - 1;
}
vector<int> toposort(){
    vector<int> ans;
    queue<int> S;
    for(int i = 1; i <= n; i++){
        if(!ent[i]) S.push(i);
    }
    while(!S.empty()){
        int now = S.front();
        S.pop();
        ans.push_back(now);
        for(int i = head[now]; ~i; i = edge[i].next){
            int to = edge[i].to;
            if(!(--ent[to])){
                S.push(to);
            }
        }
    }
    return ans;
}
int main(){
    cin >> n;
    head.assign(n + 1, -1);
    for(int i = 1, t; i <= n; i++){
        while(cin >> t, t){
            add(i, t);
            ent[t]++;
        }
    }
    vector<int> ans = toposort();
    for(auto i : ans) cout << i << ' ';
    return 0;
}