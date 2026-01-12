#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "config.h"
#include "utils/files.h"
#include "utils/vertexlayout.h"

namespace kern
{

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setFloat(const std::string& name, float value) = 0;
        virtual void setVec2(const std::string& name, Vector2 value) = 0;
        virtual void setVec3(const std::string& name, Vector3 value) = 0;

        virtual unsigned int getId() const = 0;
    };

    class OpenGLShaderProgram : public Shader
    {
    public:
        OpenGLShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
        {
            GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
            GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

            if (vertexShader == 0 || fragmentShader == 0)
            {
                id = 0;
                cast("Shader compilation failed", DebugLevel::Error);
                return;
            }

            id = glCreateProgram();
            glAttachShader(id, vertexShader);
            glAttachShader(id, fragmentShader);
            glLinkProgram(id);

            GLint success;
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success)
            {
                GLchar infoLog[512];
                glGetProgramInfoLog(id, 512, nullptr, infoLog);
                cast("Program link error: " + std::string(infoLog), DebugLevel::Error);
                glDeleteProgram(id); 
                id = 0;
                return;
            }
            
            glValidateProgram(id);
            GLint validated;
            glGetProgramiv(id, GL_VALIDATE_STATUS, &validated);
            if (!validated)
            {
                GLchar log[512];
                glGetProgramInfoLog(id, 512, nullptr, log);
                cast("Program validation failed: " + std::string(log), DebugLevel::Error);
                glDeleteProgram(id);
                id = 0;
                return;
            }

            cast("Program linked with ID: " + std::to_string(id), kern::DebugLevel::Everything);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        OpenGLShaderProgram()
        {
            id = 0;
        }

        ~OpenGLShaderProgram()
        {
            if (id != 0)
            {
                glDeleteProgram(id);
            }
        }

        OpenGLShaderProgram(const OpenGLShaderProgram&) = delete;
        OpenGLShaderProgram& operator=(const OpenGLShaderProgram&) = delete;

        OpenGLShaderProgram(OpenGLShaderProgram&& other) noexcept
            : vertexLayout(std::move(other.vertexLayout))
            , id(other.id)
        {
            other.id = 0;
        }

        OpenGLShaderProgram& operator=(OpenGLShaderProgram&& other) noexcept
        {
            if (this != &other) {
                if (id) glDeleteProgram(id);
                vertexLayout = std::move(other.vertexLayout);
                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        void bind() const override
        {
            if (!id) {
                cast("Trying to bind shader with id=0!", kern::DebugLevel::Error);
                return;
            }

            glUseProgram(id);
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                cast("glUseProgram error: " + std::to_string(err) + " with ID: " + std::to_string(id), DebugLevel::Error);
            }
        }


        void unbind() const override
        {
            glUseProgram(0);
        }

        void setFloat(const std::string& name, float value) override
        {
            glUniform1f(glGetUniformLocation(id, name.c_str()), value);
        }

        void setVec2(const std::string& name, Vector2 value) override
        {
            glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
        }

        void setVec3(const std::string& name, Vector3 value) override
        {
            glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
        }

        unsigned int getId() const override { return id; }

        void setVertexLayout(const VertexLayout& layout) { vertexLayout = layout; }
        const VertexLayout& getVertexLayout() const { return vertexLayout; }

    private:
        VertexLayout vertexLayout;
        GLuint id;

        GLuint compileShader(const std::string& source, GLenum type)
        {
            GLuint shader = glCreateShader(type);

            // Explicitly ensure null termination
            const char* src = source.c_str();
            GLint length = static_cast<GLint>(source.length());

            glShaderSource(shader, 1, &src, &length);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                GLchar infoLog[512];
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                cast("Shader compile error: " + std::string(infoLog), DebugLevel::Error);
                glDeleteShader(shader);
                return 0;
            }

            return shader;
        }
    };

    inline OpenGLShaderProgram createShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
    {
        std::string vertexSource = kern::readFile(vertexFilepath);
        std::string fragmentSource = kern::readFile(fragmentFilepath);

        if (vertexSource.empty()) {
            cast("Vertex shader source empty", kern::DebugLevel::Error);
            return OpenGLShaderProgram("", ""); // Invalid
        }
        if (fragmentSource.empty()) {
            cast("Fragment shader source empty", kern::DebugLevel::Error);
            return OpenGLShaderProgram("", "");
        }

        return OpenGLShaderProgram(vertexSource, fragmentSource);
    }
}
