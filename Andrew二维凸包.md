```cpp
auto static_convec_hull(std::vector<Pd> &pts) {
    int n = pts.size();
    std::sort(pts.begin(), pts.end());
    std::vector<int> stack;
    std::vector<bool> vis(n);
    stack.push_back(0);
    for (int i = 1; i < n; ++i) {
        while (stack.size() >= 2) {
            int s1 = stack.back(), s2 = stack[stack.size() - 2];
            if (cross((pts[s1] - pts[s2]), (pts[i] - pts[s1])) > 0) { //如果需要保留凸包边上的点，此处改为>=，同下
                break;
            }
            vis[s1] = false;
            stack.pop_back();
        }
        vis[i] = true;
        stack.push_back(i);
    }
    int down_sz = stack.size();
    for (int i = n - 2; i >= 0; --i) {
        if (vis[i]) continue;
        while (stack.size() > down_sz) {
            int s1 = stack.back(), s2 = stack[stack.size() - 2];
            if (cross((pts[s1] - pts[s2]), (pts[i] - pts[s1])) > 0) {
                break;
            }
            vis[s1] = false;
            stack.pop_back();
        }
        vis[i] = true;
        stack.push_back(i);
    }
    return stack;
}
```
```cpp
#include<bits/stdc++.h>  
using namespace std;  
using ld = long double;  
const ld PI = acos(-1);  
const ld EPS = 1e-7;  
const ld INF = numeric_limits<ld>::max();  
#define cc(x) cout << fixed << setprecision(x);  
ld fgcd(ld x, ld y) { // 实数域gcd  
    return abs(y) < EPS ? abs(x) : fgcd(y, fmod(x, y));  
}  
template<class T, class S> bool equal(T x, S y) {  
    return -EPS < x - y && x - y < EPS;  
}  
template<class T> int sign(T x) {  
    if (-EPS < x && x < EPS) return 0;  
    return x < 0 ? -1 : 1;  
}  
template<class T> struct Point { // 在C++17下使用 emplace_back 绑定可能会导致CE！  
    T x, y;  
    Point(T x_ = 0, T y_ = 0) : x(x_), y(y_) {} // 初始化  
    template<class U> operator Point<U>() { // 自动类型匹配  
        return Point<U>(U(x), U(y));  
    }  
    Point &operator+=(Point p) & { return x += p.x, y += p.y, *this; }  
    Point &operator+=(T t) & { return x += t, y += t, *this; }  
    Point &operator-=(Point p) & { return x -= p.x, y -= p.y, *this; }  
    Point &operator-=(T t) & { return x -= t, y -= t, *this; }  
    Point &operator*=(T t) & { return x *= t, y *= t, *this; }  
    Point &operator/=(T t) & { return x /= t, y /= t, *this; }  
    Point operator-() const { return Point(-x, -y); }  
    friend Point operator+(Point a, Point b) { return a += b; }  
    friend Point operator+(Point a, T b) { return a += b; }  
    friend Point operator-(Point a, Point b) { return a -= b; }  
    friend Point operator-(Point a, T b) { return a -= b; }  
    friend Point operator*(Point a, T b) { return a *= b; }  
    friend Point operator*(T a, Point b) { return b *= a; }  
    friend Point operator/(Point a, T b) { return a /= b; }  
    friend bool operator<(Point a, Point b) {  
        return equal(a.x, b.x) ? a.y < b.y - EPS : a.x < b.x - EPS;  
    }  
    friend bool operator>(Point a, Point b) { return b < a; }  
    friend bool operator==(Point a, Point b) { return !(a < b) && !(b < a); }  
    friend bool operator!=(Point a, Point b) { return a < b || b < a; }  
    friend auto &operator>>(istream &is, Point &p) {  
        return is >> p.x >> p.y;  
    }  
    friend auto &operator<<(ostream &os, Point p) {  
        return os << "(" << p.x << ", " << p.y << ")";  
    }  
};  
template<class T> T cross(Point<T> a, Point<T> b) { // 叉乘  
    return a.x * b.y - a.y * b.x;  
}  
template<class T> T cross(Point<T> p1, Point<T> p2, Point<T> p0) { // 叉乘 (p1 - p0) x    (p2 - p0);  
    return cross(p1 - p0, p2 - p0);  
}  
template<class T> vector<pair<Point<T>,int>> staticConvexHull(vector<Point<T>> A, int flag =1) {  
    int n = A.size();  
  
    vector<pair<Point<T>,int>> ans(n * 2);  
    vector<pair<Point<T>,int>>B;  
    int cnt=1;  
    for(auto i:A){  
        B.emplace_back(i,cnt++);  
    }  
    if (n <= 2) { // 特判  
        return B;  
    }  
    sort(B.begin(), B.end());  
    int now = -1;  
    for (int i = 0; i < n; i++) { // 维护下凸包  
        while (now > 0 && cross(B[i].first, ans[now].first, ans[now - 1].first) < flag) {  
            now--;  
        }  
        ans[++now] = B[i];  
    }  
    int pre = now;  
    for (int i = n - 2; i >= 0; i--) { // 维护上凸包  
        while (now > pre && cross(B[i].first, ans[now].first, ans[now - 1].first) < flag) {  
            now--;  
        }  
        ans[++now] = B[i];  
    }  
    ans.resize(now);  
    return ans;  
}
```