#include <cfloat>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

#include "Vec.hpp"

#include "Object.hpp"

SimpleObject::SimpleObject(char* obj) {

    FILE* file = fopen(obj, "r");

    float x, y, z;
    
    int v1, v2, v3;
    int vn1, vn2, vn3;
    int vt1, vt2, vt3;
    
    char line[4096];
    
    float minX = FLT_MAX;
    float maxX = FLT_MIN;
    float minY = FLT_MAX;
    float maxY = FLT_MIN;
    float minZ = FLT_MAX;
    float maxZ = FLT_MIN;
    
    char mtl[4096];
    
    map<string, GLuint> textures;
    GLuint tex;
    
    while (!feof(file)) {
    
        if (fscanf(file, "%[^\n]\n", line) != 1) break;
        
        if (sscanf(line, "mtllib %s", mtl) == 1) {
            textures = readMtl(mtl);
            continue;
        }
        
        if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
        
            v.push_back(Vec3f(x, y, z));
            
            minX = min(minX, x);
            maxX = max(maxX, x);
            minY = min(minY, y);
            maxY = max(maxY, y);
            minZ = min(minZ, z);
            maxZ = max(maxZ, z);
            
            continue;
        }
        
        if (sscanf(line, "vn %f %f %f", &x, &y, &z) == 3) {
            vn.push_back(Vec3f(x, y, z));
            continue;
        }
        
        if (sscanf(line, "vt %f %f", &x, &y) == 2) {
            vt.push_back(Vec3f(x, y, 0));
            continue;
        }
        
        if (sscanf(line, "usemtl %s", mtl) == 1) {
            tex = textures[string(mtl)];
            continue;
        }
        
        if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
            &v1, &vn1, &vt1, &v2, &vn2, &vt2, &v3, &vn3, &vt3) == 9) {
            
            Vec3<Vec3i> f1(Vec3i(v1 - 1, vn1 - 1, vt1 - 1), 
                           Vec3i(v2 - 1, vn2 - 1, vt2 - 1), 
                           Vec3i(v3 - 1, vn3 - 1, vt3 - 1));
            
            fs[tex].push_back(f1);
            
            continue;
        }
    }

    fclose(file);

    x = (maxX + minX) / 2;
    y = (maxY + minY) / 2;
    z = (maxZ + minZ) / 2;
    
    bounds = Rectf(minX - 6, minZ - 6, 12 + maxX - minX, 12 + maxZ - minZ);
    center = Vec3f(x, y, z);
}

map<string, GLuint> SimpleObject::readMtl(char* path) {

    FILE* file = fopen(path, "r");
    
    char line[4096];
    
    char n[4096];
    char f[4096];
    
    map<string, GLuint> textures;

    while (!feof(file)) {
    
        if (fscanf(file, "%[^\n]\n", line) != 1) break;
        
        if (sscanf(line, "newmtl %s", n) == 1) {
            continue;
        }
        
        if (sscanf(line, "map_Kd %s", f) == 1) {
            
            GLuint texId = SOIL_load_OGL_texture
	        (
	            f,
		        SOIL_LOAD_AUTO,
		        SOIL_CREATE_NEW_ID,
		        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	        );
	        
            textures[string(n)] = texId;
            
            continue;
        }
    }
    
    return textures;
}

void SimpleObject::draw() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    glColor4f(1, 1, 1, 1);

    for (map<GLuint, vector<Vec3<Vec3i> > >::iterator it = fs.begin(); it != fs.end(); it++) {
        
        vector<Vec3<Vec3i> >& f = it->second;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, it->first);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBegin(GL_TRIANGLES);
        
        for (int i = 0; i < (int) f.size(); ++i) {

            Vec3f v1 = v[f[i].x.x];
            Vec3f vt1 = vt[f[i].x.y];
            Vec3f vn1 = vn[f[i].x.z];  
      
            Vec3f v2 = v[f[i].y.x];
            Vec3f vt2 = vt[f[i].y.y];
            Vec3f vn2 = vn[f[i].y.z]; 
            
            Vec3f v3 = v[f[i].z.x];
            Vec3f vt3 = vt[f[i].z.y];
            Vec3f vn3 = vn[f[i].z.z];       
            
        	glNormal3f(vn1.x, vn1.y, vn1.z);
	        glTexCoord2f(vt1.x, vt1.y);
            glVertex3f(v1.x, v1.y, v1.z);

            glNormal3f(vn2.x, vn2.y, vn2.z);
		    glTexCoord2f(vt2.x, vt2.y);
            glVertex3f(v2.x, v2.y, v2.z);
            
    	    glNormal3f(vn3.x, vn3.y, vn3.z);
		    glTexCoord2f(vt3.x, vt3.y);
            glVertex3f(v3.x, v3.y, v3.z);
        }

        glEnd(); 
    }
    
    glPopAttrib();
}

void SimpleObject::scale(float x, float y, float z) {

    for (int i = 0; i < (int) v.size(); ++i) {
        v[i].x *= x;
        v[i].y *= y;
        v[i].z *= z;
    }
    
    center.x *= x;
    center.y *= y;
    center.z *= z;
    
    bounds.x *= x;
    bounds.y *= z;
    bounds.w *= x;
    bounds.h *= z;
}

void SimpleObject::translate(float x, float y, float z) {

    for (int i = 0; i < (int) v.size(); ++i) {
        v[i].x += x;
        v[i].y += y;
        v[i].z += z;
    }
    
    center.x += x;
    center.y += y;
    center.z += z;
    
    bounds.x += x;
    bounds.y += z;
}

void SimpleObject::rotateY(float angle_) {

    float angle = angle_ * 3.14159265 / 180.0;

    float c = cos(angle);
    float s = sin(angle);
    
    float x, z;
    
    for (int i = 0; i < (int) v.size(); ++i) {
        x = v[i].x * c + v[i].z * s;
        z = v[i].x * -s + v[i].z * c; 
        v[i].x = x;
        v[i].z = z;
    }
    
    for (int i = 0; i < (int) vn.size(); ++i) {
        x = vn[i].x * c + vn[i].z * s;
        z = vn[i].x * -s + vn[i].z * c; 
        vn[i].x = x;
        vn[i].z = z;
    }
    
    x = center.x * c + center.z * s;
    z = center.x * -s + center.z * c; 
    center.x = x;
    center.z = z;
    
    x = bounds.x * c + bounds.y * s;
    z = bounds.x * -s + bounds.y * c; 
    bounds.x = x;
    bounds.y = z;
}

float SimpleObject::intersection(Vec3f p1, Vec3f p2) {

    float d = NO_INTERSECTION;

    for (map<GLuint, vector<Vec3<Vec3i> > >::iterator it = fs.begin(); it != fs.end(); it++) {
        
        vector<Vec3<Vec3i> >& f = it->second;

        for (int i = 0; i < (int) f.size(); ++i) {

            Vec3f v1 = v[f[i].x.x];
            Vec3f v2 = v[f[i].y.x];
            Vec3f v3 = v[f[i].z.x];
            
            float d0 = triangleSegmentIntersection(v1, v2, v3, p1, p2);

            if (d == NO_INTERSECTION) d = d0;
            else if (d0 != NO_INTERSECTION && d0 < d) d = d0;
        }
    }

    return d;
}
