#include <vector>
using namespace std;
class Inv{
private:
    using ll = long long;
    vector<ll> vec;
    ll mod;
public:
    explicit Inv(ll size, ll p) : vec(size + 1){
        mod = p;
        vec[0] = vec[1] = 1;
        for(ll i = 2; i <= size; i++){
            vec[i] = (p - p / i * vec[p % i] % p) % p;
        }
    }
    void extend(ll size){
        if(vec.size() - 1 >= size) return;
        for(int i = vec.size(); i <= size; i++){
            vec.push_back((mod - mod / i * vec[mod % i] % mod) % mod);
        }
    }
    ll operator[](ll index){
        return vec[index];
    }
};