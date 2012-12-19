#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <GL/gl.h>
#include <GL/glut.h>

class Skybox {
private:

    GLuint texIds[6];
    
public:
    Skybox();
    void draw();
};

#endif // __SKYBOX_HPP__
