#include <iostream>
#include <filesystem>
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
            lastFrameTime = glfwGetTime();
            std::fill(m_prevMouseButtons, m_prevMouseButtons + MOUSE_BUTTON_COUNT, false);

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

            // Sample current mouse button states
            for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
            {
                m_prevMouseButtons[i] = m_currMouseButtons[i];
                m_currMouseButtons[i] = window && (glfwGetMouseButton(window, i) == GLFW_PRESS);
            }

            glfwPollEvents();

            double currentTime = glfwGetTime();
            m_deltaTime = static_cast<float>(currentTime - lastFrameTime);
            lastFrameTime = currentTime;
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

        float getDeltaTime() const { return m_deltaTime; }

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
            int btn = static_cast<int>(button);
            return btn < MOUSE_BUTTON_COUNT && m_currMouseButtons[btn];
        }

        bool wasMouseButtonPressed(MouseButton button) const
        {
            int btn = static_cast<int>(button);
            return !m_prevMouseButtons[btn] && m_currMouseButtons[btn]; // pressed now, not before
        }

        int getWidth() const
        {
            if (window)
            {
                int width, height;
                getSize(width, height);
                return width;
            }
            return 0;
        }

        int getHeight() const
        {
            if (window)
            {
                int width, height;
                getSize(width, height);
                return height;
            }
            return 0;
        }

        void getSize(int& width, int& height) const
        {
            if (window)
            {
                if (isFullscreen())
                {
                    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                    width = mode->width;
                    height = mode->height;
                    return;
                }
                glfwGetWindowSize(window, &width, &height);
            }
        }

        void setMouse(bool hide)
        {
            if (window)
            {
                glfwSetInputMode(window, GLFW_CURSOR, hide ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            }
        }

        void setMousePosition(Vector2 position)
        {
            if (window)
            {
                glfwSetCursorPos(window, position.x, position.y);
            }
        }

        bool isFocused() const
        {
            if (window)
            {
                return glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;
            }
            return false;
        }

        void setFullscreen(bool fullscreen)
        {
            if (!window) return;

            static int storedX, storedY, storedWidth, storedHeight;
            static GLFWmonitor* monitor = nullptr;

            if (fullscreen && !monitor) {
                // Switch to fullscreen
                monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                glfwGetWindowPos(window, &storedX, &storedY);
                glfwGetWindowSize(window, &storedWidth, &storedHeight);

                glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            }
            else if (!fullscreen && monitor) {
                // Restore windowed mode
                glfwSetWindowMonitor(window, nullptr, storedX, storedY, storedWidth, storedHeight, 0);
                monitor = nullptr;
            }
        }

        void setVsync(bool enabled)
        {
            if (window)
            {
                glfwSwapInterval(enabled ? 1 : 0);
            }
        }

        Vector2 getSize() const
        {
            if (window)
            {
                int width, height;
                getSize(width, height);
                return {static_cast<float>(width), static_cast<float>(height)};
            }
            return {0, 0};
        }

        bool isFullscreen() const
        {
            if (window)
            {
                return glfwGetWindowMonitor(window) != nullptr;
            }
            return false;
        }

    private:
        GLFWwindow* window;
        Renderer* renderer; 
        GraphicsAPI graphics;

        double previousTime;
        int frameCount = 0;
        int FPS;
        double lastFrameTime = 0.0;
        float m_deltaTime = 0.0f;

        // For key state tracking
        static constexpr int KEY_COUNT = 512;
        bool m_prevKeys[KEY_COUNT] = {false};
        bool m_currKeys[KEY_COUNT] = {false};
        static constexpr int MOUSE_BUTTON_COUNT = 8;
        bool m_prevMouseButtons[MOUSE_BUTTON_COUNT] = {false};
        bool m_currMouseButtons[MOUSE_BUTTON_COUNT] = {false};
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
