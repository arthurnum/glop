#include "sphere.h"

GLOPSphere::GLOPSphere()
{
    object = gluNewQuadric();
    selected = 0;
}


void GLOPSphere::Draw()
{
    gluSphere(object, 0.5, 32, 32);
}
