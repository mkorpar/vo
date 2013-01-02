#include<cstdio>
#include<cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <set>

#include "Vec.hpp"

#include "Player.hpp"

#define PI 3.14159265

Player::Player() {

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    mouse = Vec3f(w / 2, h / 2, 0);
    angle = Vec3f(0, 0, 0);
    
    position = Vec3f(0, 0, 0);
}

void Player::handleMouse(int x, int y) {

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    float dx = x - mouse.x;
    angle.y += dx / 5.0;
    
    float dy = y - mouse.y;
    angle.x += dy / 10.0;
    angle.x = min(angle.x,  30.0f);
    angle.x = max(angle.x, -30.0f);
    
    if (x < 50 || x > w - 50 || y < 50 || y > h - 50) {
        glutWarpPointer(w / 2, h / 2);
        mouse.x = w / 2;
        mouse.y = h / 2;
    } else {
        mouse.x = x;
        mouse.y = y;
    }
}

void Player::handleKeyDown(unsigned char key, int x, int y) {
    keysDown.insert(key);
}

void Player::handleKeyUp(unsigned char key, int x, int y) {
    keysDown.erase(key);
}

void Player::update() {

    float angle = this->angle.y * PI / 180.0;
    
    for (std::set<unsigned char>::iterator it = keysDown.begin(); it != keysDown.end(); ++it) {
        switch (*it) {
        case 'w':
        case 'W':
            position.x -= sin(angle);
            position.z += cos(angle);
            break;
        case 's':
        case 'S':
            position.x += sin(angle);
            position.z -= cos(angle);
            break;
        case 'd':
        case 'D':
            position.x -= sin(angle + PI / 2);
            position.z += cos(angle + PI / 2);
            break;
        case 'a':
        case 'A':
            position.x += sin(angle + PI / 2);
            position.z -= cos(angle + PI / 2);
            break;
        }
    }
}

void Player::draw() {

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    // OVERLAY 
    glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, -1.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    glBegin(GL_QUADS);
    glVertex2f(w / 2 - 1.0, h / 2 - 6.0);
    glVertex2f(w / 2 + 1.0, h / 2 - 6.0);
    glVertex2f(w / 2 + 1.0, h / 2 + 6.0);
    glVertex2f(w / 2 - 1.0, h / 2 + 6.0);
    glVertex2f(w / 2 - 6.0, h / 2 - 1.0);
    glVertex2f(w / 2 + 6.0, h / 2 - 1.0);
    glVertex2f(w / 2 + 6.0, h / 2 + 1.0);
    glVertex2f(w / 2 - 6.0, h / 2 + 1.0);
    glEnd();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); 
    
	glPopAttrib();
    // OVERLAY END 
}
