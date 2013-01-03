#ifndef __TERRAIN_HPP__
#define __TERRAIN_HPP__

#include <GL/gl.h>
#include <GL/glut.h>

#include "Object.hpp"
#include "Vec.hpp"

class Terrain : public Object {
private:

    Recti bounds;
    
    float* heights;
    Vec3f* normals;
    
    GLuint texId;

public:

    Terrain(Recti bounds, char* texture, char* heightmap);
    
    float getHeight(float x, float y);
    
    virtual void draw();
    virtual Circlef getBounds() { return Circlef(0, 0, 0); }
    virtual float intersection(Vec3f p1, Vec3f p2);
};

#endif // __TERRAIN_HPP__
