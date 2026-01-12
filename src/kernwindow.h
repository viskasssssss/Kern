#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "config.h"
#include "utils/colors.h"
#include "utils/vectors.h"
#include "backends/OpenGL/openglrenderer.h"

namespace kern
{
    enum class GraphicsAPI
    {
        OpenGL,
        Vulkan,
        DirectX12
    };

    class Window
    {
    public:
        Window(int width, int height, std::string header, GraphicsAPI graphics)
            : window(nullptr), renderer(nullptr), graphics(graphics)
        {
            cast("Initializing Window");
            if (!glfwInit())
            {
                cast("GLFW init failed", DebugLevel::Error);
                return;
            }

            if (graphics == GraphicsAPI::OpenGL)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            }

            this->window = glfwCreateWindow(width, height, header.c_str(), nullptr, nullptr);
            if (!window)
            {
                cast("Window creation failed", DebugLevel::Error);
                glfwTerminate();
                return;
            }

            glfwMakeContextCurrent(window);

            // Creating renderer
            if (graphics == GraphicsAPI::OpenGL)
            {
                if (!gladLoadGL())
                {
                    cast("Failed to initialize OpenGL context with GLAD", DebugLevel::Error);
                    glfwDestroyWindow(window);
                    glfwTerminate();
                    return;
                }
                renderer = new OpenGLRenderer(window, width, height);
            }
            // Other APIs later
            // else if (graphics == GraphicsAPI::Vulkan) { ... }

            previousTime = glfwGetTime();

            cast("Window and Renderer initialized");
        }

        ~Window()
        {
            delete renderer;
            if (window)
            {
                glfwDestroyWindow(window);
                glfwTerminate();
            }
        }

        void clear()
        {
            if (isOpen() && renderer) {
                renderer->clear();
            }
            glfwPollEvents();

            double currentTime = glfwGetTime();
            frameCount++;
            // If a second has passed.
            if ( currentTime - previousTime >= 1.0 )
            {
                // Display the frame count here any way you want.
                FPS = frameCount;
                frameCount = 0;
                previousTime = currentTime;
            }
        }

        void present()
        {
            if (isOpen() && renderer) {
                renderer->present();
            }
        }

        void clearColor(float r, float g, float b, float a = 1.0f)
        {
            if (renderer)
            {
                renderer->setClearColor(r, g, b, a);
            }
        }

        void tri(Vector2 a, Vector2 b, Vector2 c, Color color)
        {
            if (renderer)
            {
                renderer->renderTri(a, b, c, color);
            }
        }

        template<typename Vertex>
        void draw(const std::vector<Vertex>& verts, const kern::OpenGLShaderProgram& shader)
        {
            if (renderer && graphics == GraphicsAPI::OpenGL) {
                static_cast<OpenGLRenderer*>(renderer)->draw(verts, shader);
            }
        }

        void line(Vector2 a, Vector2 b, Color color, float thickness = 1.0f)
        {
            if (renderer)
            {
                renderer->renderLine(a, b, color, thickness);
            }
        }

        void setTitle(std::string title)
        {
            if (window)
            {
                glfwSetWindowTitle(window, title.c_str());
            }
        }

        void setSize(int width, int height)
        {
            if (window)
            {
                glfwSetWindowSize(window, width, height);
            }
        }

        void circle(Vector2 center, float radius, Color color)
        {
            if (renderer)
            {
                renderer->renderCircle(center, radius, color);
            }
        }

        int getFPS() const
        {
            if (isOpen() && window)
            {
                return FPS;
            }
            return 0;
        }

        float getDeltaTime() const
        {
            if (isOpen() && window)
            {
                return 1.0f / FPS;
            }
            return 0.0f;
        }

        void clearColor(Color color)
        {
            clearColor(color.r, color.g, color.b, color.a);
        }

        bool isOpen() const
        {
            return window && !glfwWindowShouldClose(window);
        }

    private:
        GLFWwindow* window;
        Renderer* renderer; 
        GraphicsAPI graphics;

        double previousTime;
        int frameCount = 0;
        int FPS;
    };


    void setDebug(DebugLevel value)
    {
        debug = value;
    }

    Window initWindow(int width, int height, std::string header, GraphicsAPI graphics = GraphicsAPI::OpenGL)
    {
        Window window(width, height, header, graphics);
        return window;
    }
}
