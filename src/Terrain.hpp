#ifndef __TERRAIN_HPP__
#define __TERRAIN_HPP__

#include <GL/gl.h>
#include <GL/glut.h>

#include "Vec.hpp"

class Terrain {
private:

    int width;
    int height;
    
    float* heights;
    Vec3f* normals;
    
    GLuint texId;
    
public:
    Terrain();
    Terrain(int width, int height);
    void generate();
    void draw();
    float getHeight(float x, float y);
};

#endif // __TERRAIN_HPP__
