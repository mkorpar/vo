#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <cstdio>

#include "Vec.hpp"
#include "Terrain.hpp"
#include "Object.hpp"
#include "Skybox.hpp"

using namespace std;

static vector<Object*> objects;
static Vec3f viewpoint = Vec3f(0, 0, 0);
static Terrain* terrain;
static Skybox* skybox;

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
    gluPerspective(45, (double) w / (double) h, 1, 5000);
    
    gluLookAt(viewpoint.x, viewpoint.y, viewpoint.z, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 27: //Escape key
            exit(0);
        case 'a':
            viewpoint.x += 0.5;
            break;
        case 'd':
            viewpoint.x -= 0.5;
            break;
        case 'w':
            viewpoint.z += 0.5;
            break;
        case 's':
            viewpoint.z -= 0.5;
            break;
        case 'g':
            viewpoint.y += 0.5;
            break;
        case 'h':
            viewpoint.y -= 0.5;
            break;
    }

    resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glutPostRedisplay();
}

void draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lights();
    
    terrain->draw();
    skybox->draw();
    
    for (int i = 0; i < (int) objects.size(); ++i) {
        objects[i]->draw();
    }

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {


    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("VO");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);

    objects.push_back(new Object((char*) "cstl.obj", (char*) "cstl.png"));
    viewpoint = Vec3f(20.0f, 20.0f, 20.0f);
    terrain = new Terrain();
    terrain->generate();
    skybox = new Skybox();
    
    viewpoint + viewpoint;
    
    glutMainLoop();

    return 0;
}
