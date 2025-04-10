#include <string>
#include <ranges>
#include <random>
using namespace std;
using ll = long long;
using ull = unsigned long long;
struct Hash{
    random_device rd;
    mt19937 generator;
    uniform_int_distribution<ull> rnd;
    ull M1, M2;
    ull base = 131;
    Hash() : generator(rd()), rnd(1e8, 1e9) {
        M1 = rnd(generator);
        M2 = rnd(generator);
    }
    auto get_hash(const string &s){
        ull res1 = 0, res2 = 0;
        for(auto &i : s){
            res1 = (res1 * base + i) % M1;
            res2 = (res2 * base + i) % M2;
        }
        return pair(res1, res2);
    }
};
struct Hash_RangeQuery{
    random_device rd;
    mt19937 generator;
    uniform_int_distribution<ull> rnd;
    ull M1, M2;
    ull base = 131;
    vector<array<ull, 2>> prem, preb;
    Hash_RangeQuery(const string &s) : generator(rd()), rnd(1e8, 1e9) {
        M1 = rnd(generator);
        M2 = rnd(generator);
        prem.assign(s.size() + 1, array{0ull, 0ull});
        preb.assign(s.size() + 1, array{0ull, 0ull});
        for(int i = 0; i < s.size(); i++){
            prem[i + 1][0] = (prem[i][0] * base + s[i]) % M1;
            prem[i + 1][1] = (prem[i][1] * base + s[i]) % M2;
        }
        preb[0][0] = preb[0][1] = 1;
        for(int i = 1; i <= s.size(); i++){
            preb[i][0] = preb[i - 1][0] * base % M1;
            preb[i][1] = preb[i - 1][1] * base % M2;
        }
    }
    auto query(int l, int r){
        return pair(prem[r + 1][0] - prem[l][0] * preb[r - l + 1][0], prem[r + 1][1] - prem[l][1] * preb[r - l + 1][1]);
    }
    bool check(int l1, int r1, int l2, int r2){
        return query(l1, r1) == query(l2, r2);
    }
};