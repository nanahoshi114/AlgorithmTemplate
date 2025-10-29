```cpp
#include <bits/stdc++.h>
#define LL long long
using namespace std;

LL n, longest = 0;
string s;

vector<LL> z_function(string s){
    vector<LL> ret;
    ret.resize(s.size());
    LL ll = 0, rr = 0; // 可以使用已知資訊區間的 [ll, rr)

    for (LL i=1 ; i<s.size() ; i++){ // 目前要找的索引值
        LL j = 0; // i 需要枚舉前綴的開頭
		
		// 如果 i 還在只知資訊區間就可以先用前面得到的資訊
		// 找 i-ll 的答案，rr-i 是上界
        if (i<rr) j = min(ret[i-ll], rr-i);
        while (s[j]==s[i+j]) j++; // 枚舉已知資訊後的內容
        ret[i] = j;
		
        if (i+j>rr){ // 如果新的範圍大於已知資訊的區間，就更新
            ll = i;
            rr = i+j;
        }
    }

    ret[0] = s.size();
    return ret;
}

LL z_value(vector<LL> &v, LL p){
    if (0<=p && p<v.size()) return v[p];
    return 0;
}

// 尋找字串 s 中的重串數量
LL repetition(string s){

    // 終止條件
    if (s.size()==1){
        return 0;
    }

    // 找到左邊右邊的重串
    LL mid = s.size()/2;
    string u = s.substr(0, mid);
    string v = s.substr(mid);
    string ru(u.rbegin(), u.rend());
    string rv(v.rbegin(), v.rend());
    LL lc = repetition(u);
    LL rc = repetition(v);

    // 尋找中間的重串
    LL total = 0;
    vector<LL> z1 = z_function(ru);
    vector<LL> z2 = z_function(v+'#'+u);
    vector<LL> z3 = z_function(ru+'#'+rv);
    vector<LL> z4 = z_function(v);

    // 找到左偏重串
    for (LL ptr=0 ; ptr<u.size() ; ptr++){
        if (u[ptr]==v[0]){
            LL l = u.size()-ptr;
            LL L = z_value(z1, u.size()-ptr);
            L = min(L, l-1); // 限制長度，不能和 ptr 重疊
            LL R = z_value(z2, v.size()+1+ptr);
            R = min(R, l-1); // 限制長度，確保是左偏重串

            int add = max(0LL, R+L-l+1);
            total += add;
            if (add){
                longest = max(longest, l);
            }
        }
    }

    // 找到右偏重串
    for (LL ptr=0 ; ptr<v.size() ; ptr++){
        if (u.back()==v[ptr]){
            LL l = ptr+1;
            LL L = z_value(z3, u.size()+v.size()-ptr);
            LL R = z_value(z4, ptr+1);
            R = min(R, l-1); // 限制長度，不能和 ptr 重疊

            int add = max(0LL, R+L-l+1);
            total += add;
            if (add){
                longest = max(longest, l);
            }
        }
    }

    return lc+rc+total;
}

int main(){

    // input
    cin >> n;
    cin >> s;

    // process
    int res = repetition(s);
    cout << longest*2 << " " << res << "\n";

    return 0;
}

```