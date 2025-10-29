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