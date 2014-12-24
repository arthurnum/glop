#ifndef GUARD_CAMERA_VIEW_H
#define GUARD_CAMERA_VIEW_H
#include <GL/glew.h>
#include "glop_math.h"


class GLOPCameraView
{
public:
    GLOPCameraView();
    void Camera();
    void Move(GLfloat);
    void Rotate(GLint);
private:
    GLfloat pos_x, pos_y, pos_z, look_at_x, look_at_y, look_at_z;
    GLint angle;

    void calculateLookAt();
};

#endif
