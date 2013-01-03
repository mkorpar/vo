#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <GL/gl.h>
#include <GL/glut.h>

#include "Object.hpp"
#include "Vec.hpp"

class Skybox : public Object {
private:
    GLuint texIds[6];
    Vec3f center;
public:

    Skybox();
    
    void setCenter(Vec3f center) { this->center = center; };
    
    virtual void draw();
    virtual Circlef getBounds() { return Circlef(0, 0, 0); }
};

#endif // __SKYBOX_HPP__
