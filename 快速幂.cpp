using ll = long long;
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