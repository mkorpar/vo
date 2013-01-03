#include <cstdio>

#include "Vec.hpp"

extern float triangleSegmentIntersection(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f p1, Vec3f p2) {

    // Find triangle normal
    Vec3f n = (v2 - v1).cross(v3 - v1);

    // Find distance from LP1 and LP2 to the plane defined by the triangle
    float d1 = (p1 - v1).dot(n);
    float d2 = (p2 - v1).dot(n);

    // line doesn't cross the triangle.
    if ((d1 * d2) >= 0.0f) { 
        return NO_INTERSECTION; 
    } 

    // line and plane are parallel
    if (d1 == d2) { 
        return NO_INTERSECTION; 
    } 

    // Find point on the line that intersects with the plane
    Vec3f x = p1 + (p2 - p1) * (-d1 / (d2 - d1));

    // Find if the interesection point lies inside the triangle by testing it against all edges
    if (n.cross(v2 - v1).dot(x - v1) < 0.0f) { 
        return NO_INTERSECTION; 
    }

    if (n.cross(v3 - v2).dot(x - v2) < 0.0f) { 
        return NO_INTERSECTION; 
    }

    if (n.cross(v1- v3).dot(x - v1) < 0.0f) { 
        return NO_INTERSECTION; 
    }

    // printf("( %f, %f, %f )\n", x.x, x.y, x.z);

    return (p1 - x).magnitude();
}
