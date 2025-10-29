```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;
auto prefix_function(const string &s){
    vector<int> pi(s.size());
    for(int i = 1; i < s.size(); i++){
        int j = pi[i - 1];
        while(j > 0 && s[i] != s[j]) j = pi[j - 1];
        if(s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}
auto find_occur(const string &text, const string &pattern){
    string cur = pattern + '#' + text;
    vector<int> ans, lpf = prefix_function(cur);
    for(int i = pattern.size() + 1; i <= text.size() + pattern.size(); i++){
        if(lpf[i] == pattern.size()) ans.push_back(i - 2 * pattern.size());
    }
    return ans;
}
int main(){
    string s1, s2;
    cin >> s1 >> s2;
    auto ans = find_occur(s1, s2);
    for(auto i : ans){
        cout << i + 1 << '\n';
    }
    auto bd = prefix_function(s2);
    for(auto i : bd){
        cout << i << ' ';
    }
    cout << '\n';
    return 0;
}
```