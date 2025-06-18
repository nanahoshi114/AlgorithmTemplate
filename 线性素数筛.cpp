#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <bitset>

const int M = 1e8 + 1;
using i64 = long long;
struct Prime{
    std::bitset<M> prime_flag;
    std::vector<int> prime_list;
    int size;
    Prime(int _size) : size(_size){
        prime_flag = ~prime_flag;
        prime_flag[1] = 0;
        for(int i = 2; i <= size; i++){
            if(prime_flag[i]){
                prime_list.push_back(i);
            }
            for(auto j : prime_list){
                if(i * j > size) break;
                prime_flag[i * j] = 0;
                if(i % j == 0) break;
            }
        }
    }
    int operator[](int ind){
        return prime_list[ind];
    }
    bool is_prime(int x){
        return prime_flag[x];
    }
} prime(M - 1);
int main(){
    int n, q;
    std::cin >> n >> q;
    Prime prime(n);
    while(q--){
        int t;
        std::cin >> t;
        std::cout << prime[t - 1] << '\n';
    }
    return 0;
}