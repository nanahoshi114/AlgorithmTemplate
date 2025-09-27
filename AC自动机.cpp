#include <vector>
#include <queue>
#include <unordered_map>
constexpr int MAX_NODE = 2e5 + 5;
using i64 = long long;
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