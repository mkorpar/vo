#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>

using namespace std;

template<class T>
class Vec3 {
public:
    T x;
    T y;
    T z;
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {};
};

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

class Object {
private:
    vector<Vec3f> v;
    vector<Vec3f> vn;
    vector<Vec3f> vt;
    vector<Vec3<Vec3i> > f;
    
    GLuint texId;

public:
    Object(char* obj, char* tex);
    void draw();
};

#endif // __OBJECT_HPP__
