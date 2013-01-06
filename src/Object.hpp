#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <GL/gl.h>
#include <GL/glut.h>
#include <map>
#include <vector>
#include <string>

#include "Vec.hpp"

using namespace std;

class Object {
public:
    virtual void draw() = 0;
    virtual Rectf getBounds() = 0;
    virtual float intersection(Vec3f p1, Vec3f p2) { return NO_INTERSECTION; }
};

class Texture {
public:
    GLuint id;
    bool transparent;
    Texture() {};
    Texture(GLuint id, bool transparent) : id(id), transparent(transparent) {};
    bool operator<( const Texture& other) const { return id < other.id; }
};

class SimpleObject : public Object {
private:

    vector<Vec3f> v;
    vector<Vec3f> vn;
    vector<Vec3f> vt;
    
    map<string, Texture> readMtl(char* path);
    map<Texture, vector<Vec3<Vec3i> > > fs;
    
    Rectf bounds;
    Vec3f center;

public:

    SimpleObject(char* obj);
    SimpleObject(const SimpleObject& other);
    
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotateY(float angle);
    
    Vec3f getCenter() { return center; }
    void setBounds(Rectf bounds) { this->bounds = bounds; }

    virtual void draw();
    virtual Rectf getBounds() { return bounds; }
    virtual float intersection(Vec3f p1, Vec3f p2);
};

#endif // __OBJECT_HPP__
