#include <cmath>
#include <utility>
#include <algorithm>
#include <tuple>
using ld = long double;
const ld pi = std::acos(-1.0);
const ld EPS = 1e-7;

bool equal(auto&& x, auto&& y) {
    return -EPS < (x - y) && (x - y) < EPS;
}
int sign(auto&& x) {
    if(-EPS < x && x < EPS) return 0;
    return x < 0 ? -1 : 1;
}
template<typename T = ld>
struct Point{
    T x, y;
    Point() : x(), y() {}
    Point(auto&& _x, auto&& _y) : x(T(_x)), y(T(_y)) {}
    Point operator-(Point _r) const {
        return {x - _r.x, y - _r.y};
    }
    Point operator+(Point _r) const {
        return {x + _r.x, y + _r.y};
    }   
    Point& operator+=(Point _r) {
        x += _r.x; y += _r.y;
        return *this;
    }
    Point& operator-=(Point _r) {
        x -= _r.x; y -= _r.y;
        return *this;
    }
    T operator*(Point _r) const {
        return x * _r.x + y * _r.y;
    }
    friend Point operator*(Point _l, T _r) {
        return {_l.x * _r, _l.y * _r};
    }
    friend Point operator/(Point _l, T _r) {
        return {_l.x / _r, _l.y / _r};
    }
    bool operator==(Point _r) const {
        return equal(x, _r.x) && equal(y, _r.y);
    }
};


template<typename T = ld>
struct Line{
    Point<T> a, b;
    Line(Point<T> _l, Point<T> _r) : a(_l.x, _l.y), b(_r.x, _r.y) {}
    auto get_vec() const {
        return b - a;
    }
};

using Pd = Point<>;
using Ld = Line<>;

ld toArc(ld x) {
	return pi / 180 * x;
}

ld cross(Pd veca, Pd vecb) {
    return veca.x * vecb.y - veca.y * vecb.x;
}

ld cross(Pd p1, Pd p2, Pd p0) {
    return cross(p1 - p0, p2 - p0);
}

ld dot(Pd p1, Pd p2, Pd p0) {
    return (p1 - p0) * (p2 - p0);
}
ld dis(Pd a, Pd b) {
    ld val = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    return sqrt(val);
}

Pd standardlize(Pd vec) {
    return vec / sqrt(vec.x * vec.x + vec.y * vec.y);
}

Pd rotate(Pd p1, Pd p2) {
    Pd vec = p1 - p2;
    return {-vec.x, vec.y};
}

ld angle(Pd veca, Pd vecb) {
    return std::abs(std::atan2(std::abs(cross(veca, vecb)), veca * vecb));
}

bool is_onLine(Pd a, Pd b, Pd c) {
    return sign(cross(c - b, c - a)) == 0;
}

bool is_onLine(Pd p, Ld l) {
    return is_onLine(p, l.a, l.b);
}

bool pointOnSameSide(Pd p1, Pd p2, Ld line) {
    auto vec = line.get_vec();
    auto val = cross(line.b - p1, vec) * cross(line.b - p2, vec);
    return sign(val) == 1;
}

Pd getLineIntersection(Ld l1, Ld l2) {
    auto vec1 = l1.get_vec(), vec2 = l2.get_vec();
    ld t = cross(l2.a - l1.a, vec2) / cross(vec1, vec2);
    return l1.a + vec1 * t;
}

bool is_LineParallel(Ld l1, Ld l2) {
    return sign(cross(l1.get_vec(), l2.get_vec())) == 0;
}

bool is_LineVertical(Ld l1, Ld l2) {
    return sign(l1.get_vec() * l2.get_vec()) == 0;
}

auto pointToLine(Pd p, Ld l) {
    Pd ans = getLineIntersection({p, p + rotate(l.a, l.b)}, l);
    return std::pair(ans, dis(p, ans));
}

ld dis_PointToLine(Pd p, Ld l) {
    return std::abs(cross(l.b - p, l.get_vec())) / dis(l.a, l.b);
}

bool is_PointOnSegment(Pd p, Ld l) {
    return sign(cross(p, l.a, l.b)) == 0 
        && sign(std::min(l.a.x, l.b.x) - p.x) != 1 
        && sign(p.x - std::max(l.a.x, l.b.x)) != 1
        && sign(std::min(l.a.y, l.b.y) - p.y) != 1
        && sign(p.y - std::max(l.a.y, l.b.y)) != 1;
}

auto pointToSegment(Pd p, Ld l) {
    if (sign(dot(p, l.b, l.a)) == -1) {
        return std::pair(l.a, dis(p, l.a));
    } else if (sign(dot(p, l.a, l.b)) == -1) {
        return std::pair(l.b, dis(p, l.b));
    }
    return pointToLine(p, l);
}

Pd project(Pd p, Ld l) {
    Pd vec = l.get_vec();
    ld r = vec * (p - l.a) / (vec.x * vec.x + vec.y * vec.y);
    return l.a + vec * r;
}

Ld midSegment(Ld l) {
    auto mid = (l.a + l.b) / 2;
    return {mid, mid + rotate(l.a, l.b)};
}

std::tuple<int, Pd, Pd> getSegmentIntersection(Ld l1, Ld l2) {
    auto [s1, e1] = l1;
    auto [s2, e2] = l2;
    auto A = std::max(s1.x, e1.x), AA = std::min(s1.x, e1.x);
    auto B = std::max(s1.y, e1.y), BB = std::min(s1.y, e1.y);
    auto C = std::max(s2.x, e2.x), CC = std::min(s2.x, e2.x);
    auto D = std::max(s2.y, e2.y), DD = std::min(s2.y, e2.y);
    if (A < CC || C < AA || B < DD || D < BB) {
        return {0, {}, {}};
    }
    if (sign(cross(e1 - s1, e2 - s2)) == 0) {
        if (sign(cross(s2, e1, s1)) != 0) {
            return {0, {}, {}};
        }
        Point p1(std::max(AA, CC), std::max(BB, DD));
        Point p2(std::min(A, C), std::min(B, D));
        if (!is_PointOnSegment(p1, l1)) {
            std::swap(p1.y, p2.y);
        }
        if (p1 == p2) {
            return {3, p1, p2};
        } else {
            return {2, p1, p2};
        }
    }
    auto cp1 = cross(s2 - s1, e1 - s1), cp2 = cross(s2 - e1, e2 - e1), 
         cp3 = cross(s1 - s2, e1 - s2), cp4 = cross(s1 - e2, e1 - e2);
    if(sign(cp1 * cp2) == 1 || sign(cp3 * cp4)) {
        return {0, {}, {}};
    }
    Pd p = getLineIntersection(l1, l2);
    if (sign(cp1) != 0 && sign(cp2) != 0 && sign(cp3) != 0 && sign(cp4) != 0) {
        return {1, p, p};
    } else {
        return {3, p, p};
    }
}

bool is_SegmentIntersection(Ld l1, Ld l2) {
    auto [s1, e1] = l1;
    auto [s2, e2] = l2;
    auto A = std::max(s1.x, e1.x), AA = std::min(s1.x, e1.x);
    auto B = std::max(s1.y, e1.y), BB = std::min(s1.y, e1.y);
    auto C = std::max(s2.x, e2.x), CC = std::min(s2.x, e2.x);
    auto D = std::max(s2.y, e2.y), DD = std::min(s2.y, e2.y);
    return A >= CC && B >= DD && C >= AA && D >= BB
        && sign(cross(s1, s2, e1) * cross(s1, e1, e2)) == 1
        && sign(cross(s2, s1, e2) * cross(s2, e2, e1)) == 1;
}

std::pair<Pd, ld> getPointToCircle(Pd p, Pd o, ld r) {
    Pd U = o, V = o;
    ld d = dis(p, o);
    if (sign(d) == 0) {
        return {o, 0};
    }
    ld val1 = r * abs(o.x - p.x) / d;
    ld val2 = r * abs(o.y - p.y) / d * (sign((o.x - p.x) * (o.y - p.y)) < 0 ? -1 : 1);
    U.x += val1, U.y += val2;
    V.x -= val1, V.y -= val2;
    if (dis(U, p) < dis(V, p)) {
        return {U, dis(U, p)};
    } else {
        return {V, dis(V, p)};
    }
}

Pd getPointOnCircle(Pd p, ld r, ld rad) {
    return {p.x + std::cos(rad) * r, p.y + std::sin(rad) * r};
}

