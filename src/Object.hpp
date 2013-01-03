#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>

#include "Vec.hpp"

using namespace std;

class Object {
private:

    vector<Vec3f> v;
    vector<Vec3f> vn;
    vector<Vec3f> vt;
    vector<Vec3<Vec3i> > f;
    
    Circlef bounds;
    
    GLuint texId;

public:

    Object(char* obj, char* tex);
    
    Circlef getBounds() { return bounds; }
    
    void draw();
};

#endif // __OBJECT_HPP__
