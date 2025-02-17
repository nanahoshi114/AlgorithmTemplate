#include <string>
#include <vector>
using namespace std;
pair<vector<int>, vector<int>> manacher(const string &s){
    //d1：奇数长度的回文串长度，d1[i]：以s[i]为中心的回文串个数；d2：偶数长度的回文串长度
    vector<int> d1(s.size() + 5, 0), d2(s.size() + 5, 0); 
    for(int i = 0, l = 0, r = -1; i < s.size(); i++){
        if(i <= r) d1[i] = min(d1[l + r - i], r - i + 1);
        while(0 <= i - d1[i] && i + d1[i] < s.size() && s[i - d1[i]] == s[i + d1[i]]){
            d1[i]++;
        }
        if(i + d1[i] - 1 > r){
            l = i - d1[i] + 1;
            r = i + d1[i] - 1;
        }
    }
    for(int i = 0, l = 0, r = -1; i < s.size(); i++){
        if(i <= r) d2[i] = min(d2[l + r - i + 1], r - i + 1);
        while(0 <= i - d2[i] - 1 && i + d2[i] < s.size() && s[i - d2[i] - 1] == s[i + d2[i]]){
            d2[i]++;
        }
        if(i + d2[i] - 1 > r){
            l = i - d2[i];
            r = i + d2[i] - 1;
        }
    }
    return {d1, d2};
}