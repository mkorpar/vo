#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <cstdio>
#include <cmath>
#include <ctime>

#include "Player.hpp"
#include "Object.hpp"
#include "Skybox.hpp"
#include "Terrain.hpp"
#include "Util.hpp"
#include "Vec.hpp"

using namespace std;

static vector<Object*> objects;
static vector<Object*> collision;
static Terrain* terrain;
static Skybox* skybox;
static Player* player;
static SimpleObject* target;

void lights() {

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    
    GLfloat lightColor0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos0[] = { 100.0f, 80.0f, -100.0f, 1.0f };
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

void placeTarget() {

    Recti bounds = terrain->getRectBounds();
    Vec3f center = target->getCenter();
    
    Vec3f d;
    
    while (true) {
    
        d.x = (rand() % bounds.w) + bounds.x;
        d.z = (rand() % bounds.h) + bounds.y;
        
        Vec3f c = center + d;

        if (
            bounds.x + 5 > c.x || bounds.x + bounds.w - 5 < c.x ||
            bounds.y + 5 > c.z || bounds.y + bounds.h - 5 < c.z
        ) {
            continue;
        }
        
        if (fabs(c.x - player->getX()) <= 15 || fabs(c.z - player->getZ()) <= 15) {
            continue;
        }
        
        bool ok = true;
        for (int i = 0; i < (int) collision.size(); ++i) {
            if (collision[i] != terrain && collision[i] != target &&
                collision[i]->getBounds().intersects(target->getBounds())) {
                ok = false;
                break;
            }
        }
        
        if (!ok) {
            continue;
        }
        
        d.y = 0.9 + terrain->getHeight(c.x, c.z) - center.y;
        break;
    }
    
    target->translate(d.x, d.y, d.z);
    player->setTarget(target);
}

void update() {

    if (player->getTarget() == NULL) {
        placeTarget();
    }
    
    player->setY(terrain->getHeight(player->getX(), player->getZ()) + 2);
    player->update();
    skybox->setCenter(player->getPosition());
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(25, timer, 0);
}

void draw() {

    update();
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // CAMERA
    glRotatef(player->getAngleX(), 1, 0, 0);
    glRotatef(player->getAngleY(), 0, 1, 0);
    glTranslatef(-player->getX(), -player->getY(), -player->getZ());
    
    // LIGHTS
    lights();
    
    // 2D
    player->draw();

    // 3D
    for (int i = 0; i < (int) objects.size(); ++i) {
        objects[i]->draw();
    }

    glutSwapBuffers();
}

void useShaders() {

    glewInit();

	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

	char* vs = textFileRead((char*) "shader.vert");
	char* fs = textFileRead((char*) "shader.frag");

	const char* ff = fs;
	const char* vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);
	free(fs);

	glCompileShader(v);
	glCompileShader(f);

	GLuint p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
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
    glEnable(GL_BLEND);

    glDepthFunc(GL_LEQUAL);	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    player->setBounds(Rectf(-48, -48, 96, 96));
    
    target = new SimpleObject((char*) "box.obj");
    target->scale(0.2, 0.2, 0.2);
    
    SimpleObject* tree1 = new SimpleObject((char*) "tree1.obj");
    tree1->scale(0.03, 0.03, 0.03);
    tree1->setBounds(Rectf(-1, -1.5, 3.5, 3.5));
    
    SimpleObject* tree2 = new SimpleObject(*tree1);
    SimpleObject* tree3 = new SimpleObject(*tree1);
    SimpleObject* tree4 = new SimpleObject(*tree1);
    
    tree1->translate(-5, terrain->getHeight(-5, -5) - 1, -5);
    tree2->translate(6, terrain->getHeight(6, 25) - 1, 25);
    tree3->translate(-15, terrain->getHeight(-15, -25) - 1, -25);
    tree4->translate(30, terrain->getHeight(30, 10) - 1, 10);
    
    SimpleObject* wall1 = new SimpleObject((char*) "wall.obj");
    SimpleObject* wall2 = new SimpleObject(*wall1);
    SimpleObject* wall3 = new SimpleObject(*wall1);
    SimpleObject* wall4 = new SimpleObject(*wall1);
    
    wall1->translate(0, 0, 50);
    wall2->translate(0, 0, -50);
    wall3->rotateY(90);
    wall3->translate(50, 0, 0);
    wall4->rotateY(90);
    wall4->translate(-50, 0, 0);

    objects.push_back(target);
    objects.push_back(wall1);
    objects.push_back(wall2);
    objects.push_back(wall3);
    objects.push_back(wall4);
    objects.push_back(terrain);
    objects.push_back(skybox);
    objects.push_back(tree1);
    objects.push_back(tree2);
    objects.push_back(tree3);
    objects.push_back(tree4);
    
    collision.push_back(target);
    collision.push_back(terrain);
    collision.push_back(tree1);
    collision.push_back(tree2);
    collision.push_back(tree3);
    collision.push_back(tree4);
    
    for (int i = 0; i < (int) collision.size(); ++i) {
        player->addObject(collision[i]);
    }

    // useShaders();
    glutMainLoop();

    return 0;
}
