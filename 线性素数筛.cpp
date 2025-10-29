#include <iostream>
#include <vector>
#include <bitset>
constexpr int M = 1e8 + 1;
using i64 = long long;
std::bitset<M> prime_flag;
std::vector<int> prime_list;
struct Prime_Initor {
    Prime_Initor() {
        prime_flag.set();
        prime_flag[1] = 0;
        for(int i = 2; i < M; i++){
            if(prime_flag[i]){
                prime_list.push_back(i);
            }
            for(auto j : prime_list){
                if(i * j >= M) break;
                prime_flag[i * j] = 0;
                if(i % j == 0) break;
            }
        }
    }
} prime_initor;
int main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cout.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    while(q--){
        int t;
        std::cin >> t;
        std::cout << prime_list[t - 1] << '\n';
    }
    return 0;
}