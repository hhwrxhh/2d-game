#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"

glm::ivec2 g_WinSize(640, 480);
Game g_game(g_WinSize);


void glfwWinSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_WinSize.x = width;
    g_WinSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scanode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    
    g_game.setKey(key, action);
}

int main(int argc, char** argv)
{
    GLFWwindow* pWindow;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Can not  glfwInit" << std::endl;
        return -1;
    }

    // set up the opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    /* Create a windowed mode window and its OpenGL context */
    pWindow = glfwCreateWindow(g_WinSize.x, g_WinSize.y, "2d game", nullptr, nullptr);

    if (!pWindow)
    {
        std::cout << "Can not glfwCreateWindow" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWinSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);
	
	if (!gladLoadGL())
	{
		std::cout << "Can not load glad" << std::endl;
		return -1;
	}
	
    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Version of ... " << glGetString(GL_VENDOR) << std::endl;
	
	glClearColor(0.f, 0.f, 0.f, 1);
    
    {
        ResourceManager::setExecutablePath(argv[0]);
        g_game.init();
        
        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            g_game.update(duration);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            g_game.render();
            
            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        ResourceManager::unloadAllResources();
    }
    glfwTerminate();
    return 0;
}