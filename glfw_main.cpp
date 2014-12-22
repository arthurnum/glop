#define GLFW_DLL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "init.h"
#include "sphere.h"

GLuint textureID;
GLOPSphere* sphere1;
GLOPSphere* sphere2;

unsigned int _cursor_pos_callback_counter = 0;


void draw()
{
    glEnable(GL_LIGHTING);
    glClearColor(0.6, 0.6, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2.0f, 2.0f, -5.0f, 10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVOB);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coordVBO);

    float colorAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorAmbient);
    float colorDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuse);
    float colorSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorSpecular);
    glMateriali(GL_FRONT, GL_SHININESS, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 127*127*6);
    glDisable(GL_TEXTURE_2D);

    if (sphere1->selected)
    {
        float colorAmbientGold[] = { 0.24f, 0.19f, 0.07f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorAmbientGold);
        float colorDiffuseGold[] = { 0.75f, 0.60f, 0.22f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseGold);
        float colorSpecularGold[] = { 0.62f, 0.55f, 0.36f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorSpecularGold);
        glMaterialf(GL_FRONT, GL_SHININESS, 0.4f * 128.0f);
    }
    glTranslatef(5.0f, 1.0f, -5.0f);
    sphere1->Draw();
    glTranslatef(2.0f, 0.0f, -2.0f);
    sphere2->Draw();
}


void picking_draw()
{
    glDisable(GL_LIGHTING);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2.0f, 2.0f, -5.0f, 10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f);

    glColor3ub(10, 10, 10);
    glTranslatef(5.0f, 1.0f, -5.0f);
    sphere1->Draw();
    glColor3ub(20, 20, 20);
    glTranslatef(2.0f, 0.0f, -2.0f);
    sphere2->Draw();
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


void set_view_port(int w, int h)
{
    glViewport(0, 0, w, h);
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(55.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             /* clear the matrix */
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    set_view_port(width, height);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        picking_draw();
        unsigned char pixel[3];

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        if (pixel[0] == 10 && pixel[1] == 10 && pixel[2] == 10) { printf("%s\n", "this is spere1"); }
        if (pixel[0] == 20 && pixel[1] == 20 && pixel[2] == 20) { printf("%s\n", "this is spere2"); }
    }
}


void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    if (_cursor_pos_callback_counter % 4 == 0)
	{
        picking_draw();
		unsigned char pixel[3];
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		sphere1->selected = 0;
		if (pixel[0] == 10 && pixel[1] == 10 && pixel[2] == 10) { sphere1->selected = 1; }
		if (pixel[0] == 20 && pixel[1] == 20 && pixel[2] == 20) { sphere2->selected = 1; }
	}
	_cursor_pos_callback_counter++;
	if (_cursor_pos_callback_counter > 65000) _cursor_pos_callback_counter = 0;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 2);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    GLenum err = glewInit();
    if (GLEW_OK != err) { printf("Error: %s\n", glewGetErrorString(err)); }
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("%s\n", glGetString(GL_VERSION));
    set_view_port(640, 480);
    load_texture();
    build_glop_data();

    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    sphere1 = new GLOPSphere();
    sphere2 = new GLOPSphere();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        draw();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Wait for and process events */
		glfwWaitEvents();
    }

    glfwTerminate();
    delete sphere1;
    delete sphere2;
    return 0;
}
