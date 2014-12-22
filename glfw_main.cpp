#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "init.h"

GLuint textureID;

void draw()
{
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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 127*127*6);
    glDisable(GL_TEXTURE_2D);
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


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 1);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        draw();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
