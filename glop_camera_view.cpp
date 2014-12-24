#include "glop_camera_view.h"

GLOPCameraView::GLOPCameraView()
{
    pos_x = 0.0f;
    pos_y = 1.0f;
    pos_z = 0.0f;
    look_at_y = 0.0f;
    angle = 0;
    calculateLookAt();
}


void GLOPCameraView::Camera()
{
    gluLookAt(pos_x, pos_y, pos_z, look_at_x, look_at_y, look_at_z, 0.0f, 1.0f, 0.0f);
}


void GLOPCameraView::calculateLookAt()
{
    float cosv = (float)(cos(dtor(angle)));
    float sinv = (float)(sin(dtor(angle)));
    look_at_x = pos_x + 5.0f * cosv;
    look_at_z = pos_z - 5.0f * sinv;
}


void GLOPCameraView::Move(GLfloat aspect)
{
    float cosv = (float)(cos(dtor(angle)));
    float sinv = (float)(sin(dtor(angle)));
    pos_x += aspect * cosv;
    pos_z -= aspect * sinv;
    calculateLookAt();
}


void GLOPCameraView::Rotate(GLint degrees)
{
    angle += degrees;
    if (angle < 0) { angle = 360 + degrees; }
    if (angle > 360) { angle = degrees; }
    calculateLookAt();
}
