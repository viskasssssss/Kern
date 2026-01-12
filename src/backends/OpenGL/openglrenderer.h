#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/vectors.h"
#include "utils/colors.h"
#include "backends/renderer.h"
#include "utils/shaders.h"
#include "utils/vertexlayout.h"
#include <unordered_map>

#include "config.h"
#include <vector>

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(GLFWwindow* window, int width, int height);
    ~OpenGLRenderer() override = default;

    void clear() override;
    void present() override;
    void setClearColor(float r, float g, float b, float a) override;
    void renderTri(kern::Vector2 a, kern::Vector2 b, kern::Vector2 c, kern::Color color) override;
    void renderLine(kern::Vector2 a, kern::Vector2 b, kern::Color color, float thickness) override;
    void renderCircle(kern::Vector2 center, float radius, kern::Color color) override;

    template<typename Vertex>
    void draw(const std::vector<Vertex>& vertices, const kern::OpenGLShaderProgram& shader)
    {
        if (vertices.empty()) return;

        const kern::VertexLayout& layout = shader.getVertexLayout();
        if (layout.getStride() != sizeof(Vertex)) {
            cast("Vertex size mismatch!", kern::DebugLevel::Error);
            return;
        }

        shader.bind();
        bindVertexData(vertices.data(), vertices.size(), layout);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(vertices.size()));
        glBindVertexArray(0);
    }

private:
    GLFWwindow* window;
    int width, height;

    // Remove default initialization
    kern::OpenGLShaderProgram triProgram;

    mutable std::unordered_map<size_t, GLuint> vboCache;
    mutable std::unordered_map<size_t, GLuint> vaoCache;

    void bindVertexData(const void* vertices, size_t vertexCount, const kern::VertexLayout& layout);
};
