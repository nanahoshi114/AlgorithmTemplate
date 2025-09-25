#include <iostream>
#include <queue>
#include <vector>
#include <numeric>
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

void add(GraphEdge& edge, int u, int to, int dis) {
    edge[u].push_back({to, dis});
}

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

int main() {
    int n, m, s;
    std::cin >> n >> m >> s;
    GraphEdge edge(n + 1);
    while (m--) {
        int u, d, v;
        std::cin >> u >> d >> v;
        add(edge, u, d, v);
    }
    auto ans = dijkstra(edge, s);
    for (int i = 1; i <= n; ++i) {
        std::cout << ans[i] << ' ';
    }
    std::cout << '\n';
    return 0;
}