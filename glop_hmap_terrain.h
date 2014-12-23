#ifndef GUARD_GLOP_HMAP_TERRAIN_H
#define GUARD_GLOP_HMAP_TERRAIN_H
#include <GL/glew.h>
#include <stdio.h>

#include "glop_math.h"

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


class GLOPHmapTerrain
{
public:
    void Build(const char*);
    void Draw();
private:
    GLuint triangleVBO;
    GLuint normalsVOB;
    GLuint texture_coordVBO;
    GLuint textureID;

    void load_texture();
    bmp_image load_bmp_image(const char*);
};

#endif
