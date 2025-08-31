#include <string>
#include <ranges>
#include <random>
using namespace std;
using i64 = long long;
std::mt19937 generator(43);
std::uniform_int_distribution<i64> rnd(1e8, 1e9);
const i64 M1 = rnd(generator), M2 = rnd(generator), base = 131;
auto get_hash(const string &s){
    i64 res1 = 0, res2 = 0;
    for(auto &i : s){
        res1 = (res1 * base + i) % M1;
        res2 = (res2 * base + i) % M2;
    }
    return pair(res1, res2);
}
using u64 = unsigned long long;
class Hash_RangeQuery {
    std::vector<std::array<u64, 2>> prem, preb;
public:
    explicit Hash_RangeQuery(const std::string& s) {
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