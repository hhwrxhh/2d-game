#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Physics/PhysicsEngine.h"

#include "Renderer/Renderer.h"

static constexpr unsigned int SCALE = 2;
static constexpr unsigned int BLOCK_SIZE = 16;
glm::uvec2 g_WinSize(SCALE * 16 * BLOCK_SIZE, SCALE * 15 * BLOCK_SIZE); // width , height  
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_WinSize);


void glfwWinSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_WinSize.x = width;
    g_WinSize.y = height;
    g_game->setWindowSize(g_WinSize);
    
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scanode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    
    g_game->setKey(key, action);
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
	
    std::cout << "Renderer " << RenderEngine::Renderer::getRendererStr() << std::endl;
	std::cout << "OpenGL Version " << RenderEngine::Renderer::getVersionStr() << std::endl;
	std::cout << "Version of ... " << glGetString(GL_VENDOR) << std::endl;
	
    RenderEngine::Renderer::setClearColor(0.f, 0.f, 0.f, 1);
    RenderEngine::Renderer::setDepthTest(true);
    
    {
        ResourceManager::setExecutablePath(argv[0]);
        Physics::PhysicsEngine::init();
        g_game->init();

        //glfwSetWindowSize(pWindow, static_cast<int>(2 * g_game->getCurrentLevelWidth()), static_cast<int>(2 * g_game->getCurrentLevelHeight()));
        
        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
            lastTime = currentTime;

            g_game->update(duration);
            Physics::PhysicsEngine::update(duration);

            /* Render here */
            RenderEngine::Renderer::clear();

            g_game->render();
            
            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        Physics::PhysicsEngine::terminate();
        g_game = nullptr;
        ResourceManager::unloadAllResources();
    }
    glfwTerminate();
    return 0;
}