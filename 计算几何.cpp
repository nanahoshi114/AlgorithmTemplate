#include <cmath>

#ifdef HIGH_PRECISION
#define double long double
#define fabs fabsl
#define fsin fsinl
#define fcos fcosl
#define acos acosl
#define atan atanl
#define asin asinl
#define sqrt sqrtl
#endif

const double pi = std::acos(-1.0);

struct Vector{
    double x, y;
    Vector(auto&& _x, auto&& _y) : x(double(_x)), y(double(_y)) {}
    double operator*(Vector _r) const {
        return x * _r.x + y * _r.y;
    }
    double length() {
        return sqrt(x * x + y * y);
    }
    Vector operator/(double num) const {
        return {x / num, y / num};
    }
    Vector operator*(double num) const {
        return {x * num, y * num};
    }
    Vector& operator*=(double num) {
        x *= num, y *= num;
        return *this;
    }
    Vector& operator/=(double num) {
        x /= num, y /= num;
        return *this;
    }
};

struct Point{
    double x, y;
    Point(auto&& _x, auto&& _y) : x(double(_x)), y(double(_y)) {}
    Vector operator-(Point _r) const {
        return {x - _r.x, y - _r.y};
    }
    Point operator+(Vector _r) const {
        return {x + _r.x, y + _r.y};
    }   
};

Point operator+(Vector l, Point r){
    return {l.x + r.x, l.y + r.y};
}

struct Line{
    double k, b;
    Line(auto&& _k, auto&& _b) : k(double(_k)), b(double(_b)) {}
    Point get_point(double x) const {
        return {x, k * x + b};
    }
    Point operator&(Line _r) const {
        double x = (_r.b - b) / (k - _r.k);
        return get_point(x);
    }
};

Point getIntersection(Line a, Line b){
    double x = (b.b - a.b) / (a.k - b.k);
    return a.get_point(x);
}

double getCross(Vector a, Vector b){
    return fabs(a.x * b.y - a.y * b.x);
}

double get_dis(Point l, Point r) {
	return sqrt((l.x - r.x) * (l.x - r.x) + (l.y - r.y) * (l.y - r.y));
}

