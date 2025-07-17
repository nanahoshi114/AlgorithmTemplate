#include <vector>
#include <functional>

template <typename T>
class sparseTable{
    std::vector<std::vector<T>> ST;
    std::vector<T> lgn;
    std::function<T(const T &, const T &)> func;
public:
    sparseTable(const std::vector<T> &org, std::function<T(const T &, const T &)> _func){
        func = _func;
        int sz = org.size() - 1;
        lgn.assign(sz + 5, 0);
        lgn[1] = 0, lgn[2] = 1;
        for(int i = 3; i <= sz; i++){
            lgn[i] = lgn[i / 2] + 1;
        }
        //ST.assign(sz + 5, vector<T>(lgn[sz] + 5));
        ST.assign(lgn[sz] + 5, std::vector<T>(sz + 5));
        for(int i = 1; i <= sz; i++){
            ST[0][i] = org[i];
        }
        for(int j = 1; j <= lgn[sz]; j++){
            int pj = (1 << (j - 1));
            for(int i = 1; i + pj <= sz; i++){
                ST[j][i] = func(ST[j - 1][i], ST[j - 1][i + pj]);
            }
        }
    }
    T query(int l, int r){
        int pj = lgn[r - l + 1];
        return func(ST[pj][l], ST[pj][r - (1 << pj) + 1]);
    }
};