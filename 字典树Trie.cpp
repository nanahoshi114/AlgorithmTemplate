#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
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