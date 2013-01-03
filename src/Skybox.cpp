#include <cstdio>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "Vec.hpp"

#include "Skybox.hpp"

Skybox::Skybox() {

    char* files[5] = {  
        (char*) "textures/skybox1.bmp", 
        (char*) "textures/skybox2.bmp", 
        (char*) "textures/skybox3.bmp",
        (char*) "textures/skybox4.bmp", 
        (char*) "textures/skybox5.bmp"
    };
    
    for (int i = 0; i < 5; ++i) {
        texIds[i] = SOIL_load_OGL_texture
	    (
	        files[i],
		    SOIL_LOAD_AUTO,
		    SOIL_CREATE_NEW_ID,
		    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	    ); 
    }
    
    center = Vec3f(0, 0, 0);
}
void Skybox::draw() {

 
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    glColor4f(1, 1, 1, 1);
    
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( 2000.0f + center.x, -2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(1, 0); glVertex3f(-2000.0f + center.x, -2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(1, 1); glVertex3f(-2000.0f + center.x,  2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(0, 1); glVertex3f( 2000.0f + center.x,  2000.0f + center.y, -2000.0f + center.z);
    glEnd();

    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( 2000.0f + center.x, -2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(1, 0); glVertex3f( 2000.0f + center.x, -2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(1, 1); glVertex3f( 2000.0f + center.x,  2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(0, 1); glVertex3f( 2000.0f + center.x,  2000.0f + center.y,  2000.0f + center.z);
    glEnd();
 
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, texIds[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-2000.0f + center.x, -2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(1, 0); glVertex3f( 2000.0f + center.x, -2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(1, 1); glVertex3f( 2000.0f + center.x,  2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(0, 1); glVertex3f(-2000.0f + center.x,  2000.0f + center.y,  2000.0f + center.z);
 
    glEnd();

    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, texIds[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-2000.0f + center.x, -2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(1, 0); glVertex3f(-2000.0f + center.x, -2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(1, 1); glVertex3f(-2000.0f + center.x,  2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(0, 1); glVertex3f(-2000.0f + center.x,  2000.0f + center.y, -2000.0f + center.z);
    glEnd();

    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, texIds[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-2000.0f + center.x,  2000.0f + center.y, -2000.0f + center.z);
        glTexCoord2f(0, 0); glVertex3f(-2000.0f + center.x,  2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(1, 0); glVertex3f( 2000.0f + center.x,  2000.0f + center.y,  2000.0f + center.z);
        glTexCoord2f(1, 1); glVertex3f( 2000.0f + center.x,  2000.0f + center.y, -2000.0f + center.z);
    glEnd();
    
    glPopAttrib();
}
