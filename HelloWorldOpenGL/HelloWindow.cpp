#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/******************************************************************************************************************************* 
Shaders written in GLSL
*******************************************************************************************************************************/

/* Vertex shader */
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

/* Fragment shader */
/* For color! */
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // Represented in RGBA
"}\n\0";

/*******************************************************************************************************************************
End shaders written in GLSL
*******************************************************************************************************************************/

int main()
{

    /*******************************************************************************************************************************
    Window setup
    *******************************************************************************************************************************/
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
    /*******************************************************************************************************************************
    End window setup
    *******************************************************************************************************************************/



    /*******************************************************************************************************************************
    Shader operations
    *******************************************************************************************************************************/
    /* In order for OpenGL to use the shader it has to dynamically compile it at run-time from its source code */
    unsigned int vertexShader; // vertexShader will be stored as an unsigned int
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // We provide the type of shader we want to create as an argument.

    /* Attach the shader source to the shader object. */
    /* Parameters:
       1: Shader object to compile
       2: Number of strings we're passing as source code
       3: Source code
       4: (Tutorial just says to leave NULL */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 

    /* Compile vertex shader written in GLSL */
    glCompileShader(vertexShader); 

    /* Check for successful compilation */
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* Link shaders */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); // Returns ID reference
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* Check for linking errors */
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    /* Linking results in a program object we can call like so: 
    glUseProgram(shaderProgram); 
    Every shader and rendering call after glUseProgram will use this program (and, by extension, its shaders) */

    /* Delete shader objects as we no longer need them */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    /*******************************************************************************************************************************
    End shader operations
    *******************************************************************************************************************************/



    /*******************************************************************************************************************************
    Buffer operations
    *******************************************************************************************************************************/
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

    /* Each vertex attribute takes its data from memory that is managed by a VBO. The VBO that it takes its data from is 
       determined by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer. 
       Since the previously defined VBO is still bound, 
       calling glVertexAttribPointer below will result in vertex attribute 0 associated with said VBO's vertex data */

    /* Tell OpenGL how to interpret vertex data per vertex attribute
    Parameters are as follows:
    1: Which vertex attribute we want to configure. 
       We specified the location of the position vertex attribute in the vertex shader code with layout (location = 0)
       This sets the location of the vertex attribute to 0. Since we want to pass data to this vertex attribute, we pass 0.
    2: Size of the vertex attribute. In this case, it's a vec3. 3 values.
    3: Type of data. vecs in GLSL consist of floating point values.
    4: Specifies if we want the data to be normalized. This is not relevant to us. We leave it GL_FALSE
    5: Stride. This is the space between consecutive vertex attributes(See VertexBufferData.png).
    6: Offset of where the position data begins in the buffer. For now, the position data is at the start of the data array. */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



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
    /*******************************************************************************************************************************
    End buffer operations
    *******************************************************************************************************************************/




    /*******************************************************************************************************************************
    Render loop
    *******************************************************************************************************************************/
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
    /*******************************************************************************************************************************
    End render loop
    *******************************************************************************************************************************/



    glfwTerminate();
    return 0;
}


/* Resize callback function */
/* Whenever the window changes in size, GLFW calls this functionand fills in the proper arguments */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /* Set dimensions of rendering window */
    /* OpenGL uses the data specified via glViewport to transform the 2d coordinates it processed to coordinates on the screen
       A point of locations (-0.5, 0.5) would be mapped to (200, 450) in screen coordinates.
       Processed coordinates in OpenGL are between -1 and 1. We effectively map from (-1, 1) to (0, 800) and (0, 600) */
    glViewport(0, 0, width, height); // this IS different from the dimensions in CreateWindow()
}




void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}