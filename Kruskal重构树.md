用于解决原图中两个点之间的所有简单路径上最大边权的最小值类似的问题。
```cpp
signed main() {  
    int n, m;  
    cin >> n >> m;  
    vector<edge> e(m);  
    for (int i = 0; i < m; i++) {  
        int u, v, w;  
        cin >> u >> v >> w;  
        e[i] = {u, v, w};  
    }  
    sort(e.begin(), e.end(),[](edge a,edge b){return a.w>b.w;});  
    vector<int> fa(2*n + 1);  
    for (int i = 1; i <= 2*n; i++) {  
        fa[i] = i;  
    }  
    auto find = [&](auto &&self, int x) -> int {  
        return fa[x] == x ? fa[x] : fa[x]=self(self, fa[x]);  
    };  
    int cnt=n,c=1;  
    vector h(2*n+1,vector<int>());  
    vector val(2*n+1,0ll),maxx(2*n+1,0ll);  
    for(int i=0;i<m;i++){  
        auto[u,v,w]=e[i];  
        if(find(find,u)==find(find,v))continue;  
        val[++cnt]=w;  
        int l=find(find,u),r=find(find,v);  
        h[cnt].push_back(l);h[l].push_back(cnt);  
        h[cnt].push_back(r);h[r].push_back(cnt);  
        fa[l]=cnt,fa[r]=cnt;  
        maxx[cnt]=max({maxx[l],maxx[r],w});  
        c++;  
        if(c==n)break;  
    }
}
```