#include <vector>
#include <utility>
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