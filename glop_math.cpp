#include "glop_math.h"

void normalize (vertex_t *v)
{
    // calculate the length of the vector
    float len = (float)(sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));
    // avoid division by 0
    if (len == 0.0f) len = 1.0f;
    // reduce to unit size
    v->x /= len;
    v->y /= len;
    v->z /= len;
}


// normal(); - finds a normal vector and normalizes it
void normal (vertex_t v1, vertex_t v2, vertex_t v3, vertex_t *normal)
{
    vertex_t a, b;
    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    a.x = v1.x - v2.x;
    a.y = v1.y - v2.y;
    a.z = v1.z - v2.z;
    // b
    b.x = v2.x - v3.x;
    b.y = v2.y - v3.y;
    b.z = v2.z - v3.z;
    // calculate the cross product and place the resulting vector
    // into the address specified by vertex_t *normal
    normal->x = (a.y * b.z) - (a.z * b.y);
    normal->y = (a.z * b.x) - (a.x * b.z);
    normal->z = (a.x * b.y) - (a.y * b.x);
    // normalize
    normalize(normal);
}
