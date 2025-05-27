#include <cmath>
#include <utility>
#include <algorithm>
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