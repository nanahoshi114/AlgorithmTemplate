## 文件: 2-sat.md
```cpp
#include <iostream>
#include <vector>
using GraphEdge = std::vector<std::vector<int>>;
auto tarjan_search(GraphEdge& edge, int n) {
    std::vector<int> dfn(n + 1), low(n + 1), stack, scc(n + 1), sc_size(1);
    stack.reserve(n);
    std::vector<bool> is_in_stack(n + 1);
    int dfncnt = 0;
    auto dfs = [&](auto&& self, int now) -> void {
        dfn[now] = low[now] = ++dfncnt;
        stack.push_back(now);
        is_in_stack[now] = true;
        for (const auto& next : edge[now]) {
            if (!dfn[next]) {
                self(self, next);
                low[now] = std::min(low[now], low[next]);
            } else if (is_in_stack[next]) {
                low[now] = std::min(low[now], low[next]);
            }
        }
        if (dfn[now] == low[now]) {
            sc_size.push_back(0);
            while (true) {
                scc[stack.back()] = sc_size.size() - 1;
                ++sc_size.back();
                is_in_stack[stack.back()] = false;
                if (stack.back() == now) {
                    stack.pop_back();
                    break;
                }
                stack.pop_back();
            }
            
        }
    };
    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            dfs(dfs, i);
        }
    }
    return std::pair(sc_size, scc);
}
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n, m;
    std::cin >> n >> m; 
    GraphEdge edge(2 * n + 1); 
    for (int i = 1; i <= m; ++i) {
        int x1, a, x2, b;
        std::cin >> x1 >> a >> x2 >> b;
        edge[x1 * 2 - a].push_back(x2 * 2 - (!b));
        edge[x2 * 2 - b].push_back(x1 * 2 - (!a));
    }
    auto [sc_size, scc] = tarjan_search(edge, 2 * n);
    std::vector<bool> ans(n + 1);
    for (int i = 1; i <= n; ++i) {
        if (scc[i * 2 - 1] == scc[i * 2]) {
            std::cout << "IMPOSSIBLE\n";
            return 0;
        }
        ans[i] = scc[i * 2 - 1] > scc[i * 2];
        // !xi -> xi，则xi = true
        // !xi -> xi <=> !xi所在scc拓扑序 小于 xi所在scc拓扑序 <=> !xi所在scc编号 大于 xi所在scc编号
    }
    std::cout << "POSSIBLE\n";
    for (int i = 1; i <= n; ++i) {
        std::cout << ans[i] << ' ';
    }
    std::cout << '\n';
}
```

## 文件: AC自动机.md
```cpp
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
constexpr int MAX_NODE = 2e5 + 5;
using i64 = long long;
namespace AC { 
    struct TrieNode {
        int next[26] = {};
        bool is_end = false;
        std::size_t end_hash = 0;
    } tree[MAX_NODE];
    int fail[MAX_NODE], node_cnt;
    std::vector<int> fail_tree[MAX_NODE];
    auto new_node() {
        tree[node_cnt++] = TrieNode{};
        return node_cnt - 1;
    }
    void reset() {
        tree[0] = {};
        node_cnt = 1;
    }
    void insert(std::string_view s, std::size_t hash) {
        int cur = 0;
        for (auto c : s) {
            if (!tree[cur].next[c - 'a']) {
                tree[cur].next[c - 'a'] = new_node();
            }
            cur = tree[cur].next[c - 'a'];
        }
        tree[cur].is_end = true;
        tree[cur].end_hash = hash;
    }
    void build_AC() {
        std::fill(fail, fail + node_cnt, 0);
        std::for_each(fail_tree, fail_tree + node_cnt, [](auto& v) { v.clear(); });
        std::queue<int> que;
        for (auto i : tree[0].next) {
            if (i) {
                que.push(i);
                fail_tree[0].push_back(i);
            } 
        }
        while (!que.empty()) {
            int now = que.front();
            que.pop();
            for (int i = 0; i < 26; ++i) {
                int& next = tree[now].next[i];
                if (next) {
                    fail[next] = tree[fail[now]].next[i]; 
                    fail_tree[tree[fail[now]].next[i]].push_back(next);
                    que.push(next);
                } else {
                    next = tree[fail[now]].next[i];
                }
            }
        }
    }
    auto query_AC(std::string_view s) {
        int cur = 0;
        std::unordered_map<std::size_t, int> occ;
        std::vector<int> ans(node_cnt);
        for (auto c : s) {
            cur = tree[cur].next[c - 'a'];
            ++ans[cur];
        }
        auto dfs = [&](auto&& self, int now) -> void {
            for (int next : fail_tree[now]) {
                self(self, next);
                ans[now] += ans[next];
            }
            if (tree[now].is_end && ans[now]) {
                occ[tree[now].end_hash] = ans[now];
            }
        };
        dfs(dfs, 0);
        return occ;
    }
}
class AC_automata { //查询字符串出现次数，建图dfs优化查询
    struct TrieNode {
        int next[26] = {};
        bool is_end = false;
        std::size_t end_hash = 0;
    };
    std::vector<TrieNode> tree;
    std::vector<int> fail;
    std::vector<std::vector<int>> fail_tree;
    auto new_node() {
        tree.push_back(TrieNode{});
        return tree.size() - 1;
    }
public:
    AC_automata() : tree(1) {
        tree.reserve(MAX_NODE);
    }
    void insert(std::string_view s, std::size_t hash) {
        int cur = 0;
        for (auto c : s) {
            if (!tree[cur].next[c - 'a']) {
                tree[cur].next[c - 'a'] = new_node();
            }
            cur = tree[cur].next[c - 'a'];
        }
        tree[cur].is_end = true;
        tree[cur].end_hash = hash;
    }
    void build_AC() {
        fail.assign(tree.size(), 0);
        fail_tree.assign(tree.size(), {});
        std::queue<int> que;
        for (auto i : tree[0].next) {
            if (i) {
                que.push(i);
                fail_tree[0].push_back(i);
            } 
        }
        while (!que.empty()) {
            int now = que.front();
            que.pop();
            for (int i = 0; i < 26; ++i) {
                int& next = tree[now].next[i];
                if (next) {
                    fail[next] = tree[fail[now]].next[i]; 
                    fail_tree[tree[fail[now]].next[i]].push_back(next);
                    que.push(next);
                } else {
                    next = tree[fail[now]].next[i];
                }
            }
        }
    }
    auto query_AC(std::string_view s) {
        int cur = 0;
        std::unordered_map<std::size_t, int> occ;
        std::vector<int> ans(tree.size());
        for (auto c : s) {
            cur = tree[cur].next[c - 'a'];
            ++ans[cur];
        }
        auto dfs = [&](auto&& self, int now) -> void {
            for (int next : fail_tree[now]) {
                self(self, next);
                ans[now] += ans[next];
            }
            if (tree[now].is_end && ans[now]) {
                occ[tree[now].end_hash] = ans[now];
            }
        };
        dfs(dfs, 0);
        return occ;
    }
};
namespace str_a { //支持多次查询，查询模式串是否出现过
    struct TrieNode {
        int next[26] = {};
        int endcnt = 0;
    };
    using TrieTree = std::vector<TrieNode>;
    auto init_tree() {
        TrieTree tree(1);
        tree.reserve(MAX_NODE);
        return tree;
    }
    auto new_node(TrieTree& tree) {
        tree.push_back(TrieNode{});
        return tree.size() - 1;
    }
    void insert(TrieTree& tree, std::string_view s) {
        int cur = 0;
        for (auto c : s) {
            if (!tree[cur].next[c - 'a']) {
                tree[cur].next[c - 'a'] = new_node(tree);
            }
            cur = tree[cur].next[c - 'a'];
        }
        ++tree[cur].endcnt;
    }
    struct AC_automata {
        TrieTree& tree;
        std::vector<int> fail, vis_cnt;
        int query_cnt = 0;
    };
    auto build_AC(TrieTree& tree) {
        std::vector<int> fail(tree.size());
        std::queue<int> que;
        for (auto i : tree[0].next) {
            if (i) que.push(i);
        }
        while (!que.empty()) {
            int now = que.front();
            que.pop();
            for (int i = 0; i < 26; ++i) {
                int& next = tree[now].next[i];
                if (next) {
                    fail[next] = tree[fail[now]].next[i]; 
                    que.push(next);
                } else {
                    next = tree[fail[now]].next[i];
                }
            }
        }
        return AC_automata{tree, fail, std::vector<int>(tree.size()), 0};
    }
    auto query(AC_automata& ac, std::string_view s) {
        auto& [tree, fail, vis_cnt, query_cnt] = ac;
        ++query_cnt;
        int cur = 0;
        i64 ans = 0;
        for (auto c : s) {
            cur = tree[cur].next[c - 'a'];
            for (int next = cur; next && vis_cnt[next] != query_cnt; next = fail[next]) {
                ans += tree[next].endcnt;
                vis_cnt[next] = query_cnt;
            }
        }
        return ans;
    }
}
```

## 文件: Andrew二维凸包.md
```cpp
auto static_convec_hull(std::vector<Pd> &pts) {
    int n = pts.size();
    std::sort(pts.begin(), pts.end());
    std::vector<int> stack;
    std::vector<bool> vis(n);
    stack.push_back(0);
    for (int i = 1; i < n; ++i) {
        while (stack.size() >= 2) {
            int s1 = stack.back(), s2 = stack[stack.size() - 2];
            if (cross((pts[s1] - pts[s2]), (pts[i] - pts[s1])) > 0) { //如果需要保留凸包边上的点，此处改为>=，同下
                break;
            }
            vis[s1] = false;
            stack.pop_back();
        }
        vis[i] = true;
        stack.push_back(i);
    }
    int down_sz = stack.size();
    for (int i = n - 2; i >= 0; --i) {
        if (vis[i]) continue;
        while (stack.size() > down_sz) {
            int s1 = stack.back(), s2 = stack[stack.size() - 2];
            if (cross((pts[s1] - pts[s2]), (pts[i] - pts[s1])) > 0) {
                break;
            }
            vis[s1] = false;
            stack.pop_back();
        }
        vis[i] = true;
        stack.push_back(i);
    }
    return stack;
}
```

## 文件: Hash.md
```cpp
#include <string>
#include <ranges>
#include <random>
using namespace std;
using i64 = long long;
std::mt19937 generator(43);
std::uniform_int_distribution<i64> rnd(1e8, 1e9);
const i64 M1 = rnd(generator), M2 = rnd(generator), base = 131;
auto get_hash(const string &s){
    i64 res1 = 0, res2 = 0;
    for(auto &i : s){
        res1 = (res1 * base + i) % M1;
        res2 = (res2 * base + i) % M2;
    }
    return pair(res1, res2);
}
using u64 = unsigned long long;
class Hash_RangeQuery {
    std::vector<std::array<u64, 2>> prem, preb;
public:
    explicit Hash_RangeQuery(const std::string& s) {
        prem.assign(s.size() + 1, std::array{0ull, 0ull});
        preb.assign(s.size() + 1, std::array{0ull, 0ull});
        for(int i = 1; i <= s.size(); i++){
            prem[i][0] = (prem[i - 1][0] * base + s[i - 1]) % M1;
            prem[i][1] = (prem[i - 1][1] * base + s[i - 1]) % M2;
        }
        preb[0][0] = preb[0][1] = 1;
        for(int i = 1; i <= s.size(); i++){
            preb[i][0] = preb[i - 1][0] * base % M1;
            preb[i][1] = preb[i - 1][1] * base % M2;
        }
    }
    auto query(int l, int r) const {
        return std::pair((prem[r + 1][0] - prem[l][0] * preb[r - l + 1][0] % M1 + M1) % M1, (prem[r + 1][1] - prem[l][1] * preb[r - l + 1][1] % M2 + M2) % M2);
    }
    bool check(int l1, int r1, int l2, int r2) const {
        return query(l1, r1) == query(l2, r2);
    }
};
//重载哈希
struct MyHash {
    auto operator()(std::pair<int, int> x) {
        auto hash1 = std::hash<int>{}(x.first);
        auto hash2 = std::hash<int>{}(x.second);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }  
};
```

## 文件: KMP.md
```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;
auto prefix_function(const string &s){
    vector<int> pi(s.size());
    for(int i = 1; i < s.size(); i++){
        int j = pi[i - 1];
        while(j > 0 && s[i] != s[j]) j = pi[j - 1];
        if(s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}
auto find_occur(const string &text, const string &pattern){
    string cur = pattern + '#' + text;
    vector<int> ans, lpf = prefix_function(cur);
    for(int i = pattern.size() + 1; i <= text.size() + pattern.size(); i++){
        if(lpf[i] == pattern.size()) ans.push_back(i - 2 * pattern.size());
    }
    return ans;
}
int main(){
    string s1, s2;
    cin >> s1 >> s2;
    auto ans = find_occur(s1, s2);
    for(auto i : ans){
        cout << i + 1 << '\n';
    }
    auto bd = prefix_function(s2);
    for(auto i : bd){
        cout << i << ' ';
    }
    cout << '\n';
    return 0;
}
```

## 文件: Kruskal重构树.md
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

## 文件: LCA最近公共祖先.md
```cpp
#include <vector>
using GraphEdge = std::vector<std::vector<int>>;
class LCA {
    std::vector<std::array<int, 25>> pa;
    std::vector<int> depth; 
    int size;
public:
    LCA(const GraphEdge &graph, int size, int root) : size(size), pa(size + 1), depth(size + 1) {
        auto dfs = [&](auto &&self, int now, int nowpa) -> void {
            pa[now][0] = nowpa;
            depth[now] = depth[nowpa] + 1;
            for (int i = 1; (1 << i) <= depth[now]; ++i) {
                pa[now][i] = pa[pa[now][i - 1]][i - 1];
            }
            for (auto &i : graph[now]) {
                if(i != nowpa)
                    self(self, i, now);
            }
        };
        dfs(dfs, root, 0);
    }
    auto query(int x, int y) {
        if (depth[x] > depth[y]) std::swap(x, y);
        for (int i = 24; i >= 0; --i) {
            if ((1 << i) <= depth[y] - depth[x]) {
                y = pa[y][i];
            }
        }
        if (y == x) return x;
        for (int i = 24; i >= 0; --i) {
            if (pa[y][i] != pa[x][i]) {
                x = pa[x][i];
                y = pa[y][i];
            }
        }
        return pa[x][0];
    }
};
```

## 文件: Main-Lorentz(寻找重串数量）.md
```cpp
#include <bits/stdc++.h>
#define LL long long
using namespace std;

LL n, longest = 0;
string s;

vector<LL> z_function(string s){
    vector<LL> ret;
    ret.resize(s.size());
    LL ll = 0, rr = 0; // 可以使用已知資訊區間的 [ll, rr)

    for (LL i=1 ; i<s.size() ; i++){ // 目前要找的索引值
        LL j = 0; // i 需要枚舉前綴的開頭
		
		// 如果 i 還在只知資訊區間就可以先用前面得到的資訊
		// 找 i-ll 的答案，rr-i 是上界
        if (i<rr) j = min(ret[i-ll], rr-i);
        while (s[j]==s[i+j]) j++; // 枚舉已知資訊後的內容
        ret[i] = j;
		
        if (i+j>rr){ // 如果新的範圍大於已知資訊的區間，就更新
            ll = i;
            rr = i+j;
        }
    }

    ret[0] = s.size();
    return ret;
}

LL z_value(vector<LL> &v, LL p){
    if (0<=p && p<v.size()) return v[p];
    return 0;
}

// 尋找字串 s 中的重串數量
LL repetition(string s){

    // 終止條件
    if (s.size()==1){
        return 0;
    }

    // 找到左邊右邊的重串
    LL mid = s.size()/2;
    string u = s.substr(0, mid);
    string v = s.substr(mid);
    string ru(u.rbegin(), u.rend());
    string rv(v.rbegin(), v.rend());
    LL lc = repetition(u);
    LL rc = repetition(v);

    // 尋找中間的重串
    LL total = 0;
    vector<LL> z1 = z_function(ru);
    vector<LL> z2 = z_function(v+'#'+u);
    vector<LL> z3 = z_function(ru+'#'+rv);
    vector<LL> z4 = z_function(v);

    // 找到左偏重串
    for (LL ptr=0 ; ptr<u.size() ; ptr++){
        if (u[ptr]==v[0]){
            LL l = u.size()-ptr;
            LL L = z_value(z1, u.size()-ptr);
            L = min(L, l-1); // 限制長度，不能和 ptr 重疊
            LL R = z_value(z2, v.size()+1+ptr);
            R = min(R, l-1); // 限制長度，確保是左偏重串

            int add = max(0LL, R+L-l+1);
            total += add;
            if (add){
                longest = max(longest, l);
            }
        }
    }

    // 找到右偏重串
    for (LL ptr=0 ; ptr<v.size() ; ptr++){
        if (u.back()==v[ptr]){
            LL l = ptr+1;
            LL L = z_value(z3, u.size()+v.size()-ptr);
            LL R = z_value(z4, ptr+1);
            R = min(R, l-1); // 限制長度，不能和 ptr 重疊

            int add = max(0LL, R+L-l+1);
            total += add;
            if (add){
                longest = max(longest, l);
            }
        }
    }

    return lc+rc+total;
}

int main(){

    // input
    cin >> n;
    cin >> s;

    // process
    int res = repetition(s);
    cout << longest*2 << " " << res << "\n";

    return 0;
}

```

## 文件: ST表.md
```cpp
template <typename T>
class sparseTable {
    std::vector<std::array<T, 30>> table;
    std::vector<T> lg;
    T (*func)(const T, const T);
public:
    sparseTable(const std::vector<T>& arr, T (*func)(const T, const T)) : func(func), table(arr.size()), lg(arr.size()) {
        lg[1] = 0;
        int n = arr.size() - 1;
        for (int i = 2; i <= n; ++i) {
            lg[i] = lg[i / 2] + 1;
        }
        for (int i = 1; i <= n; ++i) {
            table[i][0] = arr[i];
        }
        for (int j = 1; j <= lg[n]; ++j) {
            for (int i = 1; i + (1 << j) - 1 <= n; ++i) {
                table[i][j] = func(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    T query(int l, int r) {
        int k = lg[r - l + 1];
        return func(table[l][k], table[r - (1 << k) + 1][k]);
    }
};
```

## 文件: Tarjan强连通分量.md
```cpp
using i64 = long long;
using GraphEdge = std::vector<std::vector<int>>;
auto tarjan_search(GraphEdge& edge, int n) {
    std::vector<int> dfn(n + 1), low(n + 1), stack;
    std::vector<bool> is_in_stack(n + 1);
    std::vector<std::vector<int>> answer;
    int dfncnt = 0;
    auto dfs = [&](auto&& self, int now) -> void {
        dfn[now] = low[now] = ++dfncnt;
        stack.push_back(now);
        is_in_stack[now] = true;
        for (const auto& next : edge[now]) {
            if (!dfn[next]) {
                self(self, next);
                low[now] = std::min(low[now], low[next]);
            } else if (is_in_stack[next]) {
                low[now] = std::min(low[now], low[next]);
            }
        }
        if (dfn[now] == low[now]) {
            answer.emplace_back();
            do {
                answer.back().push_back(stack.back());
                is_in_stack[stack.back()] = false;
                stack.pop_back();
            } while (answer.back().back() != now);
        }
    };
    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            dfs(dfs, i);
        }
    }
    return answer;
}
auto tarjan_search_ver2(GraphEdge& edge, int n) {
    std::vector<int> dfn(n + 1), low(n + 1), stack, scc(n + 1), sc_size;
    std::vector<bool> is_in_stack(n + 1);
    int dfncnt = 0;
    auto dfs = [&](auto&& self, int now) -> void {
        dfn[now] = low[now] = ++dfncnt;
        stack.push_back(now);
        is_in_stack[now] = true;
        for (const auto& next : edge[now]) {
            if (!dfn[next]) {
                self(self, next);
                low[now] = std::min(low[now], low[next]);
            } else if (is_in_stack[next]) {
                low[now] = std::min(low[now], low[next]);
            }
        }
        if (dfn[now] == low[now]) {
            sc_size.push_back(0);
            while (true) {
                scc[stack.back()] = sc_size.size() - 1;
                ++sc_size.back();
                is_in_stack[stack.back()] = false;
                if (stack.back() == now) {
                    stack.pop_back();
                    break;
                }
                stack.pop_back();
            }
            
        }
    };
    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            dfs(dfs, i);
        }
    }
    return std::pair(sc_size, scc);
}
```

## 文件: floyd.md
```cpp
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
```

## 文件: manacher（分别计算奇数长度、偶数长度）.md
```cpp
std::pair<std::vector<int>, std::vector<int>> manacher(const std::string &s){
    std::vector<int> d1(s.size() + 5, 0), d2(s.size() + 5, 0); 
    for(int i = 0, l = 0, r = -1; i < s.size(); i++){
        if(i <= r) d1[i] = std::min(d1[l + r - i], r - i + 1);
        while(0 <= i - d1[i] && i + d1[i] < s.size() && s[i - d1[i]] == s[i + d1[i]]){
            d1[i]++;
        }
        if(i + d1[i] - 1 > r){
            l = i - d1[i] + 1;
            r = i + d1[i] - 1;
        }
    }
    for(int i = 0, l = 0, r = -1; i < s.size(); i++){
        if(i <= r) d2[i] = std::min(d2[l + r - i + 1], r - i + 1);
        while(0 <= i - d2[i] - 1 && i + d2[i] < s.size() && s[i - d2[i] - 1] == s[i + d2[i]]){
            d2[i]++;
        }
        if(i + d2[i] - 1 > r){
            l = i - d2[i];
            r = i + d2[i] - 1;
        }
    }
    return {d1, d2};
}
```

## 文件: pbds库.md
```cpp
#include <bits/extc++.h>
using i64 = long long;
struct multiset {
    __gnu_pbds::tree<std::pair<i64, int>, __gnu_pbds::null_type, std::less<>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> st;
    int cnt = 0;
    multiset() {
        st.insert({-1, 0});
    }
    void insert(i64 x) {
        st.insert({x, ++cnt});
    }
    void erase(i64 x) {
        st.erase(st.lower_bound({x, 0}));
    }
    i64 find_by_order(int x) {
        return st.find_by_order(x)->first;
    }
    int order_of_key(i64 x) {
        return st.order_of_key({x, 0});
    }
};
```

## 文件: readme.md
# 算法模版
同时存在.md文件和.cpp文件的是正式的模版，只存在.cpp/.c文件的是不重要的内容/待优化的内容/写得太烂放弃治疗的内容。


## 文件: 主席树.md
```cpp
struct persistentSegmentTree{ 
    struct Node{
        int ls, rs, sum; 
        Node(){
            ls = rs = sum = 0;
        }
        Node(int _ls, int _rs, int _sum){
            ls = _ls, rs = _rs, sum = _sum;
        }
    };
    vector<Node> tree;
    vector<int> root; 
    int size;
    int build(int l, int r){ 
        int root = tree.size();
        tree.emplace_back();
        int mid = (l + r) >> 1;
        if(l != r){
            tree[root].ls = build(l, mid);
            tree[root].rs = build(mid + 1, r);
        }
        return root;
    }
    int update(int l, int r, int pre, int x){ 
        int root = tree.size();
        tree.emplace_back(tree[pre].ls, tree[pre].rs, tree[pre].sum + 1);
        if(l < r){
            int mid = (l + r) >> 1;
            if(x <= mid)
                tree[root].ls = update(l, mid, tree[pre].ls, x);
            else
                tree[root].rs = update(mid + 1, r, tree[pre].rs, x);
        }
        return root;
    }
    int query_find_rank_of_k(int root, int l, int r, int k){
        if(r <= k){
            return tree[root].sum;
        }
        if(l > k){
            return 0;
        }
        int mid = (l + r) >> 1, res = 0;
        if(k > mid){
            res += query_find_rank_of_k(tree[root].rs, mid + 1, r, k);
        }
        res += query_find_rank_of_k(tree[root].ls, l, mid, k);
        return res;
    }
    int query_find_kth(int rootl, int rootr, int l, int r, int k){
        if(l == r){
            return l;
        }
        int mid = (l + r) >> 1;
        int lsum = tree[tree[rootr].ls].sum - tree[tree[rootl].ls].sum;
        if(lsum >= k){
            return query_find_kth(tree[rootl].ls, tree[rootr].ls, l, mid, k);
        }else{
            return query_find_kth(tree[rootl].rs, tree[rootr].rs, mid + 1, r, k - lsum);
        }
    }
    persistentSegmentTree(int _size) : size(_size){
        root.reserve(1000000);
        root.push_back(build(1, size));
    }
    void update(int x){
        root.push_back(update(1, size, root.back(), x));
    }
    int query_find_rank_of_k(int l, int r, int k){
        return query_find_rank_of_k(root[r], 1, size, k) - query_find_rank_of_k(root[l - 1], 1, size, k);
    }
    int query_find_kth(int l, int r, int k){
        return query_find_kth(root[l - 1], root[r], 1, size, k);
    }
};
```

## 文件: 匈牙利算法.md
```cpp
namespace Graph {
    using GraphEdge = std::vector<std::vector<int>>;
    auto Hungary(GraphEdge& bin_graph, int n, int m) {
        std::vector<bool> flag(m);
        std::vector<int> match(m, -1);
        auto find = [&](auto&& self, int from) -> bool {
            for (auto next : bin_graph[from]) {
                if (!flag[next]) {
                    flag[next] = true;
                    if (match[next] == -1 || self(self, match[next])) {
                        match[next] = from;
                        return true;
                    }
                }
            }
            return false;
        };
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            std::fill(flag.begin(), flag.end(), false);
            if (find(find, i)) {
                ++ans;
            }
        }
        std::vector<std::pair<int, int>> match_pair;
        for (int i = 0; i < m; ++i) {
            if (match[i] != -1) {
                match_pair.push_back({i, match[i]});
            }
        }
        return match_pair;
    }
}
```

## 文件: 单源最短路径（dijkstra）.md
```cpp
struct Node {
    int pos, dis;
    bool operator<(Node _r) const {
        return dis > _r.dis;
    }
};
struct Edge{
    int to, dis;
};
using GraphEdge = std::vector<std::vector<Edge>>;
auto dijkstra(GraphEdge& edge, int st) {
    std::priority_queue<Node> que;
    std::vector<char> vis(edge.size(), 0);
    std::vector<int> dis(edge.size(), std::numeric_limits<int>::max());
    dis[st] = 0;
    que.push({st, 0});
    while (!que.empty()) {
        Node now = que.top();
        que.pop();
        if (vis[now.pos]) continue;
        vis[now.pos] = 1;
        for (auto& i : edge[now.pos]) {
            Node next = {i.to, now.dis + i.dis};
            if (next.dis < dis[i.to]) {
                dis[i.to] = next.dis;
                que.push(next);
            }
        }
    }
    return dis;
}
```

## 文件: 双pq实现erase操作.md
```cpp
template<typename T, typename Cmp = std::less<>>
class multiset {
    std::priority_queue<T, std::vector<T>, Cmp> que, delque;
public:
    void push(T x) {
        que.push(x);
    }
    void erase(T x) {
        delque.push(x);
    }
    T top() {
        while (!que.empty() && !delque.empty() && que.top() == delque.top()) {
            que.pop();
            delque.pop();
        }
        return que.top();
    }
    void pop() {
        while (!que.empty() && !delque.empty() && que.top() == delque.top()) {
            que.pop();
            delque.pop();
        }
        que.pop();
    }
};
```

## 文件: 可并堆.md
```cpp
struct Node {  
    int v;  
    int id;  
    Node *child, *sibling;    Node *father;  // 新增：父指针，若该节点为根节点则指向空节点 nullptr};  
  
Node *meld(Node *x, Node *y) {  
    if (x == nullptr) return y;  
    if (y == nullptr) return x;  
    if (x->v > y->v) std::swap(x, y);  
    if (x->child != nullptr) {  // 新增：维护父指针  
        x->child->father = y;  
    }    y->sibling = x->child;    y->father = x;  // 新增：维护父指针  
    x->child = y;  
    return x;  
}  
  
Node *merges(Node *x) {  
    if (x == nullptr) return nullptr;  
    x->father = nullptr;  // 新增：维护父指针  
    if (x->sibling == nullptr) return x;  
    Node *y = x->sibling, *c = y->sibling;    y->father = nullptr;  // 新增：维护父指针  
    x->sibling = y->sibling = nullptr;  
    return meld(merges(c), meld(x, y));  
}  
  
Node *decrease_key(Node *root, Node *x, int v) {  
    x->v = v;                 // 更新权值  
    if (x == root) return x;  // 如果 x 为根，则直接返回  
    // 把x从fa的子节点中剖出去，这里要分x的位置讨论一下．  
    if (x->father->child == x) {  
        x->father->child = x->sibling;    } else {  
        x->father->sibling = x->sibling;    }    if (x->sibling != nullptr) {  
        x->sibling->father = x->father;    }    x->sibling = nullptr;  
    x->father = nullptr;  
    return meld(root, x);  // 重新合并 x 和根节点  
}  
  
Node *delete_min(Node *x) {  
    Node *t = merges(x->child);    delete x;  
    return t;  
}  
  
void delete_all(Node *x) {  
    if (x->child) {  
        delete_all(x->child);    }    if (x->sibling) {  
        delete_all(x->sibling);    }    delete x;  
}  
  
int cnt = 0;  
//new Node({i, ++cnt, nullptr, nullptr, nullptr});
```

## 文件: 可持久化01Trie.md
```cpp
namespace Trie01 {
    int tree[MAXN * MAXBIT][2], total, root[MAXN], count[MAXN * MAXBIT];
    void insert(int id, int x) {
        root[id] = ++total;
        int cur = root[id];
        int precur = root[id - 1];
        for (int i = MAXBIT; i >= 0; --i) {
            int now = (x >> i) & 1;
            tree[cur][now] = ++total;
            tree[cur][!now] = tree[precur][!now];
            cur = tree[cur][now];
            precur = tree[precur][now];
            count[cur] = count[precur] + 1;
        }
    }
    int query(int l, int r, int x) {
        int res = 0;
        int curr = root[r], curl = root[l - 1];
        for (int i = MAXBIT; i >= 0; --i) {
            int now = (x >> i) & 1;
            if (count[tree[curr][!now]] - count[tree[curl][!now]] > 0) {
                res |= (1 << i);
                curr = tree[curr][!now];
                curl = tree[curl][!now];
            } else {
                curr = tree[curr][now];
                curl = tree[curl][now];
            }
        }
        return res;
    }
}
```

## 文件: 回滚莫队.md
```cpp
#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>
struct Query {
    int l, r, id;
};
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    std::cin >> n;
    int block_size = std::ceil(std::sqrt(n));
    std::vector<int> a(n + 1), mp(n + 1), app(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> a[i];
    }
    auto cpy_a = a;
    std::sort(cpy_a.begin() + 1, cpy_a.end());
    for (int i = 1; i <= n; ++i) {
        mp[i] = std::lower_bound(cpy_a.begin() + 1, cpy_a.end(), a[i]) - cpy_a.begin();
    }
    std::vector<Query> query(n);
    std::vector<int> ans(n);
    for (int cnt = 0; auto &q : query) {
        std::cin >> q.l >> q.r;
        q.id = cnt++;
    }
    std::ranges::sort(query, [&](auto i, auto j) {
        int bil = (i.l - 1) / block_size, bjl = (j.l - 1) / block_size;
        return bil == bjl ? i.r < j.r : bil < bjl;
    });
    auto get_bl_num = [&](int x) {
        return (x - 1) / block_size + 1;
    };
    
    int l = 1, r = 1, now_ans = 0;
    int now_bl = 1;
    app[mp[1]] = 1;
    std::vector<int> appt(n + 1);
    auto move = [&](int x) {
        ++app[mp[x]];
        if (app[now_ans] < app[mp[x]] || (app[now_ans] == app[mp[x]] && now_ans > mp[x])) {
            now_ans = mp[x];
        }
    };
    auto cancel = [&](int backup) {
        for (; l < backup; ++l) {
            --app[mp[l]];
        }
    };
    auto simple_query = [&](int l, int r) {
        int ans = 0;
        for (int i = l; i <= r; ++i) {
            ++appt[mp[i]];
            if (appt[ans] < appt[mp[i]] || (appt[ans] == appt[mp[i]] && ans > mp[i])) {
                ans = mp[i];
            }
        }  
        for (int i = l; i <= r; ++i) {
            --appt[mp[i]];
        }
        return cpy_a[ans];
    };
    auto init = [&]() {
        for (int i = l; i <= r; ++i) {
            --app[mp[i]];
        }
        now_ans = 0;
        l = std::min(now_bl * block_size, n) + 1;
        r = std::min(now_bl * block_size, n);
    };
    init();
    for (const auto &q : query) {
        auto bll = get_bl_num(q.l), brr = get_bl_num(q.r);
        if (bll > now_bl) {
            now_bl = bll;
            init();
        }
        if (bll == brr) {
            ans[q.id] = simple_query(q.l, q.r);
        } else {
            while (r < q.r) move(++r);
            int backup_ans = now_ans;
            while (l > q.l) move(--l);
            ans[q.id] = cpy_a[now_ans];
            cancel(std::min(now_bl * block_size, n) + 1);
            now_ans = backup_ans;
        }
    }
    for (const auto p : ans) {
        std::cout << p << '\n';
    }
    return 0;
}
/*
对原序列进行分块，对询问按以左端点所属块编号升序为第一关键字，右端点升序为第二关键字的方式排序。
按顺序处理询问：
如果询问左端点所属块 B 和上一个询问左端点所属块的不同，那么将莫队区间的左端点初始化为 B 的右端点加 1, 将莫队区间的右端点初始化为 𝐵
的右端点；
如果询问的左右端点所属的块相同，那么直接扫描区间回答询问；
如果询问的左右端点所属的块不同：
如果询问的右端点大于莫队区间的右端点，那么不断扩展右端点直至莫队区间的右端点等于询问的右端点；
不断扩展莫队区间的左端点直至莫队区间的左端点等于询问的左端点；
回答询问；
撤销莫队区间左端点的改动，使莫队区间的左端点回滚到 B 的右端点加 1
*/
```

## 文件: 堆.md
```c
typedef struct {
    int *arr;
    int size;
} Heap;
int* make_int_arr(int size) {
    return (int*)calloc(size, 4);
}
void init_heap(Heap* heap, int total_size) {
    heap->arr = make_int_arr(total_size);
    heap->size = 0;
}
void clear_heap(Heap* heap) {
    free(heap->arr);
    heap->size = 0;
}
void swap(int* x, int* y) {
    *x = (*x) ^ (*y);
    *y = (*x) ^ (*y);
    *x = (*x) ^ (*y);
}
void push_heap(Heap* heap, int x) {
    int* arr = heap->arr;
    arr[++(heap->size)] = x;
    int now = heap->size;
    while (now) {
        int next = now >> 1;
        if (arr[next] > arr[now]) {
            swap(arr + next, arr + now);
        } else {
            break;
        }
        now = next;
    }
}
void pop_heap(Heap* heap) {
    int* arr = heap->arr;
    swap(arr + 1, arr + heap->size);
    --(heap->size);
    int now = 1;
    while ((now << 1) <= heap->size) {
        int next = now << 1;
        if (next + 1 <= heap->size && arr[next + 1] < arr[next]) {
            ++next;
        }
        if (arr[next] < arr[now]) {
            swap(arr + next, arr + now);
        } else {
            break;
        }
        now = next;
    }
}
int get_top(Heap heap) {
    return heap.arr[1];
}
```

## 文件: 字典树Trie.md
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
namespace Trie {
    constexpr int MAX_TRIE = 3E6 + 5;
    constexpr int MAX_NODE_SIZE = 26 + 26 + 10;
    struct Node {
        int count = 0;
        int next[MAX_NODE_SIZE];
    };
    std::vector<Node> tree;
    void init() {
        if (tree.capacity() < MAX_TRIE) {
            tree.reserve(MAX_TRIE);
        }
        tree.clear();
        tree.push_back(Node{});
    }
    int newNode() {
        tree.push_back(Node{});
        return tree.size() - 1;
    }
    int get_pos(char c) {
        if (std::isupper(c)) {
            return c - 'A';
        } else if (std::islower(c)) {
            return c - 'a' + 26;
        } else {
            return c - '0' + 52;
        }
    }
    void insert(const std::string &s) {
        int cur = 0;
        for (auto c : s) {
            int pos = get_pos(c);
            if (!tree[cur].next[pos]) {
                tree[cur].next[pos] = newNode();
            }
            cur = tree[cur].next[pos];
            ++tree[cur].count;
        }
    }
    int find_prefix(const std::string &s) {
        int cur = 0;
        for (auto c : s) {
            int pos = get_pos(c);
            if (!tree[cur].next[pos]) {
                return 0;
            }
            cur = tree[cur].next[pos];
        }
        return tree[cur].count;
    }
}
//No closure ver
struct Node{
    std::unordered_map<char, int> next;
    bool isEnd;
    Node() : isEnd(0) {};
};
using TrieTree = std::vector<Node>;
auto make_TrieTree() {
    TrieTree tree;
    tree.reserve(1000000);
    tree.emplace_back();
    return tree;
}
void insert(TrieTree& tree, const std::string& word) {
    int pos = 0;
    for(auto &i : word){
        if(!tree[pos].next.count(i)){
            tree[pos].next[i] = tree.size();
            tree.emplace_back();
        }
        pos = tree[pos].next[i];
    }
    tree[pos].isEnd = 1;
}
bool find(const TrieTree& tree, const std::string& word) {
    int pos = 0;
    for(auto &i : word){
        if(!tree[pos].next.count(i)){
            return false;
        }
        pos = tree[pos].next.find(i)->second;
    }
    return tree[pos].isEnd;
}
//Closure ver
class Trie{
    struct Node{
        std::unordered_map<char, int> next;
        bool isEnd;
        Node() : isEnd(0) {};
    };
    std::vector<Node> tree;
public:
    Trie(){
        tree.emplace_back();
    }
    void insert(const std::string &word){
        int pos = 0;
        for(auto &i : word){
            if(tree[pos].next.find(i) == tree[pos].next.end()){
                tree[pos].next[i] = tree.size();
                tree.emplace_back();
            }
            pos = tree[pos].next[i];
        }
        tree[pos].isEnd = 1;
    }
    bool find(const std::string &word){
        int pos = 0;
        for(auto &i : word){
            if(tree[pos].next.find(i) == tree[pos].next.end()){
                return 0;
            }
            pos = tree[pos].next[i];
        }
        return tree[pos].isEnd;
    }
};
```

## 文件: 并查集（按秩合并）.md
```cpp
#include <vector>
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    int count; 

public:
    UnionFind(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        count = n; 
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            count--; 
        }
    }

    int getCount() const {
        return count;
    }
};
```

## 文件: 快读.md
```cpp
#include <cstdio>
#include <cctype>
char fast_getchar() {
	static char buf[1 << 20];
	static char *pos = buf, *end = buf;
	if (pos == end) {
		end = buf + fread(buf, 1, sizeof(buf), stdin);
		pos = buf;
		if (pos == end) return EOF;
	}
	return *pos++;
}
template <typename T>
T read(){
    T sum = 0, fl = 1;
    int ch = getchar();
    for(; !isdigit(ch); ch = getchar()){
        if(ch == '-') fl = -1;
    }
    for(; isdigit(ch); ch = getchar()){
        sum = sum * 10 + ch - '0';
    }
    return sum * fl;
}
struct istream{
	template<typename T>
	istream operator>>(T &x){
		x = 0;
		T fl = 1;
		int ch = getchar();
		for(; !isdigit(ch); ch = getchar()){
			if(ch == '-') fl = -1;
		}
		for(; isdigit(ch); ch = getchar()){
			x = x * 10 + ch - '0';
		}
		return *this;
	}	
} cin;
void read(auto& x) {
    using x_type = std::remove_cvref_t<decltype(x)>;
    if constexpr (std::is_same<x_type, std::string>::value || std::is_same<x_type, char*>::value) {
        x = fr.fast_gets();
    } else if constexpr (std::is_arithmetic<x_type>::value) {
        x_type fl = 1;
        x = 0;
        int ch = fr.fast_getchar();
        for(; !isdigit(ch); ch = fr.fast_getchar()){
            if (ch == '-') fl = -1;
        }
        for(; isdigit(ch); ch = fr.fast_getchar()){
            x = x * 10 + ch - '0';
        }
        x *= fl;
    }
}
```

## 文件: 快速幂.md
```cpp
using i64 = long long;

constexpr i64 mod = 1e9 + 7;
i64 qpow(i64 a, i64 b) {
    i64 result = 1;
    a = a % mod;
    while (b > 0) {
        if (b & 1) {
            result = (result * a) % mod;
        }
        a = (a * a) % mod;
        b >>= 1;
    }
    return result;
}
```

## 文件: 扫描线+线段树.md
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
struct Line{
    ll l, r, h, mark;
};
struct SegTree{
    #define ls(p) (p << 1)
    #define rs(p) (p << 1 | 1)
    struct Node{
        ll l, r, cnt, len;
    };
    vector<Node> tree;
    vector<ll> &vecx;
    int range;
    void build(int p, int l, int r){
        tree[p].l = l; tree[p].r = r;
        if(l == r){
            return ;
        }
        int mid = (l + r) >> 1;
        build(ls(p), l, mid);
        build(rs(p), mid + 1, r);
    }
    SegTree(vector<ll> &_vecx) : vecx(_vecx){
        auto it = unique(vecx.begin() + 1, vecx.end());
        range = it - vecx.begin() - 2;
        tree.assign(range << 2, {});
        build(1, 1, range);
    }
    void pushup(int p){
        if(tree[p].cnt)
            tree[p].len = vecx[tree[p].r + 1] - vecx[tree[p].l];
        else if(tree[p].l != tree[p].r)
            tree[p].len = tree[ls(p)].len + tree[rs(p)].len;
        else
            tree[p].len = 0;
    }
    void update(int p, ll L, ll R, ll flag){
        if(vecx[tree[p].r + 1] <= L || R <= vecx[tree[p].l]){
            return ;
        }
        if(L <= vecx[tree[p].l] && vecx[tree[p].r + 1] <= R){
            tree[p].cnt += flag;
            pushup(p);
            return ;
        }
        update(ls(p), L, R, flag);
        update(rs(p), L, R, flag);
        pushup(p);
    }
    void update(ll l, ll r, ll val){
        update(1, l, r, val);
    }
    ll query_total(){
        return tree[1].len;
    }
    #undef ls
    #undef rs
};
int main(){
    //freopen("testin.txt", "r", stdin);
    cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    cin >> n;
    vector<Line> line;
    vector<ll> vecx{0};
    ll x1, y1, x2, y2;
    for(int i = 1; i <= n; i++){
        cin >> x1 >> y1 >> x2 >> y2;
        vecx.push_back(x1); vecx.push_back(x2);
        line.push_back({x1, x2, y1, 1ll});
        line.push_back({x1, x2, y2, -1ll});
    }
    ranges::sort(vecx);
    SegTree sgt(vecx);
    ranges::sort(line, [](const Line &_l, const Line &_r){
        return _l.h < _r.h;
    });
    ll ans = 0;
    for(int i = 0; i < line.size() - 1; i++){
        sgt.update(line[i].l, line[i].r, line[i].mark);
        ans += sgt.query_total() * (line[i + 1].h - line[i].h);
        //cout << line[i].l << ' ' << line[i].r << ' ' << line[i].h << ' ' << line[i + 1].h << ' ' << sgt.query_total() << '\n';
    }
    cout << ans << '\n';
    return 0;
}
```

## 文件: 拉格朗日插值.md
```cpp
i64 qpow(i64 a, i64 b) {
    i64 res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}
i64 inv(i64 x) {
    if (x < 0) x += mod;
    return qpow(x, mod - 2);
}\
auto lagrange(const std::vector<std::pair<i64, i64>> &ps, i64 k) {
    int n = ps.size();
    i64 res = 0;
    for (int i = 0; i < n; ++i) {
        i64 up = 1, down = 1;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            up = up * ((k - ps[j].first) + mod) % mod;
            down = down * inv(ps[i].first - ps[j].first + mod) % mod;
        }
        res = (res + ((ps[i].second * up % mod) * down % mod)) % mod;
    }
    return res;
}
```

## 文件: 拓扑排序.md
```cpp
auto toposort(){
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
```

## 文件: 最小生成树.md
```cpp
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
```

## 文件: 最小费用最大流.md
```cpp
constexpr auto INF = std::numeric_limits<i64>::max();
class MinCostMaxFlow {
    struct Edge {
        int to, rev;
        i64 cap, cost;
    };
    std::vector<std::vector<Edge>> adj;
    std::vector<i64> dist;
    std::vector<int> prevv, preve;
    bool spfa(int s, int t, i64& flow, i64& cost) {
        std::ranges::fill(dist, INF);
        std::ranges::fill(prevv, -1);
        std::ranges::fill(preve, -1);
        int n = adj.size();
        std::vector<bool> in_queue(n);
        std::queue<int> que;
        dist[s] = 0;
        que.push(s);
        in_queue[s] = true;
        while (!que.empty()) {
            int now = que.front();
            que.pop();
            in_queue[now] = false;
            for (int i = 0; auto& next : adj[now]) {
                if (next.cap > 0 && dist[next.to] > dist[now] + next.cost) {
                    dist[next.to] = dist[now] + next.cost;
                    prevv[next.to] = now;
                    preve[next.to] = i;
                    if (!in_queue[next.to]) {
                        que.push(next.to);
                        in_queue[next.to] = true;
                    }
                }
                ++i;
            }
        }
        if (dist[t] == INF) {
            return false;
        }
        i64 d = INF;
        for (int v = t; v != s; v = prevv[v]) {
            d = std::min(d, adj[prevv[v]][preve[v]].cap);
        }
        flow += d;
        cost += d * dist[t];
        for (int v = t; v != s; v = prevv[v]) {
            auto& e = adj[prevv[v]][preve[v]];
            e.cap -= d;
            adj[v][e.rev].cap += d;
        }
        return true;
    }

public:
    MinCostMaxFlow(int n) : adj(n), dist(n), prevv(n), preve(n) {}
    void add_edge(int u, int v, i64 cap, i64 cost) {
        adj[u].push_back({v, int(adj[v].size()), cap, cost});
        adj[v].push_back({u, int(adj[u].size() - 1), 0, -cost});
    }
    auto get_mcmf(int s, int t) {
        i64 flow = 0, cost = 0;
        while (spfa(s, t, flow, cost)) ;
        return std::pair(flow, cost);
    }
};
```

## 文件: 权值线段树（维护值域）.md
```cpp
struct segtree{
    struct Node{
        int sum, ls, rs, l, r;
        Node(){
            sum = ls = rs = l = r = 0;
        }
    };
    vector<Node> tree;
    int range;
    segtree(int _range) : range(_range){
        tree.reserve(int(1e7 + 8));
        tree.emplace_back();
    }
    void check(int &p){
        if(!p){
            p = tree.size();
            tree.emplace_back();
        }
    }
    void update(int p, int l, int r, int now){
        tree[p].sum++;
        if(l == r){
            return ;
        }
        int mid = (l + r) >> 1;
        if(now <= mid){
            check(tree[p].ls);
            update(tree[p].ls, l, mid, now);
        }else{
            check(tree[p].rs);
            update(tree[p].rs, mid + 1, r, now);
        }
    }
    int query(int p, int l, int r, int fl, int fr){
        if(!p) return 0;
        if(fl <= l && r <= fr) return tree[p].sum;
        int res = 0, mid = (l + r) >> 1;
        if(fl <= mid){
            res += query(tree[p].ls, l, mid, fl, fr);
        }
        if(mid < fr){
            res += query(tree[p].rs, mid + 1, r, fl, fr);
        }
        return res;
    }
    int query_kth(int p, int l, int r, int k){
        if(!p || tree[p].sum == 0) return -1;
        if(l == r) return l;
        int mid = (l + r) >> 1;
        int lsize = tree[p].ls ? tree[tree[p].ls].sum : 0;
        if(lsize >= k){
            return query_kth(tree[p].ls, l, mid, k);
        }else{
            return query_kth(tree[p].rs, mid + 1, r, k - lsize);
        }
    }
    void update(int x){
        check(tree[0].ls);
        update(tree[0].ls, 1, range, x);
    }
    int query(int l, int r){
        if(l > r) return 0;
        return query(tree[0].ls, 1, range, l, r);
    }
};
```

## 文件: 树状数组.md
```cpp
class binaryIndexedTree{
    ll lowbit(ll x){
        return x & -x;
    }
    const vector<ll> &org;
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
    binaryIndexedTree(const vector<ll> &_org, int _size) : tree(_size + 5), org(_org) { //initialize
        size = _size;
        for(int i = 1; i <= size; i++){
            for(int j = i; j <= size; j += lowbit(j)){ //the father of tree[j] is tree[j + lowbit(j)]
                tree[j] += org[i];
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
```

## 文件: 树状数组套权值线段树.md
```cpp
using i64 = long long;
const int MAXN = 40000000;
class SegTree {
    struct Node {
        int val;
        int ls, rs;
    };
    static const int range = int(1e9);
    static int count;
    static Node tree[MAXN];
    int insert_new_node() {
        tree[count] = {0, -1, -1};
        return count++;
    }
    int query(int p, int l, int r, int fl, int fr) {
        if (p == -1) return 0;
        if (fl <= l && r <= fr) {
            return tree[p].val;
        }
        int mid = (l + r) >> 1;
        int res = 0;
        if (fl <= mid) {
            res += query(tree[p].ls, l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(tree[p].rs, mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int p, int l, int r, int pos, int val) {
        tree[p].val += val;
        if (l == r) {
            return ;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            if (tree[p].ls == -1) {
                tree[p].ls = insert_new_node();
            }
            update(tree[p].ls, l, mid, pos, val);
        }
        if (mid < pos) {
            if (tree[p].rs == -1) {
                tree[p].rs = insert_new_node();
            }
            update(tree[p].rs, mid + 1, r, pos, val);
        }
    }
    static int query_kth_from_all(std::vector<int>& p_vec_l, std::vector<int>& p_vec_r, int l, int r, int k) {
        int lsize = 0, nowsize = 0;
        for (auto p : p_vec_r) {
            if (p == -1) continue;
            nowsize += tree[p].val;
            lsize += (tree[p].ls == -1 ? 0ll : tree[tree[p].ls].val);
        }
        for (auto p : p_vec_l) {
            if (p == -1) continue;
            nowsize -= tree[p].val;
            lsize -= (tree[p].ls == -1 ? 0ll : tree[tree[p].ls].val);
        }
        if (nowsize == 0) return -1;
        if (l == r) return l;
        int mid = (l + r) >> 1;
        
        if (lsize >= k) {
            for (auto& p : p_vec_l) {
                if (p != -1) {
                    p = tree[p].ls;
                }
            }
            for (auto& p : p_vec_r) {
                if (p != -1) {
                    p = tree[p].ls;
                }
            }
            return query_kth_from_all(p_vec_l, p_vec_r, l, mid, k);
        } else {
            for (auto& p : p_vec_l) {
                if (p != -1) {
                    p = tree[p].rs;
                }
            }
            for (auto& p : p_vec_r) {
                if (p != -1) {
                    p = tree[p].rs;
                }
            }
            return query_kth_from_all(p_vec_l, p_vec_r, mid + 1, r, k - lsize);
        }
    }
public:
    int root;
    SegTree() {
        root = insert_new_node();
    }
    int query(int l, int r) {
        return query(root, 0, range, l, r);
    }
    void update(int pos, i64 val) {
        update(root, 0, range, pos, val);
    } 
    static int query_kth_from_all(std::vector<int>& p_vec_l, std::vector<int>& p_vec_r, int k) {
        return query_kth_from_all(p_vec_l, p_vec_r, 0, range, k);
    }
};
SegTree::Node SegTree::tree[MAXN];
int SegTree::count = 0;
class Seg_in_Fenwick {
    std::vector<SegTree> tree;
    int range;
    static int lowbit(int x) {
        return x & -x;
    }
    auto get_tree_node(int ind) {
        std::vector<int> p;
        while (ind) {
            p.push_back(tree[ind].root);
            ind -= lowbit(ind);
        }
        return p;
    }
public:
    Seg_in_Fenwick(int size) : range(size), tree(size + 1) {}
    void update(int ind, int seg_ind, int x) {
        for (int i = ind; i <= range; i += lowbit(i)) {
            tree[i].update(seg_ind, x);
        }
    }
    int query_kth_range(int l, int r, int k) {
        auto pl = get_tree_node(l - 1), pr = get_tree_node(r);
        return SegTree::query_kth_from_all(pl, pr, k);
    }
};
```

## 文件: 树状数组套线段树.md
```cpp
using i64 = long long;
int lowbit(int x) {
    return x & -x;
}
class SegTree {
    struct Node {
        i64 val;
        int ls = -1, rs = -1;
    };
    int root;
    static std::vector<Node> tree;
    int insert_new_node() {
        tree.push_back({0, -1, -1});
        return tree.size() - 1;
    }
    i64 query(int p, int l, int r, int fl, int fr) {
        if (p == -1) return 0;
        if (fl <= l && r <= fr) {
            return tree[p].val;
        }
        int mid = (l + r) >> 1;
        i64 res = 0;
        if (fl <= mid) {
            res += query(tree[p].ls, l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(tree[p].rs, mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int p, int l, int r, int fl, int fr, i64 x) {
        ++tree[p].val;
        if (fl <= l && r <= fr) {
            return ;
        }
        int mid = (l + r) >> 1;
        if (fl <= mid) {
            if (tree[p].ls == -1) {
                tree[p].ls = insert_new_node();
            }
            update(tree[p].ls, l, mid, fl, fr, x);
        }
        if (mid < fr) {
            if (tree[p].rs == -1) {
                tree[p].rs = insert_new_node();
            }
            update(tree[p].rs, mid + 1, r, fl, fr, x);
        }
    }
public:
    static int range;
    SegTree() {
        root = insert_new_node();
    }
    i64 query(int l, int r) {
        return query(root, 1, range, l, r);
    }
    void update(int l, int r, i64 x) {
        update(root, 1, range, l, r, x);
    } 
};
int SegTree::range;
std::vector<SegTree::Node> SegTree::tree;
class Seg_in_Fenwick {
    std::vector<SegTree> tree;
    int range;
    i64 query(int x_ind, int y_l, int y_r) {
        i64 res = 0;
        while (x_ind) {
            res += tree[x_ind].query(y_l, y_r);
            x_ind -= lowbit(x_ind);
        }
        return res;
    }
public:
    Seg_in_Fenwick(int size, int sgt_size) : range(size), tree(size + 1) {
        SegTree::range = sgt_size;
    }
    void update(int x_ind, int y_l, int y_r, i64 x) {
        for (int i = x_ind; i <= range; i += lowbit(i)) {
            tree[i].update(y_l, y_r, x);
        }
    }
    i64 query(int x_l, int x_r, int y_l, int y_r) {
        return query(x_r, y_l, y_r) - query(x_l, y_l, y_r);
    }
};
```

## 文件: 树链剖分.md
```cpp
using GraphEdge = std::vector<std::vector<int>>;
using i64 = long long;
struct SegTree {
    std::vector<i64> tree, tag;
    int size;
    SegTree(int size) : size(size), tree(size << 2), tag(size << 2) {}
    #define ls(x) (x << 1)
    #define rs(x) (x << 1 | 1)
    void pushup(int p) {
        tree[p] = tree[ls(p)] + tree[rs(p)];
    }
    void pushdown(int p, int l, int r) {
        if (tag[p] && l != r) {
            int mid = (l + r) >> 1;
            tree[ls(p)] += tag[p] * (mid - l + 1);
            tree[rs(p)] += tag[p] * (r - mid);
            tag[ls(p)] += tag[p];
            tag[rs(p)] += tag[p];
            tag[p] = 0;
        }
    }
    void update(int p, int l, int r, int fl, int fr, i64 x) {
        if (fl <= l && r <= fr) {
            tree[p] += x * (r - l + 1);
            tag[p] += x;
            return ;
        }
        pushdown(p, l, r);
        int mid = (l + r) >> 1;
        if (fl <= mid) {
            update(ls(p), l, mid, fl, fr, x);
        }
        if (mid < fr) {
            update(rs(p), mid + 1, r, fl, fr, x);
        }
        pushup(p);
    } 
    i64 query(int p, int l, int r, int fl, int fr) {
        if (fl <= l && r <= fr) {
            return tree[p];
        }
        pushdown(p, l, r);
        int mid = (l + r) >> 1;
        i64 res = 0;
        if (fl <= mid) {
            res += query(ls(p), l, mid, fl, fr);
        }
        if (mid < fr) {
            res += query(rs(p), mid + 1, r, fl, fr);
        }
        return res;
    }
    void update(int l, int r, i64 x) {
        update(1, 1, size, l, r, x);
    }
    i64 query(int l, int r) {
        return query(1, 1, size, l, r);
    }
    #undef ls
    #undef rs
};
struct TreeDecomposition {
    const GraphEdge& org_tree;
    const std::vector<i64>& org_seq;
    std::vector<int> fa, dep, size, hson, top, dfn, rnk;
    int n, dfs_cnt, root;
    SegTree sgt;
    int tree_build(int u, int nowdepth, int pre) {
        dep[u] = nowdepth;
        size[u] = 1;
        for (auto v : org_tree[u]) {
            if (v == pre) continue;
            size[u] += tree_build(v, nowdepth + 1, u);
            fa[v] = u;
            if (size[v] > size[hson[u]]) {
                hson[u] = v;
            }
        }
        return size[u];
    }
    void tree_get_top(int u, int nowtop, int pre) {
        top[u] = nowtop;
        dfn[u] = ++dfs_cnt;
        rnk[dfs_cnt] = u;
        if (hson[u])
            tree_get_top(hson[u], nowtop, u);
        for (auto v : org_tree[u]) {
            if (v == pre || v == hson[u]) continue;
            tree_get_top(v, v, u);
        }
    }
    TreeDecomposition(const GraphEdge& tree, int n, int root, const std::vector<i64>& org_seq) : 
    org_tree(tree), n(n), fa(n + 1), dep(n + 1), size(n + 1), hson(n + 1), top(n + 1), dfn(n + 1), rnk(n + 1), dfs_cnt(0), root(root), sgt(n), org_seq(org_seq)
    {
        tree_build(root, 0, 0);
        tree_get_top(root, root, 0);
        for (int i = 1; i <= n; ++i) {
            sgt.update(dfn[i], dfn[i], org_seq[i]);
        }
    }
    i64 tree_path_sum(int u, int v) {
        i64 res = 0;
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                std::swap(u, v);
            }
            res += sgt.query(dfn[top[u]], dfn[u]);
            u = fa[top[u]];
        }
        auto [up, down] = std::minmax(dfn[u], dfn[v]);
        res += sgt.query(up, down);
        return res;
    }
    void tree_path_update(int u, int v, i64 x) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                std::swap(u, v);
            }
            sgt.update(dfn[top[u]], dfn[u], x);
            u = fa[top[u]];
        }
        auto [up, down] = std::minmax(dfn[u], dfn[v]);
        sgt.update(up, down, x);
    }
    i64 subtree_sum(int u) {
        return sgt.query(dfn[u], dfn[u] + size[u] - 1);
    }
    void subtree_update(int u, i64 x) {
        sgt.update(dfn[u], dfn[u] + size[u] - 1, x);
    }
};
```

## 文件: 树链剖分（未完成）.md
```cpp
#include<bits/stdc++.h>  
  
using namespace std;  
  
struct Tree_cut{  
    vector<int> fa, dep, siz, son, top, dfn, rnk;  
    vector<vector<int>>h;  
    int idx;  
    Tree_cut(int n){  
        h.assign(n+1,vector<int>());  
        fa.assign(n + 1, 0);  
        dep.assign(n + 1, 0);  
        siz.assign(n + 1, 0);  
        son.assign(n + 1, 0);  
        top.assign(n + 1, 0);  
        dfn.assign(n + 1, 0);  
        rnk.assign(n + 1, 0);  
        idx = 0;  
    }  
    void add(int x,int y){  
        h[x].push_back(y);  
        h[y].push_back(x);  
    }  
    void dfs1(int u, int f) {  
        fa[u] = f, dep[u] = dep[f] + 1, siz[u] = 1;  
        for (auto v : h[u]) {  
            if (v == f) continue;  
            dfs1(v, u);  
            siz[u] += siz[v];  
            if (siz[v] > siz[son[u]]) son[u] = v;  
        }  
    }  
  
    void dfs2(int u, int ftop) {  
        top[u] = ftop, dfn[u] = ++idx, rnk[idx] = u;  
        if (son[u]) dfs2(son[u], ftop);  
        for (auto v : h[u])  
            if (v != son[u] && v != fa[u]) dfs2(v, v);  
    }  
    void work(int s){  
        dfs1(s,0);  
        dfs2(s,s);  
    }  
    int lca(int u, int v) {  
        while (top[u] != top[v]) {  
            if (dep[top[u]] > dep[top[v]])  
                u = fa[top[u]];  
            else  
                v = fa[top[v]];  
        }  
        return dep[u] > dep[v] ? v : u;  
    }  
    //路径信息维护  
    auto query(int u,int v){  
        int tot=0;  
        while(top[u]!=top[v]){  
            if(dep[top[u]]<dep[top[v]])swap(u,v);  
            tot+=myquery(u,top[u]);  
            u=fa[top[u]];  
        }  
        tot+=myquery(u,v);  
        return tot;  
    }  
      
};  
  
int main() {  
    int n,m,s;  
    cin>>n>>m>>s;  
    Tree_cut ct(n);  
    for(int i=1;i<n;i++){  
        int x,y;  
        cin>>x>>y;  
        ct.add(x,y);  
    }  
    ct.work(s);  
    for(int i=1;i<=m;i++){  
        int u,v;  
        cin>>u>>v;  
        cout<<ct.lca(u,v)<<"\n";  
    }  
}
```

## 文件: 求简单多边形面积.md
```cpp
auto polygon_size(const std::vector<Point> &pts) { //需要保证点按逆时针顺序给出
    int n = pts.size();
    i64 sum = 0;
    for (int i = 1; i < n; ++i) {
        sum += pts[i].y * pts[i - 1].x - pts[i].x * pts[i - 1].y;
    }
    sum += pts[0].y * pts[n - 1].x - pts[0].x * pts[n - 1].y;
    return std::abs(sum) / 2;
}
```

## 文件: 求组合数.md
```cpp
class ConbinatorialNumber {
    i64 pret[MAXN], inv[MAXN];
public:
    ConbinatorialNumber() {
        pret[0] = 1;
        for (i64 i = 1; i < MAXN; ++i) {
            pret[i] = pret[i - 1] * i % mod;
        }
        inv[MAXN - 1] = qpow(pret[MAXN - 1], mod - 2);
        for (i64 i = MAXN - 2; i >= 0; --i) {
            inv[i] = inv[i + 1] * (i + 1) % mod;
        }
    }
    i64 operator()(i64 n, i64 m) const {
        if(m == 0) return 1;
        return (((pret[n] * inv[m]) % mod) * inv[n - m]) % mod; 
    }
} C;
```

## 文件: 线性素数筛.md
```cpp
constexpr int M = 1e8 + 1;
using i64 = long long;
std::bitset<M> prime_flag;
std::vector<int> prime_list;
struct Prime_Initor {
    Prime_Initor() {
        prime_flag.set();
        prime_flag[1] = 0;
        for(int i = 2; i < M; i++){
            if(prime_flag[i]){
                prime_list.push_back(i);
            }
            for(auto j : prime_list){
                if(i * j >= M) break;
                prime_flag[i * j] = 0;
                if(i % j == 0) break;
            }
        }
    }
} prime_initor;
```

## 文件: 线性逆元.md
```cpp
constexpr int MAXN = 1000005;
using i64 = long long;
constexpr i64 mod = 1e9 + 7;
i64 inv_arr[MAXN];
struct Inv_initor {
    Inv_initor() {
        inv_arr[0] = inv_arr[1] = 1;
        for (i64 i = 2; i < MAXN; ++i) {
            inv_arr[i] = (mod - mod / i * inv_arr[mod % i] % mod) % mod;
        }
    }
} inv_initor;
```

## 文件: 线段树上二分.md
```cpp
// merge(x, y): 合并两个区间，即pushup，定义merge(x, y, z) = merge(merge(x, y), z)
// max_right(fl): 对于一个给定的函数bool f(Node x)，返回一个fr，满足f(merge(a[fl], a[fl + 1], a[fl + 2]...)) = true，而f(a[fr]) = false
// 同理，可实现min_left(fr)
auto max_right = [&](auto &&self, Node &node, int p, int l, int r) -> int {
    if (r < fl)
        return -1;
    if (fl <= l) {
        auto now_node = merge(node, tree[p]);
        if (f(now_node)) {
            node = now_node;
            flag = true;
            return -1;
        }
        if (l == r) {
            return l;
        }
    }
    push_down(p, l, r);
    int mid = (l + r) >> 1;
    auto res = self(self, node, ls(p), l, mid);
    if (res != -1)
        return res;
    return self(self, node, rs(p), mid + 1, r);
};
auto min_left = [&](auto &&self, Node &node, int p, int l, int r) -> int {
    if (l > fr)
        return -1;
    if (r <= fr) {
        auto now_node = merge(node, tree[p]);
        if (f(now_node)) {
            node = now_node;
            flag = true;
            return -1;
        }
        if (l == r) {
            return l;
        }
    }
    push_down(p, l, r);
    int mid = (l + r) >> 1;
    auto res = self(self, node, rs(p), mid + 1, r);
    if (pos != -1)
        return res;
    return self(self, node, ls(p), l, mid);
};
```

## 文件: 线段树合并.md
```cpp
#include<bits/stdc++.h>

#define int long long
using namespace std;
template<typename T>
class LCA{
    using VT = std::vector<T>;
    using VVT = std::vector<VT>;
    VT deep,lg2;
    VVT fa;
    std::vector<int> vis;
    void dfs(T x,T y,const VVT &h){
        if(vis[x]) return;
        vis[x] = true;
        deep[x]=deep[y]+1;
        fa[x][0]=y;
        for(int i=1; i<=lg2[deep[x]]; i++)
            fa[x][i]=fa[fa[x][i-1]][i-1];
        for(auto i:h[x]){
            if(i!=y)dfs(i,x,h);
        }
    }
public:
    LCA(int n,const VVT &h,int s=1){
        deep.assign(n+1,0);
        lg2.assign(n+1,0);
        fa.assign(n+1,VT(21,0));
        vis.assign(n+1,0);
        for(int i=2; i<=n; i++) {
            lg2[i] = lg2[i / 2] + 1;
        }
        dfs(s,0,h);
    }
    T lca(int x,int y){
        if(deep[x]<deep[y])std::swap(x,y);
        while(deep[x]!=deep[y])
            x=fa[x][lg2[deep[x]-deep[y]]];
        if(x==y)return x;
        for(int k=lg2[deep[x]]; k>=0; k--)
            if(fa[x][k]!=fa[y][k])
            {
                x=fa[x][k];
                y=fa[y][k];
            }
        return fa[x][0];
    }
    T findx(int x){
        return fa[x][0];
    }
};

struct tree{
    int l,r,lf,rf,mc,mt;
};
vector<int>head,ans;
int cnt=0;
void pushup(int p,vector<tree>& trr){
    trr[p].mc=trr[p].mt=0;
    if(trr[p].lf) {
        if(trr[trr[p].lf].mc>trr[p].mc){
            trr[p].mc=trr[trr[p].lf].mc;
            trr[p].mt=trr[trr[p].lf].mt;
        }
    }
    if(trr[p].rf) {
        if(trr[trr[p].rf].mc>trr[p].mc){
            trr[p].mc=trr[trr[p].rf].mc;
            trr[p].mt=trr[trr[p].rf].mt;
        }
        else if(trr[trr[p].rf].mc==trr[p].mc&&trr[trr[p].rf].mt<trr[p].mt)trr[p].mt=trr[trr[p].rf].mt;
    }
}
vector<vector<int>>h;
void search(int p,int z,int type,vector<tree>& trr){
    if(trr[p].l==trr[p].r){
        trr[p].mc+=type;
        if(trr[p].mc<=0)trr[p].mt=0;
        else trr[p].mt=trr[p].l;
        return;
    }
    int mid=(trr[p].l+trr[p].r)>>1;
    if(z<=mid){
        if(trr[p].lf)search(trr[p].lf,z,type,trr);
        else{
            trr[++cnt]={trr[p].l,mid,0,0,0,0};
            trr[p].lf=cnt;
            search(trr[p].lf,z,type,trr);
        }
    }
    else {
        if(trr[p].rf)search(trr[p].rf,z,type,trr);
        else{
            trr[++cnt]={mid+1,trr[p].r,0,0,0,0};
            trr[p].rf=cnt;
            search(trr[p].rf,z,type,trr);
        }
    }
    pushup(p,trr);
}
void modify(int x,int z,int type,vector<tree>& tr){
    if(!head[x]){
        tr[++cnt]={1,100000,0,0,0,0};
        head[x]=cnt;
    }
    search(head[x],z,type,tr);
}
auto merge(int a,int b,vector<tree>& tr){
    if(!a)return b;
    if(!b)return a;
    if(tr[a].l==tr[a].r){
        tr[a].mc+=tr[b].mc;
        if(tr[a].mc>0)tr[a].mt=tr[a].l;
        return a;
    }
    tr[a].lf=merge(tr[a].lf,tr[b].lf,tr);
    tr[a].rf=merge(tr[a].rf,tr[b].rf,tr);
    pushup(a,tr);
    return a;
}
void dfs(int x,int y,vector<tree>& tr){
    for(auto i:h[x]){
        if(i==y)continue;
        dfs(i,x,tr);
        head[x]=merge(head[x],head[i],tr);
    }
    ans[x]=tr[head[x]].mt;
}
void solve() {
    int n,m;
    cin>>n>>m;
    vector<tree>tr(m*100);
    head.assign(n+1,0);
    ans.assign(n+1,0);
    h.assign(n+1,vector<int>());
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        h[u].push_back(v);
        h[v].push_back(u);
    }
    LCA L(n,h);
    for(int i=1;i<=m;i++){
        int x,y,z;
        cin>>x>>y>>z;
        int lca=L.lca(x,y);
        if(x==y){
            modify(x,z,1,tr);
            modify(L.findx(x),z,-1,tr);
        }
        else if(x==lca){
            modify(y,z,1,tr);
            modify(L.findx(x),z,-1,tr);
        }
        else if(y==lca){
            modify(x,z,1,tr);
            modify(L.findx(y),z,-1,tr);
        }
        else {
            modify(x,z,1,tr);
            modify(y,z,1,tr);
            modify(lca,z,-1,tr);
            modify(L.findx(lca),z,-1,tr);
        }
    }
    dfs(1,0,tr);
    for(int i=1;i<=n;i++)cout<<ans[i]<<"\n";
}

signed main() {
    ios::sync_with_stdio(false);cin.tie(nullptr);cout.tie(nullptr);
    int t;
//    cin>>t;
    t=1;
    while (t--)solve();
}
```

## 文件: 网络最大流.md
封装版本
```cpp
class NetworkMaxFlow {
    struct Edge {
        i64 capacity;
        int to, rev;
    };
    std::vector<std::vector<Edge>> edge;
    std::vector<int> level;
    std::vector<int> cur;
    bool bfs(int s, int t) {
        std::fill(level.begin(), level.end(), -1);
        level[s] = 0;
        std::queue<int> que;
        que.push(s);
        while (!que.empty()) {
            int now = que.front();
            que.pop();
            for (auto& e : edge[now]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[now] + 1;
                    que.push(e.to);
                }

            }
        }
        return level[t] != -1;
    }
    i64 dfs(int v, int t, i64 f) {
        if (v == t || f == 0) return f;
        for (auto& i = cur[v]; i < edge[v].size(); ++i) {
            auto& e = edge[v][i];
            if (level[v] < level[e.to] && e.capacity > 0) {
                i64 d = dfs(e.to, t, std::min(f, e.capacity));
                if (d > 0) {
                    e.capacity -= d;
                    edge[e.to][e.rev].capacity += d;
                    return d;
                }
            }
        }
        return 0ll;
    }
public:
    NetworkMaxFlow(int n) : edge(n), level(n), cur(n) {}
    void add_edge(int u, int v, i64 cap) {
        edge[u].push_back({cap, v, int(edge[v].size())});
        edge[v].push_back({0, u, int(edge[u].size() - 1)});
    }
    i64 get_max_flow(int s, int t) {
        i64 flow = 0;
        while (bfs(s, t)) {
            std::fill(cur.begin(), cur.end(), 0);
            while (i64 f = dfs(s, t, std::numeric_limits<i64>::max())) {
                flow += f;
            }
        }
        return flow;
    }
};
```
无封装单函数版本
```cpp
i64 max_flow(const GraphEdge& org_edge, int s, int t) {
    struct Edge_ {
        i64 capacity; int to, rev;
    };
    int n = org_edge.size();
    std::vector<std::vector<Edge_>> edge(n);
    for (int u = 0; u < org_edge.size(); ++u) {
        for (auto v : org_edge[u]) {
            edge[u].push_back({v.w, v.to, int(edge[v.to].size())});
            edge[v.to].push_back({0, u, int(edge[u].size() - 1)});
        }
    }
    std::vector<int> level(n), cur(n);
    auto bfs = [&](int s, int t) {
        std::fill(level.begin(), level.end(), -1);
        level[s] = 0;
        std::queue<int> que;
        que.push(s);
        while (!que.empty()) {
            int now = que.front();
            que.pop();
            for (auto& e : edge[now]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[now] + 1;
                    que.push(e.to);
                }

            }
        }
        return level[t] != -1;
    };
    auto dfs = [&](auto&& self, int v, int t, i64 f) -> i64 {
        if (v == t || f == 0ll) return f;
        for (auto& i = cur[v]; i < edge[v].size(); ++i) {
            auto& e = edge[v][i];
            if (level[v] < level[e.to] && e.capacity > 0) {
                i64 d = self(self, e.to, t, std::min(f, e.capacity));
                if (d > 0) {
                    e.capacity -= d;
                    edge[e.to][e.rev].capacity += d;
                    return d;
                }
            }
        }
        return 0ll;
    };
    i64 flow = 0;
    while (bfs(s, t)) {
        std::fill(cur.begin(), cur.end(), 0);
        while (i64 f = dfs(dfs, s, t, std::numeric_limits<i64>::max())) {
            flow += f;
        }
    }
    return flow;
}
```

## 文件: 高斯消元（模非质数p意义下的辗转相减法）.md
```cpp
#include<bits/stdc++.h>  
using namespace std;  
#define int long long  
int p;  
int Gs(vector<vector<int>> G){  
    int n=G.size()-1;  
    int res=1,w=1;  
    for(int i=1;i<=n;i++){  
        for(int j=i+1;j<=n;j++){  
            while(G[i][i]){  
                int div=G[j][i]/G[i][i];  
                for(int k=i;k<=n;k++){  
                    G[j][k]=(G[j][k]-div*G[i][k]%p+p)%p;  
                }  
                swap(G[i],G[j]);w=-w;  
            }  
            swap(G[i],G[j]);w=-w;  
        }  
    }  
    for(int i=1;i<=n;i++){  
        res*=G[i][i];  
        res%=p;  
    }  
    res*=w;  
    return (res+p)%p;  
}
```

## 文件: 高精度模板.md
```cpp
#include<bits/stdc++.h>  
using namespace std;  
constexpr int MAXN = 1e3;  
  
string sum(string a, string b)  
{  
    if (a.size() > b.size() || (a.size() == b.size() && a > b)) swap(a, b);  
    vector<int> res(MAXN + 3, 0);  
    int pos = 0;  
    for (int i = a.size() - 1; i >= 0; i--)  
    {  
        res[pos++] = a[i] - '0';  
    }  
    for (int i = b.size() - 1; i >= 0; i--)  
    {  
        res[b.size() - 1 - i] += b[i] - '0';  
        res[b.size() - i] += res[b.size() - 1 - i] / 10;  
        res[b.size() - 1 - i] %= 10;  
    }  
    bool f = 0;  
    string ans = "";  
    for (int i = MAXN; i >= 0; i--)  
    {  
        if (res[i] != 0)  
        {  
            f = 1;  
        }  
        if (f) ans += res[i] + '0';  
    }  
    return ans == "" ? "0" : ans;  
}  
  
string sub(string a, string b)  
{  
    auto apos = a.find_first_not_of('0');  
    auto bpos = b.find_first_not_of('0');  
    if (apos != string::npos) a = a.substr(apos);  
    if (bpos != string::npos) b = b.substr(bpos);  
    bool mark = 0, f = 0;  
    int pos = 0;  
    vector<int> res(1e3 + 5, 0);  
    if (a.size() < b.size() || (a.size() == b.size() && a < b))  
    {  
        mark = 1;  
        swap(a, b);  
    }  
    for (int i = a.size() - 1; i >= 0; i--)  
    {  
        res[pos++] += a[i] - '0';  
    }  
    for (int i = b.size() - 1; i >= 0; i--)  
    {  
        if (res[b.size() - 1 - i] - (b[i] - '0') < 0)  
        {  
            res[b.size() - 1 - i] += 10;  
            res[b.size() - i] -= 1;  
        }  
        res[b.size() - 1 - i] -= b[i] - '0';  
    }  
    for (int i = b.size(); i <= MAXN; i++)  
    {  
        if (res[i] < 0)  
        {  
            res[i] += 10;  
            res[i + 1] -= 1;  
        }  
    }  
    string ans = mark ? "-" : "";  
    for (int i = MAXN; i >= 0; i--)  
    {  
        if (res[i] != 0)  
        {  
            f = 1;  
        }  
        if (f) ans += res[i] + '0';  
    }  
    return (ans == "" || ans == "-") ? "0" : ans;  
}  
  
string mul(string a, int b)  
{  
    vector<int> res(a.size() + 10, 0);  
    for (int i = a.size() - 1; i >= 0; i--)  
    {  
        res[a.size() - i - 1] = (a[i] - '0') * b;  
    }  
    for (int i = 0; i < res.size(); i++)  
    {  
        if (res[i] >= 10)  
        {  
            res[i + 1] += res[i] / 10;  
            res[i] %= 10;  
        }  
    }  
    while (res.size() > 1 && res.back() == 0)  
    {  
        res.pop_back();  
    }  
    string ans = "";  
    for (int i = res.size() - 1; i >= 0; i--)  
    {  
        ans += res[i] + '0';  
    }  
    return ans;  
}  
  
string multi(string& a, string& b)  
{  
    vector<int> na(MAXN + 3), nb(MAXN + 3), res(MAXN + 5, 0);  
    bool f = 0;  
    for (int i = a.size() - 1; i >= 0; i--)  
    {  
        na[a.size() - 1 - i] = a[i] - '0';  
    }  
    for (int i = b.size() - 1; i >= 0; i--)  
    {  
        nb[b.size() - 1 - i] = b[i] - '0';  
    }  
    for (int i = 0; i <= MAXN; i++)  
    {  
        for (int j = 0; j <= i; j++) res[i] += na[j] * nb[i - j];  
        if (res[i] >= 10)  
        {  
            res[i + 1] += res[i] / 10;  
            res[i] %= 10;  
        }  
    }  
    string ans = "";  
    for (int i = MAXN; i >= 0; i--)  
    {  
        if (res[i] != 0) f = 1;  
        if (f) ans += res[i] + '0';  
    }  
    return ans == "" ? "0" : ans;  
}  
  
string div(string& a, int b)  
{  
    string res = "";  
    long long remain = 0;  
    for (auto& i : a)  
    {  
        remain = remain * 10 + i - '0';  
        res += remain / b + '0';  
        remain %= b;  
    }  
    auto it = res.find_first_not_of('0');  
    if (it != string::npos) return res.substr(it);  
    else return "0";  
}  
  
  
/*  
    if (a.size() < b.size() || (a.size() == b.size() && a < b))    {        cout << "0" << endl;        return 0;    }*/  
bool candiv(vector<int>& a, vector<int>& b, int apos, int blen)  
{  
    if (a[apos + blen] != 0) return 1;  
    for (int i = blen - 1; i >= 0; i--)  
    {  
        if (a[apos + i] > b[i]) return 1;  
        if (a[apos + i] < b[i]) return 0;  
    }  
    return 1;  
}  
  
string div(string& x, string& y)  
{  
    if (y == "0") return "-1";  
    if (y == "1") return x;  
    vector<int> a(MAXN + 5), b(MAXN + 5), res(MAXN + 5);  
    for (int i = x.size() - 1; i >= 0; i--) a[x.size() - 1 - i] = x[i] - '0';  
    for (int i = y.size() - 1; i >= 0; i--) b[y.size() - 1 - i] = y[i] - '0';  
    int len = x.size() - y.size();  
    for (int i = x.size() - y.size(); i >= 0; i--)  
    {  
        while (candiv(a, b, i, y.size()))  
        {  
            res[i]++;  
            for (int j = 0; j < y.size(); j++)  
            {  
                if (a[i + j] < b[j])  
                {  
                    a[i + j + 1]--;  
                    a[i + j] += 10;  
                }  
                a[i + j] -= b[j];  
            }  
        }  
    }  
    string ans = "";  
    bool f = 0;  
    for (int i = MAXN; i >= 0; i--)  
    {  
        if (res[i] != 0) f = 1;  
        if (f) ans += res[i] + '0';  
    }  
    return ans;  
}
```
