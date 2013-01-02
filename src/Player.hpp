#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <set>

#include "Vec.hpp"

using namespace std;

class Player {
private:

    Vec3f mouse;
    Vec3f angle;  
    
    Vec3f position;
    set<unsigned char> keysDown;
    
    Rectf bounds;
    
public:

    Player();
    
    float getX() { return position.x; }
    float getY() { return position.y; }
    float getZ() { return position.z; }
    Vec3f getPosition() { return position; } 
    
    void setX(float x) { position.x = x; }
    void setY(float y) { position.y = y; }
    void setZ(float z) { position.z = z; }
    
    void setBounds(Rectf bounds) { this->bounds = bounds; } 
    
    float getAngleX() { return angle.x; }
    float getAngleY() { return angle.y; }
    
    void handleMouse(int x, int y);
    void handleKeyDown(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    
    void update();
    void draw();
};

#endif // __PLAYER_HPP__
