#include <vector>
using namespace std;
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    int count; // 连通块数量

public:
    // 初始化并查集，n 是节点数
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        count = n; // 初始时每个节点是一个独立的连通块
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    // 查找操作，带路径压缩
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // 合并操作，按秩合并
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
            count--; // 每次合并成功，连通块数量减 1
        }
    }

    // 获取连通块数量
    int getCount() const {
        return count;
    }
};