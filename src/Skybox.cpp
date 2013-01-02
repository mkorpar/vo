#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "Skybox.hpp"

Skybox::Skybox() {

    char* files[5] = {  
        (char*) "skybox1.bmp", 
        (char*) "skybox2.bmp", 
        (char*) "skybox3.bmp",
        (char*) "skybox4.bmp", 
        (char*) "skybox5.bmp"
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
}
void Skybox::draw() {

    glColor4f(1, 1, 1, 1);
 
 	glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( 200.0f, -200.0f, -200.0f);
        glTexCoord2f(1, 0); glVertex3f(-200.0f, -200.0f, -200.0f);
        glTexCoord2f(1, 1); glVertex3f(-200.0f,  200.0f, -200.0f);
        glTexCoord2f(0, 1); glVertex3f( 200.0f,  200.0f, -200.0f);
    glEnd();
 
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, texIds[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( 200.0f, -200.0f,  200.0f);
        glTexCoord2f(1, 0); glVertex3f( 200.0f, -200.0f, -200.0f);
        glTexCoord2f(1, 1); glVertex3f( 200.0f,  200.0f, -200.0f);
        glTexCoord2f(0, 1); glVertex3f( 200.0f,  200.0f,  200.0f);
    glEnd();
 
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, texIds[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-200.0f, -200.0f,  200.0f);
        glTexCoord2f(1, 0); glVertex3f( 200.0f, -200.0f,  200.0f);
        glTexCoord2f(1, 1); glVertex3f( 200.0f,  200.0f,  200.0f);
        glTexCoord2f(0, 1); glVertex3f(-200.0f,  200.0f,  200.0f);
 
    glEnd();
 
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-200.0f, -200.0f, -200.0f);
        glTexCoord2f(1, 0); glVertex3f(-200.0f, -200.0f,  200.0f);
        glTexCoord2f(1, 1); glVertex3f(-200.0f,  200.0f,  200.0f);
        glTexCoord2f(0, 1); glVertex3f(-200.0f,  200.0f, -200.0f);
    glEnd();
 
    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, texIds[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-200.0f,  200.0f, -200.0f);
        glTexCoord2f(0, 0); glVertex3f(-200.0f,  200.0f,  200.0f);
        glTexCoord2f(1, 0); glVertex3f( 200.0f,  200.0f,  200.0f);
        glTexCoord2f(1, 1); glVertex3f( 200.0f,  200.0f, -200.0f);
    glEnd();
}
