#ifndef GUARD_GLOP_MATH_H
#define GUARD_GLOP_MATH_H
#include <math.h>
#define PI 3.14159265358979323846
// This is how a vertex is specified in the base code
typedef struct vertex_t { float x, y, z; } vertex_t;

// This is how a vector is specified in the base code
// The origin is assumed to be [0,0,0]
void normalize (vertex_t*);


// normal(); - finds a normal vector and normalizes it
void normal (vertex_t, vertex_t, vertex_t, vertex_t*);

// degrees to radians
double dtor(int);

#endif
