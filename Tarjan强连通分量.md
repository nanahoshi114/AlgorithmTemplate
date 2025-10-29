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