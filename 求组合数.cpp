#include <vector>
#include <iostream>
#include <array>
using namespace std;
class ConbinatorialNumber{
    using ll = unsigned long long;
    ll fastPow(ll a, ll b, ll mod) {
        ll result = 1;
        a = a % mod;
        while (b > 0) {
            if (b & 1) {
                result = (result * a) % mod;
            }
            a = (a * a) % mod;
            b >>= 1;
        }
        return result;
    }
    vector<ll> pret;
    vector<ll> inv;
    ll P;
    void make(ll n, ll P){
        if(pret.size() - 1 >= n) return;
        for(ll i = pret.size(); i <= n; i++){
            pret.push_back((i * pret.back()) % P);
        }
        inv[n] = fastPow(pret[n], P - 2, P);
        for(ll i = n - 1; i >= 0 && !inv[i]; i--){
            inv[i] = inv[i + 1] * (i + 1) % P;
        }
    }
public:
    explicit ConbinatorialNumber(ll size, ll _P) : pret(2, 1), inv(int(5e6) + 5){
        P = _P;
        make(size, P);
    }
    void extend(ll size){
        make(size, P);
    }
    ll operator()(ll n, ll m){
        if(m == 0) return 1;
        return (((pret[n] * inv[m]) % P) * inv[n - m]) % P; 
    }
};