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
	GLOPBaseObj();
	void MouseOver(unsigned char*);
    void Load(const char*);
    void Draw();
    void DrawPick();
private:
    GLuint verticesVBO;
    GLuint normalsVBO;
    unsigned int faces;
    char selected;
	unsigned char id;
	static unsigned char _Id;
};

#endif
