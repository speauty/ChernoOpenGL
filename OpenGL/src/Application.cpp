#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /**
     * 交换间隔，交换缓冲区之前等待的帧数，通常称为v-sync
     * 默认情况下，交换间隔为0
     * 这里设置为1，即每帧更新一次
     **/
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    
    unsigned char* glVersion;
    GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
    std::cout << "Status: Using GL " << glVersion << std::endl;

    {
        /* 顶点位置浮点型数组 */
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
             50.0f, -50.0f, 1.0f, 0.0f, // 1
             50.0f, 50.0f, 1.0f, 1.0f,  // 2
            -50.0f, 50.0f, 0.0f, 1.0f   // 3
        };

        /* 索引缓冲区所需索引数组 */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        /**
         * 混合: 
         * 将输出颜色(判断着色器输出的颜色)和目标缓冲区已有的颜色结合
         * glEnable/glDisable(启用&关闭) => glBlendFunc(指定颜色因子) => glBlendEquation(指定混合模式)
         * glBlendEquation(mode) mode: src和dest的混合方式(默认GL_FUNC_ADD, 叠加)
         * 
         **/
         /* 启用混合(默认不会启用) */
        GLCall(glEnable(GL_BLEND));
        /**
         * glBlendFunc(src, dest) 指定颜色因子
         * src 指定输出颜色(RGBA)因子的计算方式, 默认为GL_ONE
         * dest 指定目标颜色因子的计算方式, 默认为GL_ZERO
         * GL_SRC_ALPHA 因为src的alpha为0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
         * RGBA = Srgba * GL_SRC_ALPHA + Drgba * GL_ONE_MINUS_SRC_ALPHA
         **/
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        /* glm::ortho 正交矩阵 */
        /* 这里应该是 960x720 而不是 960x540 的分辨率 */
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f);
        /* 相机位置 视图矩阵 x&y&z */
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        /* 解绑 */
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();

        // 需要指定glsl版本, 也就是shader中的version
        const char* glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);

        float r = 0.0f;
        float increment = 0.05f;

        glm::vec3 tranlationA(200, 200, 0);
        glm::vec3 tranlationB(400, 200, 0);
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            renderer.Clear();
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), tranlationA);
                glm::mat4 mvp = proj * view * model;

                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), tranlationB);
                glm::mat4 mvp = proj * view * model;

                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            if (r > 1.0f) {
                increment = -0.05f;
            }
            else if (r < 0.0f) {
                increment = 0.05f;
            }
            r += increment;

            {
                ImGui::Begin("ImGui");
                ImGui::SliderFloat3("TranlationA", &tranlationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranlationB", &tranlationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}