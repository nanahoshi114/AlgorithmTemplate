#include <iostream>
#include <string>
#include <vector>
using namespace std;
string manacher(const string &org){
    string t("#");
    for(char c : org){
        t.push_back(c); t.push_back('#');
    }
    vector<int> d(t.size(), 0);
    int C = 0, R = 0, maxlen = 0, centerInd = 0;
    for(int i = 1; i < t.size(); i++){
        if(i < R) d[i] = min(d[2 * C - i], R - i);
        else d[i] = 1;
        while(0 <= i - d[i] && i + d[i] < t.size() && t[i + d[i]] == t[i - d[i]]) d[i]++;
        if(i + d[i] > R){
            C = i;
            R = i + d[i];
        }
        if(d[i] > maxlen){
            maxlen = d[i];
            centerInd = i;
        }
    }
    return org.substr((centerInd - maxlen) / 2, maxlen - 1);
}
int main(){
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);
    string s;
    cin >> s;
    string ans = manacher(s);
    cout << ans.size();
    return 0;
}