#define GLFW_DLL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "glop_camera_view.h"


unsigned int _cursor_pos_callback_counter = 0;
GLOPCameraView* camera;
GLuint program;


void circle(float x, float z)
{
    int i;
    double radius = 1.0; 
    glColor3f(0.0f, 1.0f, 0.0);
    double twicePi = 6.28318530718;
    glUniform3f(glGetUniformLocation(program, "center"), x, 0.0f, z);
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glVertex3f(x, 0.0f, z); // center of circle
    for (i = 0; i <= 40; i++)   {
        glVertex3f( (x + (radius * cos(i * twicePi / 40))), 0.0f, (z + (radius * sin(i * twicePi / 40))) );
    }
    glEnd(); //END
}


void draw()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera->Camera();
    circle(0.0f, 0.0f);
    circle(1.0f, -1.0f);
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


/*       Function will read a text file into allocated char buffer       */
char* filetobuf(const char* file)
{
    FILE *fptr;
    long length;
    char* buf;
 
    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = new char [length+1]; // malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
 
    return buf; /* Return the buffer */
}


void printShaderInfoLog(GLuint shader)
{
    int infologLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    printf("%s\n", infoLog);
    if (infologLen > 0)
    {
        infoLog = new GLchar [infologLen];
        if (infoLog == NULL)
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");
        }
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
        printf("InfoLog:\n%s\n\n", infoLog);
        delete[] infoLog;
    }
    printf("%s\n", infoLog);
}


void init_shaders()
{
    GLuint vertex_shader, fragment_shader;
    char* vertex_source;
    char* fragment_source;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    vertex_source = filetobuf("shaders/simple_vertex_shader.vert");
    fragment_source = filetobuf("shaders/simple_fragment_shader.frag");
    glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, NULL);
    glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, NULL);

    glCompileShader(vertex_shader);
    printShaderInfoLog(vertex_shader);
    glCompileShader(fragment_shader);
    printShaderInfoLog(fragment_shader);

    program = glCreateProgram();
    
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glUseProgram(program);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    set_view_port(width, height);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
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
        unsigned char pixel[3];
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        if (x < 30)                 { camera->Rotate(1);    glfwSetCursorPos(window, 30, y); }
        if (x > viewport[2] - 30)   { camera->Rotate(-1);   glfwSetCursorPos(window, viewport[2] - 30, y); }
        if (y < 20)                 { camera->Move(0.4f);   glfwSetCursorPos(window, x, 20); }
        if (y > viewport[3] - 20)   { camera->Move(-0.4f);  glfwSetCursorPos(window, x, viewport[3] - 20); }
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

    
    camera = new GLOPCameraView();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_shaders();
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
    delete camera;
    return 0;
}
