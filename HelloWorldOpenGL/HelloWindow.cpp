#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

    /* Set dimensions of rendering window */
    /* OpenGL uses the data specified via glViewport to transform the 2d coordinates it processed to coordinates on the screen 
       A point of locations (-0.5, 0.5) would be mapped to (200, 450) in screen coordinates.
       Processed coordinates in OpenGL are between -1 and 1. We effectively map from (-1, 1) to (0, 800) and (0, 600) 
    */
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