#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "config.h"
#include "utils/colors.h"
#include "utils/vectors.h"
#include "backends/OpenGL/openglrenderer.h"

#include "utils/inputs.h"

namespace kern
{
    enum class GraphicsAPI
    {
        OpenGL,
        // Vulkan, // TODO
        // DirectX12
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

                cast("using: OpenGL Version 3.3");
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
            cast("Creating Renderer...");
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

            // Sample current key states
            for (int i = 0; i < KEY_COUNT; i++)
            {
                m_prevKeys[i] = m_currKeys[i];
                m_currKeys[i] = window && (glfwGetKey(window, i) == GLFW_PRESS);
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

        float getTime() const
        {
            if (isOpen() && window)
            {
                return glfwGetTime();
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
    
        void close()
        {
            if (window)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }

        // Check if a key is currently pressed
        bool isKeyPressed(Key key) const
        {
            int k = static_cast<int>(key);
            return k < KEY_COUNT && m_currKeys[k];
        }

        // Get all keys currently pressed
        std::vector<Key> getPressedKeys() const
        {
            std::vector<Key> pressed;
            for (int i = 0; i < KEY_COUNT; i++)
            {
                if (m_currKeys[i])
                {
                    // Optional: validate that i is a known key (you can skip invalid ones)
                    pressed.push_back(static_cast<Key>(i));
                }
            }
            return pressed;
        }

        // Check if a key was just released this frame
        bool wasKeyReleased(Key key) const
        {
            int k = static_cast<int>(key);
            if (k >= KEY_COUNT) return false;
            return m_prevKeys[k] && !m_currKeys[k];
        }

        // Optional: detect if key was just pressed (edge detection)
        bool wasKeyPressed(Key key) const
        {
            int k = static_cast<int>(key);
            if (k >= KEY_COUNT) return false;
            return !m_prevKeys[k] && m_currKeys[k];
        }

        Vector2 getMousePosition() const
        {
            if (window)
            {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                return {static_cast<float>(x), static_cast<float>(y)};
            }
            return {0, 0};
        }

        bool isMouseButtonPressed(MouseButton button) const
        {
            if (window)
            {
                return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
            }
            return false;
        }

        int getWidth() const
        {
            if (window)
            {
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                return width;
            }
            return 0;
        }

        int getHeight() const
        {
            if (window)
            {
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                return height;
            }
            return 0;
        }

        void getSize(int& width, int& height) const
        {
            if (window)
            {
                glfwGetWindowSize(window, &width, &height);
            }
        }

        Vector2 getSize() const
        {
            if (window)
            {
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                return {static_cast<float>(width), static_cast<float>(height)};
            }
            return {0, 0};
        }

    private:
        GLFWwindow* window;
        Renderer* renderer; 
        GraphicsAPI graphics;

        double previousTime;
        int frameCount = 0;
        int FPS;

        // For key state tracking
        static constexpr int KEY_COUNT = 512;
        bool m_prevKeys[KEY_COUNT] = {false};
        bool m_currKeys[KEY_COUNT] = {false};
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
