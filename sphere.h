#ifndef GUARD_SPHERE_H
#define GUARD_SPHERE_H
#include <GL/glew.h>


class GLOPSphere
{
public:
    GLOPSphere();
    void Draw();
    char selected;

private:
    GLUquadricObj* object;
};

#endif
