#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/* Shader written in GLSL */
const char* vertexShaderSource = 
"#version 330 core\n" // Version declaration

/* Declare all input vertex attributes in vertex shader with "in" keyword */
/* Right now all we care about position, so we only need a single vertex attribute */
"layout (location = 0) in vec3 aPos;\n" 

"void main()\n"
"{\n"
/* To set the output of the vertex shader we have to assign the position data to the predefined gl_Position variable*/
/* gl_Position is a vec4 behind the scenes. As such, we have to cast our input(vec3) to vec4*/
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" 
"}\0";

int main()
{
    glfwInit();

    /* Set version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Use the core profile */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* For Mac OS X */
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create window*/
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Handle resize

    /* GLAD manages function pointers for OpenGL. We want to call it before we call any OpenGL functions */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // We pass GLAD the function to load the address of the OpenGL function pointers (OS specific)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Triangle coordinates */
    /* These NDCs(Normalized Device Coordinates) will be transformed 
       to screen-space coordinates via the viewport transform using the data we provided with glViewport. 
       The resulting screen-space coordinates are then transformed to fragments as inputs to our fragment shader. */
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    /* Vertex Buffer Object*/
    unsigned int VBO;
    glGenBuffers(1, &VBO); // Generate 1 VBO and store buffer reference ID in &VBO

    // OpenGL allows us to bind several buffers at once as long as they have a different buffer type.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the newly created VBO to the GL_ARRAY_BUFFER
    // Any buffer calls we make on the GL_ARRAY_BUFFER target will be used to configure the currently bound buffer (VBO)

    /* Copies the previously defined vertex data into the buffer's memory */
    /* This function is specifically targeted to copy user-defined data into the currently bound buffer */
    /* ARGUMENTS: 
       1: Type of buffer we want to copy data into. In this case it's the VBO currently bound to the GL_ARRAY_BUFFER target. 
       2: The size of the data.\
       3: Actual data we want to send
       4: How we want the graphics card to manage the data. There are 3 ways:
            a: GL_STREAM_DRAW: data is set once and used by the GPU at most few times.
            b: GL_STATIC_DRAW: data is set once and used many times.
            c: GL_DYNAMIC_DRAW: data is changed and used many times */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Our triangle does not move, is used a lot, and stays the same.

    /* Set dimensions of rendering window */
    /* OpenGL uses the data specified via glViewport to transform the 2d coordinates it processed to coordinates on the screen 
       A point of locations (-0.5, 0.5) would be mapped to (200, 450) in screen coordinates.
       Processed coordinates in OpenGL are between -1 and 1. We effectively map from (-1, 1) to (0, 800) and (0, 600) */
    glViewport(0, 0, 800, 600); // this IS different from the dimensions in CreateWindow()

    /* Render loop */
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set color to clear the screen with
        glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer and and fill with color specified in glClearColor

        glfwSwapBuffers(window); // Double buffered. Avoid flickering issues common to single buffer
        glfwPollEvents(); // Check for mouse/keyboard input etc.
    }

    // Render loop exited
    glfwTerminate();
    return 0;
}


/* Resize callback function */
/* Whenever the window changes in size, GLFW calls this functionand fills in the proper arguments */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}