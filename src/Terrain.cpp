#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "Vec.hpp"

#include "Terrain.hpp"

Terrain::Terrain(Recti bounds, char* texture, char* heightmap) : bounds(bounds) {

    heights = (float*) malloc(bounds.w * bounds.h * sizeof(float));
    normals = (Vec3f*) malloc(bounds.w * bounds.h * sizeof(Vec3f));
    
    memset(heights, 0, bounds.w * bounds.h * sizeof(float));
    
    int width, height, channels;
    unsigned char* map = SOIL_load_image
	(
		heightmap,
		&width, &height, &channels,
		SOIL_LOAD_L
	);
	
	for (int i = 0; i < height; ++i) {
	    for (int j = 0; j < width; ++j) {
	        heights[i * bounds.w + j] = map[i * width + j] / 255.0 * 5.0;
	    }
	}

	Vec3f* normals2 = (Vec3f*) malloc(bounds.w * bounds.h * sizeof(Vec3f));
			
	for(int z = 0; z < bounds.h; z++) {
		for(int x = 0; x < bounds.w; x++) {
		
			Vec3f sum(0.0f, 0.0f, 0.0f);
			
			Vec3f out;
			Vec3f in;
			Vec3f left;
			Vec3f right;
			
			if (z > 0) out = Vec3f(0.0f, heights[(z - 1) * bounds.h + x] - heights[z * bounds.h + x], -1.0f);
			if (z <  bounds.h - 1) in = Vec3f(0.0f, heights[(z + 1) * bounds.h + x] - heights[z * bounds.h + x], 1.0f);
			if (x > 0) left = Vec3f(-1.0f, heights[z * bounds.h + x - 1] - heights[z * bounds.h + x], 0.0f);
			if (x <  bounds.w - 1) right = Vec3f(1.0f, heights[z * bounds.h + x + 1] - heights[z * bounds.h + x], 0.0f);
			
			if (x > 0 && z > 0) sum += out.cross(left).normalize();
			if (x > 0 && z <  bounds.h - 1) sum += left.cross(in).normalize();
			if (x <  bounds.w - 1 && z <  bounds.h - 1) sum += in.cross(right).normalize();
			if (x <  bounds.w - 1 && z > 0) sum += right.cross(out).normalize();
			
			normals2[z * bounds.h + x] = sum;
		}
	}
	
	for(int z = 0; z < bounds.h; ++z) {
		for(int x = 0; x < bounds.w; ++x) {
		
			Vec3f sum = normals2[z * bounds.h + x];
			
			if (x > 0) sum += normals2[z * bounds.h + x - 1] * 0.5f;
			if (x <  bounds.w - 1) sum += normals2[z * bounds.h + x + 1] * 0.5f;
			if (z > 0) sum += normals2[(z - 1) * bounds.h + x] * 0.5f;
			if (z <  bounds.h - 1) sum += normals2[(z + 1) * bounds.h + x] * 0.5f;
			
			if (sum.magnitude() == 0) sum = Vec3f(0.0f, 1.0f, 0.0f);
			
			normals[z * bounds.h + x] = sum;
		}
	}
			
		
    texId = SOIL_load_OGL_texture
	(
	    texture,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

void Terrain::draw() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
	glColor3f(0.3f, 0.9f, 0.0f);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	for(int i = 0; i < bounds.h - 1; ++i) {
	
		glBegin(GL_TRIANGLE_STRIP);
		
		for(int j = 0; j < bounds.w; ++j) {
		
		    Vec3f n1 = normals[i * bounds.w + j];
  		    Vec3f n2 = normals[(i + 1) * bounds.w + j];
  		    
			glTexCoord2f(i / 12.0f, j / 12.0f);
	    	glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(bounds.x + j, heights[i * bounds.w + j], bounds.y + i);
			glTexCoord2f((i + 1) / 12.0f, j / 12.0f);
	    	glNormal3f(n2.x, n2.y, n2.z);
			glVertex3f(bounds.x + j, heights[(i + 1) * bounds.w + j], bounds.y + i + 1);
		}
		
		glEnd();
	}
	
    glPopAttrib();
}

float Terrain::getHeight(float x, float y) {
    return heights[(int) ((round(y) - bounds.y) * bounds.w + round(x) - bounds.x)];
}

float Terrain::intersection(Vec3f p1, Vec3f p2) {

    float d = NO_INTERSECTION;
    
	for(int i = 0; i < bounds.h - 1; ++i) {
		for(int j = 0; j < bounds.w - 1; ++j) {
    		for(int l = 0; l < 2; ++l) {

			    Vec3f v1(bounds.x + j + l, heights[(i + l) * bounds.w + j + l], bounds.y + i + l);
			    Vec3f v2(bounds.x + j + 1, heights[i * bounds.w + j + 1], bounds.y + i);
			    Vec3f v3(bounds.x + j, heights[(i + 1) * bounds.w + j], bounds.y + i + 1);

                float d0 = triangleSegmentIntersection(v1, v2, v3, p1, p2);
                
                if (d == NO_INTERSECTION) d = d0;
                else if (d0 != NO_INTERSECTION && d0 < d) d = d0;
            }    
		}
	}
	
    return d;
}
