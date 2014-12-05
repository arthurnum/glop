#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846

#include "headers/bmp_image.h"
#include "headers/glop_math.h"


bmp_image Hmap;
GLuint textureID;

GLuint triangleVBO;
GLuint normalsVOB;
GLuint texture_coordVBO;

float viewer_x, viewer_y, viewer_z;
float angleX;
float lookat_x, lookat_z;

unsigned int *indices;
vertex_t *super_vertices;


typedef struct quad_texture_uv 
{ 
    float a, b, c, d, e, f;
    float a2, b2, c2, d2, e2, f2;
} quad_texture_uv;


void build_glop_data()
{
    vertex_t *glop_vertices;
    vertex_t *glop_normals;
    vertex_t *super_normals;

    int D = Hmap.width;
    glop_vertices = malloc(D*D*sizeof(vertex_t));

    float scale = 0.25f;
    for (int h = 0; h < D; h++)
    {
        for (int w = 0; w < D; w++)
        {
            int i = h*D + w;
            glop_vertices[i].x = w * scale;
            glop_vertices[i].y = Hmap.data[((h)*Hmap.width*3+w*3)] / 255.0;
            glop_vertices[i].z = -(h * scale);
        }
    }

    glop_normals = malloc(D*D*sizeof(vertex_t));

    // bottom left normal
    vertex_t n1, n2, n3, n4, n5, n6;
    normal(glop_vertices[D+1], glop_vertices[D], glop_vertices[0], &n1);
    normal(glop_vertices[1], glop_vertices[D+1], glop_vertices[0], &n2);
    glop_normals[0].x = n1.x + n2.x;
    glop_normals[0].y = n1.y + n2.y;
    glop_normals[0].z = n1.z + n2.z;
    normalize(&glop_normals[0]);

    // bottom right normal
    normal(glop_vertices[D-1], glop_vertices[D+D-1], glop_vertices[D-2], &glop_normals[D-1]);

    // top left normal
    normal(glop_vertices[D*D-D+1], glop_vertices[D*D-D], glop_vertices[D*D-D-D], &glop_normals[D*D-D]);
    
    // top right normal
    normal(glop_vertices[D*D-1], glop_vertices[D*D-2], glop_vertices[D*D-D-2], &n1);
    normal(glop_vertices[D*D-D-1], glop_vertices[D*D-1], glop_vertices[D*D-D-2], &n2);
    glop_normals[D*D-1].x = n1.x + n2.x;
    glop_normals[D*D-1].y = n1.y + n2.y;
    glop_normals[D*D-1].z = n1.z + n2.z;
    normalize(&glop_normals[D*D-1]);

    // bottom normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D+i+1], glop_vertices[D+i], glop_vertices[i], &n1);
        normal(glop_vertices[i+1], glop_vertices[D+i+1], glop_vertices[i], &n2);
        normal(glop_vertices[i], glop_vertices[D+i], glop_vertices[i-1], &n3);
        glop_normals[i].x = n1.x + n2.x + n3.x;
        glop_normals[i].y = n1.y + n2.y + n3.y;
        glop_normals[i].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[i]);
    }

    // top normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D*D-D+i], glop_vertices[D*D-D+i-1], glop_vertices[D*D-D-D+i-1], &n1);
        normal(glop_vertices[D*D-D-D+i], glop_vertices[D*D-D+i], glop_vertices[D*D-D-D+i-1], &n2);
        normal(glop_vertices[D*D-D+i+1], glop_vertices[D*D-D+i], glop_vertices[D*D-D-D+i], &n3);
        glop_normals[D*D-D+i].x = n1.x + n2.x + n3.x;
        glop_normals[D*D-D+i].y = n1.y + n2.y + n3.y;
        glop_normals[D*D-D+i].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[D*D-D+i]);
    }

    // left normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D*i+D+1], glop_vertices[D*i+D], glop_vertices[D*i], &n1);
        normal(glop_vertices[D*i+1], glop_vertices[D*i+D+1], glop_vertices[D*i], &n2);
        normal(glop_vertices[D*i+1], glop_vertices[D*i], glop_vertices[D*i-D], &n3);
        glop_normals[D*i].x = n1.x + n2.x + n3.x;
        glop_normals[D*i].y = n1.y + n2.y + n3.y;
        glop_normals[D*i].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[D*i]);
    }

    // right normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D*i+D-1], glop_vertices[D*i+D-2], glop_vertices[D*i-2], &n1);
        normal(glop_vertices[D*i-1], glop_vertices[D*i+D-1], glop_vertices[D*i-2], &n2);
        normal(glop_vertices[D*i+D-1], glop_vertices[D*i+D+D-1], glop_vertices[D*i+D-2], &n3);
        glop_normals[D*i+D-1].x = n1.x + n2.x + n3.x;
        glop_normals[D*i+D-1].y = n1.y + n2.y + n3.y;
        glop_normals[D*i+D-1].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[D*i+D-1]);
    }

    // in-side normals
    for (int h = 1; h < D-1; h++)
    {
        for (int w = 1; w < D-1; w++)
        {
            int i = h*D + w;
            normal(glop_vertices[i+D+1], glop_vertices[i+D], glop_vertices[i], &n1);
            normal(glop_vertices[i+1], glop_vertices[i+D+1], glop_vertices[i], &n2);
            normal(glop_vertices[i], glop_vertices[i-1], glop_vertices[i-D-1], &n3);
            normal(glop_vertices[i-D], glop_vertices[i], glop_vertices[i-D-1], &n4);
            normal(glop_vertices[i], glop_vertices[i+D], glop_vertices[i-1], &n5);
            normal(glop_vertices[i+1], glop_vertices[i], glop_vertices[i-D], &n6);
            glop_normals[i].x = n1.x + n2.x + n3.x + n4.x + n5.x + n6.x;
            glop_normals[i].y = n1.y + n2.y + n3.y + n4.y + n5.y + n6.y;
            glop_normals[i].z = n1.z + n2.z + n3.z + n4.z + n5.z + n6.z;
            normalize(&glop_normals[i]);
        }
    }

    unsigned int size = (D-1)*(D-1)*6;
    super_vertices = malloc(size*sizeof(vertex_t));
    super_normals = malloc(size*sizeof(vertex_t));
    for (int h = 0; h < D-1; h++)
    {
        for (int w = 0; w < D-1; w++)
        {
            int i = h*D + w;
            int x = h*(D-1)*6 + w*6;
            super_vertices[x] = glop_vertices[i+D+1];
            super_vertices[x+1] = glop_vertices[i+D];
            super_vertices[x+2] = glop_vertices[i];
            super_vertices[x+3] = glop_vertices[i+1];
            super_vertices[x+4] = glop_vertices[i+D+1];
            super_vertices[x+5] = glop_vertices[i];
            super_normals[x] = glop_normals[i+D+1];
            super_normals[x+1] = glop_normals[i+D];
            super_normals[x+2] = glop_normals[i];
            super_normals[x+3] = glop_normals[i+1];
            super_normals[x+4] = glop_normals[i+D+1];
            super_normals[x+5] = glop_normals[i];
        }
    }

    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertex_t), super_vertices, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, NULL);


    glGenBuffers(1, &normalsVOB);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVOB);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertex_t), super_normals, GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, 0);



    quad_texture_uv *texcoords;
    texcoords = malloc((D-1)*(D-1)*sizeof(quad_texture_uv));
    float dim = 1.0f / D;
    for (int h = 0; h < D-1; h++)
    {
        for (int w = 0; w < D-1; w++)
        {
            int i = h*(D-1) + w;
            texcoords[i].a = dim*(w+1);     //x1
            texcoords[i].b = dim*(h+1);     //y1
            texcoords[i].c = dim*w;         //x2
            texcoords[i].d = dim*(h+1);     //y2
            texcoords[i].e = dim*w;         //x3
            texcoords[i].f = dim*h;         //y3
            texcoords[i].a2 = dim*(w+1);    //x4
            texcoords[i].b2 = dim*h;        //y4
            texcoords[i].c2 = dim*(w+1);    //x5
            texcoords[i].d2 = dim*(h+1);    //y5
            texcoords[i].e2 = dim*w;        //x6
            texcoords[i].f2 = dim*h;        //y6
        }
    }
    glGenBuffers(1, &texture_coordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coordVBO);
    glBufferData(GL_ARRAY_BUFFER, (D-1)*(D-1)*sizeof(quad_texture_uv), texcoords , GL_STATIC_DRAW);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);


    free(glop_vertices);
    free(glop_normals);
    // free(super_vertices);
    free(super_normals);
}


float calc_viewer_y(float x, float z)
{
    float x1, x2, x3, y1, y2, y3, z1, z2, z3;
    int i = (int)(x / 0.25f);
    int j = -(int)(z / 0.25f);

    printf("\nTILE: %d %d\n", j, i);
    printf("X: %f\n", x);

    float x0 = 0.25f - fmodf(x, 0.25f);
    float z0 = fmodf(z, 0.25f);
    int vertice_index = j*63*6 + i*6;
    if (x0 + z0 >= 0.25f)
    {
        x1 = super_vertices[vertice_index].x;
        y1 = super_vertices[vertice_index].y;
        z1 = super_vertices[vertice_index].z;

        x2 = super_vertices[vertice_index+1].x;
        y2 = super_vertices[vertice_index+1].y;
        z2 = super_vertices[vertice_index+1].z;

        x3 = super_vertices[vertice_index+2].x;
        y3 = super_vertices[vertice_index+2].y;
        z3 = super_vertices[vertice_index+2].z;
    }
    else
    {
        x1 = super_vertices[vertice_index+3].x;
        y1 = super_vertices[vertice_index+3].y;
        z1 = super_vertices[vertice_index+3].z;

        x2 = super_vertices[vertice_index+4].x;
        y2 = super_vertices[vertice_index+4].y;
        z2 = super_vertices[vertice_index+4].z;

        x3 = super_vertices[vertice_index+5].x;
        y3 = super_vertices[vertice_index+5].y;
        z3 = super_vertices[vertice_index+5].z;
    }
    float a = -(z3*y2-z1*y2-z3*y1+y1*z2+y3*z1-z2*y3);
    float b = (z1*x3+z2*x1+z3*x2-z2*x3-z1*x2-z3*x1);
    float c = (y2*x3+y1*x2+y3*x1-y1*x3-y2*x1-x2*y3);
    float d = -a*x1-b*y1-c*z1;

    return -(a*x+c*z+d)/b;
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();
    /* viewing transformation  */
    gluLookAt(viewer_x, viewer_y + 0.5f, viewer_z, lookat_x, -15.0f, lookat_z, 0.0f, 1.0f, 0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVOB);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coordVBO);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 63*63*6);
    glDisable(GL_TEXTURE_2D);


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();
    glRasterPos2i(50, 50);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, "GLOP@");

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glutSwapBuffers();
}


void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();             /* clear the matrix */
}


void escapeApp(int key, int x, int y)
{
    float aspect_x = sin(angleX / 180.0 * PI) * 0.01f;
    float aspect_z = -cos(angleX / 180.0 * PI) * 0.01f;
    float degree_aspect = 2.0f;
    if (key == GLUT_KEY_END) { glutLeaveMainLoop(); }
    if (key == GLUT_KEY_UP)
    {
        if (angleX < 90) { viewer_z += aspect_z; viewer_x += aspect_x; }
        else if (angleX < 180) { viewer_z += aspect_z; viewer_x += aspect_x; }
        else if (angleX < 270) { viewer_z += aspect_z; viewer_x += aspect_x; }
        else if (angleX < 360) { viewer_z += aspect_z; viewer_x += aspect_x; }
        viewer_y = calc_viewer_y(viewer_x, viewer_z);
        printf("%f\n", viewer_y);
    }
    if (key == GLUT_KEY_DOWN)
    {
        if (angleX < 90) { viewer_z -= aspect_z; viewer_x -= aspect_x; }
        else if (angleX < 180) { viewer_z -= aspect_z; viewer_x -= aspect_x; }
        else if (angleX < 270) { viewer_z -= aspect_z; viewer_x -= aspect_x; }
        else if (angleX < 360) { viewer_z -= aspect_z; viewer_x -= aspect_x; }
        viewer_y = calc_viewer_y(viewer_x, viewer_z);
        printf("%f\n", viewer_y);
    }
    if (key == GLUT_KEY_RIGHT)
    {
        angleX += degree_aspect;
        if (angleX >= 360.0) { angleX = 0.0; }
        lookat_x = sin(angleX / 180.0 * PI) * 100;
        lookat_z = cos(angleX / 180.0 * PI) * (-100);
    }
    if (key == GLUT_KEY_LEFT)
    {
        angleX -= degree_aspect;
        if (angleX < 0.0) { angleX = 360.0 - degree_aspect; }
        lookat_x = sin(angleX / 180.0 * PI) * 100;
        lookat_z = cos(angleX / 180.0 * PI) * (-100);
    }

    glutPostRedisplay();
}


void wheelHand(int wheel, int direction, int x, int y)
{
    // put some logic here
}


void mouseHand(int button, int state, int x, int y)
{
    GLdouble posX, posY, posZ;
    if (state == GLUT_DOWN)
    {
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
     
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetIntegerv( GL_VIEWPORT, viewport );
     
        winX = (float)x;
        winY = (float)viewport[3] - (float)y;
        glReadPixels( x, (int)(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
     
        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
        printf("%f %f %f\n", posX, posY, posZ);
    }

    glutPostRedisplay();
}


void load_texture()
{
    glGenTextures(1, &textureID);
     
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    bmp_image texture = load_bmp_image("terrain.bmp");
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
}


int main (int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitContextVersion(3, 0);

    glutInitWindowSize(800, 600);
    glutCreateWindow("YO!");


    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    glutSpecialFunc(escapeApp);
    glutMouseWheelFunc(wheelHand);


    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    Hmap = load_bmp_image("map.bmp");
    load_texture();
    build_glop_data();

    glClearColor (0.3, 0.3, 0.5, 0.0);

    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glutMouseFunc(mouseHand);

    viewer_x = 3.0f;
    viewer_y = 1.0f;
    viewer_z = -3.0f;
    angleX = 0.0f;
    lookat_x = sin(angleX / 180.0 * PI) * 20;
    lookat_z = cos(angleX / 180.0 * PI) * (-20);

    glutMainLoop();

    return 0;
}
