#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); /* 创建对应类型的着色器 */
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); /* 设置着色器源码 */
    glCompileShader(id); /* 编译着色器 */

    /* 编译错误处理 */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // 获取当前着色器编译状态
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // 获取日志长度
        char* msg = (char*)_malloca(length * sizeof(char)); /* Cherno这里采用的alloca, 根据IDE提示, 我这里改成了_malloca函数 */
        glGetShaderInfoLog(id, length, &length, msg); // 获取日志信息
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex":"fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;
        glDeleteShader(id); // 删除着色器
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); /* 创建程序 */
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    /* 将着色器附加到程序上 */
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program); /* 链接程序 */
    glValidateProgram(program); /* 验证 */

    /* 删除着色器 */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Status: Using GL " << glGetString(GL_VERSION) << std::endl;

    /* 顶点位置浮点型数组 */
    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); /* 生成缓冲区 */
    glBindBuffer(GL_ARRAY_BUFFER, buffer); /* 绑定缓冲区 */
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); /* 设置缓冲区数据 */
    
    glEnableVertexAttribArray(0); /* 激活顶点属性-索引0-位置 */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); /* 设置顶点属性-索引0 */

    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0)in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        " gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0)out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        " color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader); /* 使用着色器程序 */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader); // 删除着色器程序

    glfwTerminate();
    return 0;
}