#ifndef __TERRAIN_HPP__
#define __TERRAIN_HPP__

#include <cfloat>
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
    Recti getRectBounds() { return bounds; }
    
    virtual void draw();
    virtual Circlef getBounds() { return Circlef(FLT_MIN, FLT_MIN, 0); }
    virtual float intersection(Vec3f p1, Vec3f p2);
};

#endif // __TERRAIN_HPP__
