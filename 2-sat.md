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