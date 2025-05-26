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
using u64 = unsigned long long;
class Hash_RangeQuery {
    std::random_device rd;
    std::mt19937 generator;
    std::uniform_int_distribution<u64> rnd;
    u64 M1, M2;
    u64 base = 131;
    std::vector<std::array<u64, 2>> prem, preb;
public:
    explicit Hash_RangeQuery(const std::string& s) : generator(rd()), rnd(1e8, 1e9) {
        M1 = rnd(generator);
        M2 = rnd(generator);
        prem.assign(s.size() + 1, std::array{0ull, 0ull});
        preb.assign(s.size() + 1, std::array{0ull, 0ull});
        for(int i = 1; i <= s.size(); i++){
            prem[i][0] = (prem[i - 1][0] * base + s[i - 1]) % M1;
            prem[i][1] = (prem[i - 1][1] * base + s[i - 1]) % M2;
        }
        preb[0][0] = preb[0][1] = 1;
        for(int i = 1; i <= s.size(); i++){
            preb[i][0] = preb[i - 1][0] * base % M1;
            preb[i][1] = preb[i - 1][1] * base % M2;
        }
    }
    auto query(int l, int r) const {
        return std::pair((prem[r + 1][0] - prem[l][0] * preb[r - l + 1][0] % M1 + M1) % M1, (prem[r + 1][1] - prem[l][1] * preb[r - l + 1][1] % M2 + M2) % M2);
    }
    bool check(int l1, int r1, int l2, int r2) const {
        return query(l1, r1) == query(l2, r2);
    }
};
//重载哈希
namespace std {
    template<> struct hash<std::pair<u64, u64>> {
        auto operator()(const std::pair<u64, u64>& x) const {
            return std::hash<u64>()(x.first) ^ std::hash<u64>()(x.second);
        }
    };
};