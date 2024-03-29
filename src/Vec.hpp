#ifndef __VEC_HPP__
#define __VEC_HPP__

#include <cmath>

using namespace std;

#define NO_INTERSECTION -1

template<class T>
class Vec3 {
public:

    T x;
    T y;
    T z;
    
    Vec3() : x(0), y(0), z(0) {};
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {};
    
    Vec3 operator + (const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator - (const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator * (const Vec3& o) const { return Vec3(x * o.x, y * o.y, z * o.z); }
    Vec3 operator * (const float d) const { return Vec3(x * d, y * d, z * d); }
    Vec3 operator / (const float d) const { return Vec3(x / d, y / d, z / d); }
    
    const Vec3& operator *= (float d) { x *= d; y *= d; z *= d; return *this; }
    const Vec3& operator /= (float d) { x /= d; y /= d; z /= d; return *this; }
    const Vec3& operator += (const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
    const Vec3& operator -= (const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
		
    Vec3 normalize() { float m = magnitude(); return Vec3(x / m, y / m, z / m); }
    float magnitude() { return sqrt(x * x + y * y + z * z); }
    Vec3 cross(const Vec3& o) const { return Vec3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x); }
    
    float dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
};

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

template<class T>
class Rect {
public:

    T x;
    T y;
    T w;
    T h;
    
    Rect() : x(0), y(0), w(0), h(0) {};
    Rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {};
    
    bool in(float x_, float y_) { return x <= x_ && x_ < x + w && y <= y_ && y_ < y + h; }
    
    bool intersects(Rect<T> o) { 
        return false; // !(x < o.x + o.w && x + w > o.x && y < o.y + o.h && y + h > o.y);
    }
};

typedef Rect<int> Recti;
typedef Rect<float> Rectf;

template<class T>
class Circle {
public:

    T x;
    T y;
    T r;
    
    Circle() : x(0), y(0), r(0) {};
    Circle(T x, T y, T r) : x(x), y(y), r(r) {};
    
    bool in(float x_, float y_) { return (x - x_) * (x - x_) + (y - y_) * (y - y_) < r * r; }
    bool intersects(Circle<T> o) { 
        return sqrt((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)) < (r + o.r); 
    }
};

typedef Circle<int> Circlei;
typedef Circle<float> Circlef;

extern float triangleSegmentIntersection(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f p1, Vec3f p2);
#endif // __VEC_HPP__
