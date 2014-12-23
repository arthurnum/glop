#ifndef GUARD_GLOP_BASE_OBJ_H
#define GUARD_GLOP_BASE_OBJ_H
#include <fstream>
#include <stdio.h>
#include <string>
#include <GL/glew.h>

typedef struct vertex_v { float x, y, z; } vertex_v;


class GLOPBaseObj
{
public:
    void Load(const char*);
    void Draw();
private:
    GLuint verticesVBO;
    GLuint normalsVBO;
    unsigned int faces;
};


#endif
