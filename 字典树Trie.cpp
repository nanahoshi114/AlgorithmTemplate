#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
class Trie{
    struct Node{
        unordered_map<char, int> next;
        bool isEnd;
        Node() : isEnd(0) {};
    };
    vector<Node> tree;
public:
    Trie(){
        tree.emplace_back();
    }
    void insert(const string &word){
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
    bool find(const string &word){
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