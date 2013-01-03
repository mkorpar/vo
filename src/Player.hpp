#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <set>
#include <vector>

#include "Object.hpp"
#include "Vec.hpp"

using namespace std;

class Player {
private:

    Vec3f mouse;
    Vec3f angle;  
    
    Vec3f position;
    set<unsigned char> keysDown;
    set<int> mouseKeysDown;
        
    Rectf bounds;
    vector<Object*> objects;
    
    bool isRestricted(float x, float y);
    
    Object* target;
    int targetHits;
    int targetTimeout;
public:

    Player();
    
    float getX() { return position.x; }
    float getY() { return position.y; }
    float getZ() { return position.z; }
    
    void setX(float x) { position.x = x; }
    void setY(float y) { position.y = y; }
    void setZ(float z) { position.z = z; }
    
    Vec3f getPosition() { return position; } 
    
    void setBounds(Rectf bounds) { this->bounds = bounds; } 
    void addObject(Object* object) { objects.push_back(object); }
    
    float getAngleX() { return angle.x; }
    float getAngleY() { return angle.y; }

    Object* getTarget() { return target; }
    void setTarget(Object* target) { this->target = target; }
    
    void handleMouse(int x, int y);
    void handleMouseClick(int button, int state, int x, int y);
    void handleKeyDown(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    
    void update();
    void draw();
};

#endif // __PLAYER_HPP__
