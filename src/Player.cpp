#include <cfloat>
#include <cmath>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glut.h>
#include <set>
#include <string>
#include <sstream>

#include "Vec.hpp"

#include "Player.hpp"

using namespace std;

#define PI 3.14159265

Player::Player() {

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    mouse = Vec3f(w / 2, h / 2, 0);
    angle = Vec3f(0, 0, 0);
    
    position = Vec3f(0, 0, 0);
    
    target = NULL;
    targetHits = 0;
    targetTimeout = 0;
}

void Player::handleMouse(int x, int y) {

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    float dx = x - mouse.x;
    
    if (abs(dx) < 50) {
        angle.y += (dx / 5.0);
        if (angle.y <= -360) angle.y += 360;
        if (angle.y >=  360) angle.y -= 360;
    }
    
    float dy = y - mouse.y;
    if (abs(dy) < 50) {
        angle.x += dy / 10.0;
        angle.x = min(angle.x,  45.0f);
        angle.x = max(angle.x, -45.0f);
    }
    
    if (x < 50 || x > w - 50 || y < 50 || y > h - 50) {
        glutWarpPointer(w / 2, h / 2);
        mouse.x = w / 2;
        mouse.y = h / 2;
    } else {
        mouse.x = x;
        mouse.y = y;
    }
}

void Player::handleMouseClick(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouseKeysDown.insert(button);
    } else {
        mouseKeysDown.erase(button);
    }
}

void Player::handleKeyDown(unsigned char key, int x, int y) {
    keysDown.insert(key);
}

void Player::handleKeyUp(unsigned char key, int x, int y) {
    keysDown.erase(key);
}

void Player::update() {

    float alpha = this->angle.x * PI / 180.0;
    float beta = this->angle.y * PI / 180.0;
    
    // KEYBOARD
    
    float x;
    float z; 
      
    for (std::set<unsigned char>::iterator it = keysDown.begin(); it != keysDown.end(); ++it) {
        switch (*it) {
        case 'w':
        case 'W':
            x = position.x + sin(beta);
            z = position.z - cos(beta);
            
            if (!isRestricted(x, z)) {
                position.x = x;
                position.z = z;
            }
            break;
        case 's':
        case 'S':
            x = position.x - sin(beta);
            z = position.z + cos(beta);
            
            if (!isRestricted(x, z)) {
                position.x = x;
                position.z = z;
            }
            break;
        case 'd':
        case 'D':
            x = position.x + sin(beta + PI / 2);
            z = position.z - cos(beta + PI / 2);
            
            if (!isRestricted(x, z)) {
                position.x = x;
                position.z = z;
            }
            break;
        case 'a':
        case 'A':
            x = position.x - sin(beta + PI / 2);
            z = position.z + cos(beta + PI / 2);
            
            if (!isRestricted(x, z)) {
                position.x = x;
                position.z = z;
            }
            break;
        }
    }
    
    position.x = max(bounds.x, position.x);
    position.x = min(bounds.x + bounds.w, position.x);

    position.z = max(bounds.y, position.z);
    position.z = min(bounds.y + bounds.h, position.z);
    // KEYBOARD END
    
    // MOUSE
    targetTimeout--;
    
    if (target != NULL) {
    
        Vec3f p2(position.x - bounds.w * sin(-beta), 
                 position.y + bounds.h * tan(-alpha),
                 position.z - bounds.h * cos(-beta));
        
        for (std::set<int>::iterator it = mouseKeysDown.begin(); it != mouseKeysDown.end(); ++it) {
            if (*it == GLUT_LEFT_BUTTON && targetTimeout <= 0) {
            
                targetTimeout = 0;

                float t = target->intersection(position, p2);

                if (t == NO_INTERSECTION) continue;
                
                float d = NO_INTERSECTION;
                
                for (int i = 0; i < (int) objects.size(); ++i) {
                    if (objects[i] != target) {
                    
                        float d0 = objects[i]->intersection(position, p2);
                        
                        if (d == NO_INTERSECTION) d = d0;
                        else if (d0 != NO_INTERSECTION && d0 < d) d = d0;
        
                        if (d < t) break;
                    }
                }
                
                if (d == NO_INTERSECTION || d > t) {
                    targetHits++;
                    printf("bum %d %f\n", targetHits, t);
                }
            }
        }
    }
    // MOUSE END
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

    stringstream ss;
    ss << "Score: ";
    ss << targetHits;
    string str = ss.str();
    
    glRasterPos2f(10, 25);
    for (int i = 0; i < (int) str.size(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str.at(i));
    }
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); 
    
	glPopAttrib();
    // OVERLAY END 
}

bool Player::isRestricted(float x, float y) {

    for (int i = 0; i < (int) objects.size(); ++i) {
        if (objects[i]->getBounds().in(x, y)) {
            return true;
        }
    }
    
    return false;
}
