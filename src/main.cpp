#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#ifdef _WIN32
// Math functions
#define _USE_MATH_DEFINES
#endif
#include <math.h>

bool InitializeRenderer();
void Update(float dt);
void Render(float dt);

int gWidth, gHeight;

// http://www.glfw.org/docs/latest/input_guide.html
void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);
void MouseMoveCallback(GLFWwindow* Window, double X, double Y);
//
void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        fprintf(stderr, "glfwInit() failed.\n");
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello, World", nullptr, nullptr);
    if (!window)
    {
        fprintf(stderr, "glfwCreateWindow() failed.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &gWidth, &gHeight);

    if (!gladLoadGL())
    {
        fprintf(stderr, "gladLoadGL() failed.\n");
        return -1;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, MouseMoveCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!InitializeRenderer())
    {
        fprintf(stderr, "InitRenderer() failed.\n");
        return -1;
    }

    double t0 = glfwGetTime();
    double t1;
    float dt;
    while (!glfwWindowShouldClose(window))
    {
        t1 = glfwGetTime();
        dt = (float)(t1 - t0);
        t0 = t1;

        glfwPollEvents();
        Update(dt);
        Render(dt);

        glfwSwapBuffers(window);
    }

    return 0;
}

static GLuint gTriangleVBO;
static GLuint gTriangleVAO;

static GLuint gShaderProgram;

static const char* gVertexShaderSrc = R"glsl(
#version 450 core
layout (location = 0) in vec3 in_Position;
void main()
{
    gl_Position = vec4(in_Position, 1.0);
}
)glsl";

static const char* gFragmentShaderSrc = R"glsl(
#version 450 core
layout (location = 0) out vec4 out_Color;
void main()
{
    out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
)glsl";

bool CheckShader(GLuint pShader)
{
    GLint CompileStatus;

    GLchar InfoLog[512];
    glGetShaderiv(pShader, GL_COMPILE_STATUS, &CompileStatus);

    if (!CompileStatus)
    {
        glGetShaderInfoLog(pShader, 512, nullptr, InfoLog);
        fprintf(stderr, "Shader compilation error : %s\n", InfoLog);
    }

    return (bool)CompileStatus;
}

bool CheckProgram(GLuint pProgram)
{
    GLint LinkStatus;

    GLchar InfoLog[512];
    glGetProgramiv(pProgram, GL_LINK_STATUS, &LinkStatus);

    if (!LinkStatus)
    {
        glGetProgramInfoLog(pProgram, 512, nullptr, InfoLog);
        fprintf(stderr, "Program link error : %s\n", InfoLog);
    }

    return (bool)LinkStatus;
}

bool InitializeRenderer()
{
    bool result = true;

    // Setup buffers
    glGenVertexArrays(1, &gTriangleVAO);
    glBindVertexArray(gTriangleVAO);

    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    glGenBuffers(1, &gTriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Compile shaders
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &gVertexShaderSrc, nullptr);
    glCompileShader(vertexShader);
    if (!CheckShader(vertexShader))
    {
        result = false;
        return result;
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &gFragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);
    if (!CheckShader(fragmentShader))
    {
        result = false;
        return result;
    }

    // Link shaders
    gShaderProgram = glCreateProgram();
    glAttachShader(gShaderProgram, vertexShader);
    glAttachShader(gShaderProgram, fragmentShader);
    glLinkProgram(gShaderProgram);
    if (!CheckProgram(gShaderProgram))
    {
        result = false;
        return result;
    }

    glDetachShader(gShaderProgram, vertexShader);
    glDeleteShader(vertexShader);

    glDetachShader(gShaderProgram, fragmentShader);
    glDeleteShader(fragmentShader);

    return result;
}

void Update(float pDeltaTime)
{
}

void Render(float pDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Set viewport
    glViewport(0, 0, gWidth, gHeight);

    // Use program
    glUseProgram(gShaderProgram);

    // Render triangle
    glBindVertexArray(gTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void KeyCallback(GLFWwindow* pWindow, int Key, int Scancode, int Action, int Mods)
{
    if (Key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
}

void MouseButtonCallback(GLFWwindow* pWindow, int pButton, int pAction, int pMods)
{
}

void MouseMoveCallback(GLFWwindow* pWindow, double pX, double pY)
{
}

void FramebufferSizeCallback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
    gWidth = pWidth;
    gHeight = pHeight;
}
