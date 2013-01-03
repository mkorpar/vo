#include<cstdio>
#include<cfloat>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "Vec.hpp"

#include "Object.hpp"

SimpleObject::SimpleObject(char* obj, char* tex) {

    FILE* file = fopen(obj, "r");

    float x, y, z;
    
    int v1, v2, v3;
    int vn1, vn2, vn3;
    int vt1, vt2, vt3;
    
    char line[4096];
    
    float minX = FLT_MAX;
    float maxX = FLT_MIN;
    float minZ = FLT_MAX;
    float maxZ = FLT_MIN;
    
    while (!feof(file)) {
    
        if (fscanf(file, "%[^\n]\n", line) != 1) break;
        
        if (sscanf(line, "v  %f %f %f", &x, &y, &z) == 3) {
            v.push_back(Vec3f(x, y, z));
            
            minX = min(minX, x);
            maxX = max(maxX, x);
            minZ = min(minZ, z);
            maxZ = max(maxZ, z);
        }
        
        if (sscanf(line, "vn %f %f %f", &x, &y, &z) == 3) {
            vn.push_back(Vec3f(x, y, z));
        }
        
        if (sscanf(line, "vt %f %f %f", &x, &y, &z) == 3) {
            vt.push_back(Vec3f(x, y, z));
        }
        
        if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
            &v1, &vn1, &vt1, &v2, &vn2, &vt2, &v3, &vn3, &vt3) == 9) {
            f.push_back(Vec3<Vec3i> (Vec3i(v1 - 1, vn1 - 1, vt1 - 1), 
                                     Vec3i(v2 - 1, vn2 - 1, vt2 - 1), 
                                     Vec3i(v3 - 1, vn3 - 1, vt3 - 1)));
        }
    }

    fclose(file);

    x = (maxX + minX) / 2;
    z = (maxZ + minZ) / 2;
    float r = max(max(x - minX, z - minZ), max(maxX - x, maxZ - z));
    
    bounds = Circlef(x, z, r + 1.5);

    texId = SOIL_load_OGL_texture
	(
	    tex,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

void SimpleObject::draw() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
    glBegin(GL_TRIANGLES);
    glColor4f(1, 1, 1, 1);
    
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
    
    glPopAttrib();
}

bool SimpleObject::intersects(Vec3f p, Vec3f k) {

    for (int i = 0; i < (int) f.size(); ++i) {

        Vec3f v1 = v[f[i].x.x];
        Vec3f v2 = v[f[i].y.x];
        Vec3f v3 = v[f[i].z.x];
        
        Vec3f edge1 = v2 - v1;
        Vec3f edge2 = v3 - v1;
       
        // Find the cross product of edge2 and the ray direction
        Vec3f s1 = k.cross(edge2);
       
        // Find the divisor, if its zero, return false as the triangle is
        // degenerated
        float divisor = s1.dot(edge1);
        if (divisor == 0.0) {
            continue;
        }
       
        // A inverted divisor, as multipling is faster then division
        float invDivisor = 1.0 / divisor;

        // Calculate the first barycentic coordinate. Barycentic coordinates
        // are between 0.0 and 1.0
        Vec3f distance = p - v1;
        float barycCoord_1 = distance.dot(s1) * invDivisor;
        
        if (barycCoord_1 < 0.0 || barycCoord_1 > 1.0) {
            continue;
        }
        
        // Calculate the second barycentic coordinate
        Vec3f s2 = distance.cross(edge1);
        float barycCoord_2 = k.dot(s2) * invDivisor;
        
        if (barycCoord_2 < 0.0 || (barycCoord_1 + barycCoord_2) > 1.0) {
            continue;
        }

        return true;      
    }

    return false;
}
