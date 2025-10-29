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