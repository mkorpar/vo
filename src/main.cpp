#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <cstdio>
#include <cmath>
#include <ctime>

#include "Player.hpp"
#include "Object.hpp"
#include "Skybox.hpp"
#include "Terrain.hpp"
#include "Vec.hpp"

using namespace std;

static vector<Object*> objects;
static Terrain* terrain;
static Skybox* skybox;
static Player* player;
static SimpleObject* target;

void lights() {

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    
    GLfloat lightColor0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos0[] = { 50.0f, 50.0f, 50.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double) w / (double) h, 1, 15000);
}

void mouse(int x, int y) {
    player->handleMouse(x, y);
}

void mouseClick(int button, int state, int x, int y) {
    player->handleMouseClick(button, state, x, y);
}

void keyDown(unsigned char key, int x, int y) {
    player->handleKeyDown(key, x, y);
}

void keyUp(unsigned char key, int x, int y) {
    player->handleKeyUp(key, x, y);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(25, timer, 0);
}

void placeTarget() {

    Recti bounds = terrain->getRectBounds();
    Vec3f center = target->getCenter();
    
    Vec3f d;
    
    while (true) {
    
        d.x = (rand() % bounds.w) + bounds.x;
        d.z = (rand() % bounds.h) + bounds.y;
        
        Vec3f c = center + d;

        if (!bounds.in(c.x, c.z)) {
            continue;
        }
        
        if (fabs(c.x - player->getX()) < 5 || fabs(c.z - player->getZ()) < 5) {
            continue;
        }
        
        bool ok = true;
        for (int i = 0; i < (int) objects.size(); ++i) {
            if (objects[i] != target && 
                objects[i]->getBounds().intersects(target->getBounds())) {
                ok = false;
                break;
            }
        }
        
        if (!ok) {
            continue;
        }
        
        d.y = 1 + terrain->getHeight(c.x, c.z) - center.y;
        break;
    }
    
    target->translate(d.x, d.y, d.z);
    player->setTarget(target);
}

void draw() {

    if (player->getTarget() == NULL) {
        placeTarget();
    }
    
    player->setY(terrain->getHeight(player->getX(), player->getZ()) + 2);
    player->update();
    skybox->setCenter(player->getPosition());
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lights();
    
    glRotatef(player->getAngleX(), 1, 0, 0);
    glRotatef(player->getAngleY(), 0, 1, 0);
    glTranslatef(-player->getX(), -player->getY(), -player->getZ());
    
    player->draw();
    
    for (int i = 0; i < (int) objects.size(); ++i) {
        objects[i]->draw();
    }

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    
    srand(time(NULL));

    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("VO");

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMotionFunc(mouse);
    glutPassiveMotionFunc(mouse);
    glutMouseFunc(mouseClick);
    glutTimerFunc(25, timer, 0);
              
    glutSetCursor(GLUT_CURSOR_NONE); 
 
    skybox = new Skybox();
    terrain = new Terrain(Recti(-50, -50, 101, 101), (char*) "textures/grass.jpg", 
        (char*) "textures/terrain.tga");
    
    player = new Player();
    player->setBounds(Rectf(-50, -50, 100, 100));
        
    target = new SimpleObject((char*) "cstl.obj", (char*) "textures/cstl.png");
    
    objects.push_back(terrain);
    objects.push_back(skybox);
    objects.push_back(target);
    
    for (int i = 0; i < (int) objects.size(); ++i) {
        player->addObject(objects[i]);
    }
    
    glutMainLoop();

    return 0;
}
