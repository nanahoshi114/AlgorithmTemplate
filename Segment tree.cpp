#include <vector>
using namespace std;
template <typename T>
class segtree{
private:
    vector<T> segd;
    vector<T> tag;
    vector<T> *ipm;
    int size = 0, s, t;
    void build(int s, int t, int p){
        size = max(size, p);
        if(s == t){
            segd[p] = (*ipm)[s];
            return;
        }
        int mid = s + ((t - s) >> 1);
        build(s, mid, p * 2), build(mid + 1, t, p * 2 + 1);
        segd[p] = segd[p * 2] + segd[p * 2 + 1];
    }
    T _getsum(int l, int r, int s, int t, int p){
        if(l <= s && t <= r){
            return segd[p];
        }
        int mid = s + ((t - s) >> 1);
        if(tag[p] && s != t){
            segd[p * 2] += tag[p] * (mid - s + 1); tag[p * 2] += tag[p];
            segd[p * 2 + 1] += tag[p] * (t - mid); tag[p * 2 + 1] += tag[p];
            tag[p] = 0;
            segd[p] = segd[p * 2] + segd[p * 2 + 1];
        }
        T sum = 0;
        if(l <= mid){
            sum += _getsum(l, r, s, mid, p * 2);
        }
        if(r > mid){
            sum += _getsum(l, r, mid + 1, t, p * 2 + 1);
        }
        return sum;
    }
    void _update(int l, int r, int s, int t, int p, T c){
        if(l <= s && t <= r){
            segd[p] += c * (t - s + 1);
            tag[p] += c;
            return;
        }
        int mid = s + ((t - s) >> 1);
        if(tag[p] && s != t){
            segd[p * 2] += tag[p] * (mid - s + 1); tag[p * 2] += tag[p];
            segd[p * 2 + 1] += tag[p] * (t - mid); tag[p * 2 + 1] += tag[p];
            tag[p] = 0;
        }
        if(l <= mid) _update(l, r, s, mid, p * 2, c);
        if(mid < r) _update(l, r, mid + 1, t, p * 2 + 1, c);
        segd[p] = segd[p * 2] + segd[p * 2 + 1];
    }
public:
    explicit segtree(vector<T> *_ipm) : segd(4 * _ipm->size(), 0), tag(4 * _ipm->size(), 0){
        s = 0, t = _ipm->size() - 1;
        ipm = _ipm;
        build(s, t, 1);
    }
    T getsum(int l, int r){
        return _getsum(l - 1, r - 1, s, t, 1);
    }
    void update(int l, int r, T c){
        _update(l - 1, r - 1, s, t, 1, c);
    }
};
