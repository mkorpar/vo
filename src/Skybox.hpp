#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <GL/gl.h>
#include <GL/glut.h>

#include "Vec.hpp"

class Skybox {
private:
    GLuint texIds[6];
    Vec3f center;
public:
    Skybox();
    void draw();
    
    void setCenter(Vec3f center) { this->center = center; };
};

#endif // __SKYBOX_HPP__
