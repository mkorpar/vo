#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>

#include "Vec.hpp"

using namespace std;

class Object {
public:
    virtual void draw() = 0;
    virtual Circlef getBounds() = 0;
    virtual float intersection(Vec3f p1, Vec3f p2) { return NO_INTERSECTION; }
};

class SimpleObject : public Object {
private:

    vector<Vec3f> v;
    vector<Vec3f> vn;
    vector<Vec3f> vt;
    vector<Vec3<Vec3i> > f;
    
    Circlef bounds;
    
    GLuint texId;

public:

    SimpleObject(char* obj, char* tex);
    
    virtual void draw();
    virtual Circlef getBounds() { return bounds; }
    virtual float intersection(Vec3f p1, Vec3f p2);
};

#endif // __OBJECT_HPP__
