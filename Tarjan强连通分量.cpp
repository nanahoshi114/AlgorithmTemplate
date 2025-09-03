#include <iostream>
#include <vector>
#include <algorithm>
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
int main() {
    int n, m;
    std::cin >> n >> m;
    GraphEdge edge(n + 1);
    for (int i = 1; i <= m; ++i) {
        int u, v;
        std::cin >> u >> v;
        edge[u].push_back(v);
    }
    auto [sc_size, scc] = tarjan_search_ver2(edge, n);
    std::vector<bool> vis(n + 1);
    std::vector<int> now_ans;
    auto print_ans = [&](auto&& self, int now) -> void {
        now_ans.push_back(now);
        vis[now] = true;
        for (const auto& next : edge[now]) {
            if (scc[next] == scc[now] && !vis[next]) {
                self(self, next);
            }
        }
    };
    std::cout << sc_size.size() << '\n';
    for (int i = 1; i <= n; ++i) {
        if (!vis[i]) {
            print_ans(print_ans, i);
            std::ranges::sort(now_ans);
            for (const auto& j : now_ans) {
                std::cout << j << ' ';
            }
            std::cout << '\n';
            now_ans.clear();
        }
    }
    /*
    auto ans = tarjan_search(edge, n);
    for (auto& vec : ans) {
        std::ranges::sort(vec);
    }
    std::ranges::sort(ans, [](const auto& a, const auto& b) {
        return a[0] < b[0];
    });
    std::cout << ans.size() << '\n';
    for (const auto& vec : ans) {
        for (const auto& i : vec) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }*/
    return 0;
}