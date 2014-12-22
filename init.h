#ifndef GUARD_INIT_H
#define GUARD_INIT_H
#include <GL/glew.h>


static GLuint triangleVBO;
static GLuint normalsVOB;
static GLuint texture_coordVBO;

typedef struct quad_texture_uv 
{ 
    float a, b, c, d, e, f;
    float a2, b2, c2, d2, e2, f2;
} quad_texture_uv;


typedef struct bmp_image
{
    unsigned int width, height;
    unsigned char *data;
} bmp_image;

void build_glop_data();
bmp_image load_bmp_image(const char*);

#endif
