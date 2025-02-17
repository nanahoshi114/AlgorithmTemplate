#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;
const int M = 1e8 + 1;
bool is_prime[M];
int n, q;
vector<int> prime;
void make(){
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[1] = 0;
    for(int i = 2; i <= n; i++){
        if(is_prime[i]){
            prime.push_back(i);
        }
        for(auto j : prime){
            if(i * j > n) break;
            is_prime[i * j] = 0;
            if(i % j == 0) break;
        }
    }
}
int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> n >> q;
    make();
    while(q--){
        int t;
        cin >> t;
        cout << prime[t - 1] << '\n';
    }
    return 0;
}