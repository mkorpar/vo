#ifndef __TERRAIN_HPP__
#define __TERRAIN_HPP__

#include <GL/gl.h>
#include <GL/glut.h>

#include "Vec.hpp"

class Terrain {
private:

    Recti bounds;
    
    float* heights;
    Vec3f* normals;
    
    GLuint texId;
public:

    Terrain(Recti bounds, char* texture, char* heightmap);
    
    float getHeight(float x, float y);
    void draw();
};

#endif // __TERRAIN_HPP__
