#include "openglrenderer.h"
#include "utils/vectors.h"
#include "utils/colors.h"
#include "utils/shaders.h"
#include "utils/files.h"
#include "utils/vertexlayout.h"

#include <iostream>
#include <unordered_map>

#define GL_CHECK(x) do { \
    x; \
    GLenum glErr = glGetError(); \
    if (glErr != GL_NO_ERROR) { \
        std::cerr << "[KERN] OpenGL Error @ " << #x << ": " << glErr << std::endl; \
    } \
} while(0)


OpenGLRenderer::OpenGLRenderer(GLFWwindow* window, int width, int height)
    : window(window), width(width), height(height),
      triProgram(
          []() -> kern::OpenGLShaderProgram
          {
              cast("Loading built-in shaders...");
              std::string triVertexSource = kern::readFile("src/shaders/OpenGL/tri.vert");
              std::string triFragmentSource = kern::readFile("src/shaders/OpenGL/tri.frag");

              if (triVertexSource.empty()) {
                  cast("Vertex shader source empty!", kern::DebugLevel::Error);
                  return kern::OpenGLShaderProgram("", ""); // Invalid
              }
              if (triFragmentSource.empty()) {
                  cast("Fragment shader source empty!", kern::DebugLevel::Error);
                  return kern::OpenGLShaderProgram("", "");
              }

              return kern::OpenGLShaderProgram(triVertexSource, triFragmentSource);
          }()
      )
{
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (!triProgram.getId()) {
        cast("Shader program failed to link!", kern::DebugLevel::Error);
    }
}

void OpenGLRenderer::clear()
{
    if (window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void OpenGLRenderer::present()
{
    if (window)
    {
        glfwSwapBuffers(window);
    }
}

void OpenGLRenderer::setClearColor(float r, float g, float b, float a)
{
    if (window)
    {
        glClearColor(r, g, b, a);
    }
}

void OpenGLRenderer::renderTri(kern::Vector2 a, kern::Vector2 b, kern::Vector2 c, kern::Color color)
{
    // 2 floats for position, 3 floats for color
    GLfloat vertices[] = {
        a.x, a.y,  color.r, color.g, color.b,
        b.x, b.y,  color.r, color.g, color.b,
        c.x, c.y,  color.r, color.g, color.b
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Check for errors
    if (VAO == 0 || VBO == 0) {
        std::cerr << "Failed to generate VAO/VBO!" << std::endl;
        return;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    triProgram.bind();

    // Ensure program is valid
    if (!triProgram.getId()) {
        cast("Shader program ID is 0!", kern::DebugLevel::Error);
        return;
    }

    GLint linked;
    glGetProgramiv(triProgram.getId(), GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLchar log[512];
        glGetProgramInfoLog(triProgram.getId(), 512, nullptr, log);
        cast("Link log: " + std::string(log), kern::DebugLevel::Error);
    }

    // Position attribute
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_CHECK(glEnableVertexAttribArray(0));

    // Color attribute
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(1));

    glBindVertexArray(0);

    // Draw
    glBindVertexArray(VAO);
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

    // Cleanup
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void OpenGLRenderer::bindVertexData(const void* vertices, size_t vertexCount, const kern::VertexLayout& layout)
{
    size_t hash = std::hash<size_t>{}(layout.getStride()); // Simplified hash
    // In practice, hash over element names/types

    GLuint vbo = 0, vao = 0;

    if (vboCache.count(hash)) {
        vbo = vboCache[hash];
        vao = vaoCache[hash];
    } else {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);

        vboCache[hash] = vbo;
        vaoCache[hash] = vao;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, layout.getStride() * vertexCount, vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    const auto& elements = layout.getElements();
    for (const auto& elem : elements) {
        glEnableVertexAttribArray(elem.index);
        glVertexAttribPointer(
            elem.index, 
            elem.getTypeComponentCount(),
            GL_FLOAT,
            GL_FALSE,
            layout.getStride(),
            (void*)elem.offset
        );
    }
}

void OpenGLRenderer::renderLine(kern::Vector2 a, kern::Vector2 b, kern::Color color, float thickness)
{
    kern::Vector2 dir = (b - a).normalized();
    kern::Vector2 n = kern::Vector2(-dir.y, dir.x) * (thickness * 0.002f);
    kern::Vector2 
    v0 = a + n,
    v1 = a - n,
    v2 = b - n,
    v3 = b + n;

    renderTri(v0, v1, v2, color);
    renderTri(v2, v3, v0, color);
}

void OpenGLRenderer::renderCircle(kern::Vector2 center, float radius, kern::Color color)
{
    int segments = 32;
    float step = 3.1415926535 * 2 / segments;

    for (int i = 0; i < segments; i++) {
        float a0 = i * step;
        float a1 = (i + 1) * step;

        kern::Vector2 p0 = center;
        kern::Vector2 p1 = center + kern::Vector2(cos(a0), sin(a0)) * radius;
        kern::Vector2 p2 = center + kern::Vector2(cos(a1), sin(a1)) * radius;

        renderTri(p0, p1, p2, color);
    }
}