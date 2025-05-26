#include <string>
using namespace std;
class Highnum{
    string s;
public:
    Highnum(string x){
        s = x;
    }
    void operator=(string x){
        s = x;
    }
    Highnum operator+(Highnum r){
        string ans;
        int t = 0, c = 0;
        for(int i = 0; i < s.size() || i < r.s.size(); i++){
            t = c;
            if(i < s.size()) t += s[i] - '0';
            if(i < r.s.size()) t += r.s[i] - '0';
            c = t / 10;
            t %= 10;
            ans.push_back(t + '0');
        }
        if(c > 0) ans.push_back(c + '0');
        return Highnum(ans);
    }
    string show(){
        return string(s.rbegin(), s.rend());
    }
};