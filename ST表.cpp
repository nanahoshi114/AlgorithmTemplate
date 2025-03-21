#include <vector>
#include <functional>
using namespace std;
template <typename T>
class sparseTable{
    vector<vector<T>> ST;
    vector<T> lgn;
    function<T(T, T)> func;
public:
    sparseTable(const vector<T> &org, function<T(T, T)> _func){
        func = _func;
        int sz = org.size() - 1;
        lgn.assign(sz + 5, 0);
        lgn[1] = 0, lgn[2] = 1;
        for(int i = 3; i <= sz; i++){
            lgn[i] = lgn[i / 2] + 1;
        }
        ST.assign(sz + 5, vector<T>(lgn[sz] + 5));
        for(int i = 1; i <= sz; i++){
            ST[i][0] = org[i];
        }
        for(int j = 1; j <= lgn[sz]; j++){
            int pj = (1 << (j - 1));
            for(int i = 1; i + pj <= sz; i++){
                ST[i][j] = func(ST[i][j - 1], ST[i + pj][j - 1]);
            }
        }
    }
    T query(int l, int r){
        int pj = lgn[r - l + 1];
        return func(ST[l][pj], ST[r - (1 << pj) + 1][pj]);
    }
};