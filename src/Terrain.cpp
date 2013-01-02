#include <cstdlib>
#include <ctime>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "Vec.hpp"

#include "Terrain.hpp"

Terrain::Terrain() : width(100), height(100), heights(NULL), normals(NULL) {}

void Terrain::generate() {

    srand(time(NULL));

    free(heights);
    free(normals);
    
    heights = (float*) malloc(width * height * sizeof(float));
    normals = (Vec3f*) malloc(width * height * sizeof(Vec3f));
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            heights[i * width + j] = (rand() % 100) / 100.0;
        }
    }
    
	Vec3f* normals2 = (Vec3f*) malloc(width * height * sizeof(Vec3f));
			
	for(int z = 0; z < height; z++) {
		for(int x = 0; x < width; x++) {
		
			Vec3f sum(0.0f, 0.0f, 0.0f);
			
			Vec3f out;
			Vec3f in;
			Vec3f left;
			Vec3f right;
			
			if (z > 0) out = Vec3f(0.0f, heights[(z - 1) * height + x] - heights[z * height + x], -1.0f);
			if (z <  height - 1) in = Vec3f(0.0f, heights[(z + 1) * height + x] - heights[z * height + x], 1.0f);
			if (x > 0) left = Vec3f(-1.0f, heights[z * height + x - 1] - heights[z * height + x], 0.0f);
			if (x <  width - 1) right = Vec3f(1.0f, heights[z * height + x + 1] - heights[z * height + x], 0.0f);
			
			if (x > 0 && z > 0) sum += out.cross(left).normalize();
			if (x > 0 && z <  height - 1) sum += left.cross(in).normalize();
			if (x <  width - 1 && z <  height - 1) sum += in.cross(right).normalize();
			if (x <  width - 1 && z > 0) sum += right.cross(out).normalize();
			
			normals2[z * height + x] = sum;
		}
	}
	
	for(int z = 0; z < height; ++z) {
		for(int x = 0; x < width; ++x) {
		
			Vec3f sum = normals2[z * height + x];
			
			if (x > 0) sum += normals2[z * height + x - 1] * 0.5f;
			if (x <  width - 1) sum += normals2[z * height + x + 1] * 0.5f;
			if (z > 0) sum += normals2[(z - 1) * height + x] * 0.5f;
			if (z <  height - 1) sum += normals2[(z + 1) * height + x] * 0.5f;
			
			if (sum.magnitude() == 0) sum = Vec3f(0.0f, 1.0f, 0.0f);
			
			normals[z * height + x] = sum;
		}
	}
			
		
    texId = SOIL_load_OGL_texture
	(
	    "grass.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

void Terrain::draw() {

	glColor3f(0.3f, 0.9f, 0.0f);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	for(int i = 0; i < height - 1; ++i) {
	
		glBegin(GL_TRIANGLE_STRIP);
		
		for(int j = 0; j < width; ++j) {
		
		    Vec3f n1 = normals[i * width + j];
  		    Vec3f n2 = normals[(i + 1) * width + j];
  		    
			glTexCoord2f((float) i / height, (float) j / width);
	    	glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(j, heights[i * width + j], i);
			glTexCoord2f((float) (i + 1) / height, (float) j /width);
	    	glNormal3f(n2.x, n2.y, n2.z);
			glVertex3f(j, heights[(i + 1) * width + j], i + 1);
		}
		
		glEnd();
	}
}
