#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <bitset>
using namespace std;
const int M = 1e8 + 1;
struct Prime{
    bitset<M> is_prime;
    vector<int> prime;
    int size;
    Prime(int _size) : size(_size){
        is_prime = ~is_prime;
        is_prime[1] = 0;
        for(int i = 2; i <= size; i++){
            if(is_prime[i]){
                prime.push_back(i);
            }
            for(auto j : prime){
                if(i * j > size) break;
                is_prime[i * j] = 0;
                if(i % j == 0) break;
            }
        }
    }
    int operator[](int ind){
        return prime[ind];
    }
    bool is_prime(int x){
        return is_prime[x];
    }
};
int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int n, q;
    cin >> n >> q;
    Prime prime(n);
    while(q--){
        int t;
        cin >> t;
        cout << prime[t - 1] << '\n';
    }
    return 0;
}