#define GLFW_DLL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "glop_base_obj.h"
#include "glop_hmap_terrain.h"


GLOPBaseObj* baseObj;
GLOPBaseObj* baseObj2;
GLOPBaseObj* baseObj3;
GLOPHmapTerrain* terrain;

unsigned int _cursor_pos_callback_counter = 0;


void draw()
{
    glEnable(GL_LIGHTING);
    glClearColor(0.6, 0.6, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-0.5f, 0.5f, 0.5f, 10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    terrain->Draw();

	glPushMatrix();
    glTranslatef(1.0f, 0.0f, -1.0f);
	glScalef(0.1f, 0.1f, 0.1f);
    baseObj->Draw();
	glPopMatrix();

	glPushMatrix();
    glTranslatef(2.5f, 0.0f, -3.5f);
	glRotatef(-60.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);
    baseObj2->Draw();
	glPopMatrix();

	glPushMatrix();
    glTranslatef(0.8f, 0.0f, -1.4f);
	glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.05f, 0.05f, 0.05f);
    baseObj3->Draw();
	glPopMatrix();
}


void picking_draw()
{
    glDisable(GL_LIGHTING);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-0.5f, 0.5f, 0.5f, 10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
    glTranslatef(1.0f, 0.0f, -1.0f);
	glScalef(0.1f, 0.1f, 0.1f);
    baseObj->DrawPick();
	glPopMatrix();

	glPushMatrix();
    glTranslatef(2.5f, 0.0f, -3.5f);
	glRotatef(-60.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);
    baseObj2->DrawPick();
	glPopMatrix();

	glPushMatrix();
    glTranslatef(0.8f, 0.0f, -1.4f);
	glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.05f, 0.05f, 0.05f);
    baseObj3->DrawPick();
	glPopMatrix();
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
		baseObj->MouseOver(pixel);
		baseObj2->MouseOver(pixel);
		baseObj3->MouseOver(pixel);
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

    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
	glEnable(GL_NORMALIZE);

    terrain = new GLOPHmapTerrain();
    terrain->Build("map128.bmp");
    baseObj = new GLOPBaseObj();
    baseObj->Load("wolf_mesh.obj");
    baseObj2 = new GLOPBaseObj();
    baseObj2->Load("wolf_mesh.obj");
    baseObj3 = new GLOPBaseObj();
    baseObj3->Load("wolf_mesh.obj");
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
    delete baseObj;
    delete baseObj2;
    delete baseObj3;
    return 0;
}
