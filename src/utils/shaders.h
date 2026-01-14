#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "config.h"
#include "utils/files.h"
#include "utils/vertexlayout.h"
#include "utils/textures.h"
#include "kernmath.h"

namespace kern
{
    class Texture;

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setFloat(const std::string& name, float value) = 0;
        virtual void setVec2(const std::string& name, Vector2 value) = 0;
        virtual void setVec3(const std::string& name, Vector3 value) = 0;
        virtual void setSample2D(const std::string& name, const Texture& texture) = 0;
        virtual void setMat4(const std::string& name, const Mat4& matrix) = 0;
        virtual void setMat3(const std::string& name, const Mat3& matrix) = 0;

        virtual unsigned int getId() const = 0;
    };

    class OpenGLShaderProgram : public Shader
    {
    public:
        OpenGLShaderProgram(const std::string& vertexSource, const std::string& fragmentSource, const std::string& vertexFilepath = "", const std::string& fragmentFilepath = "")
        {
            GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER, vertexFilepath);
            GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER, fragmentFilepath);

            if (vertexShader == 0 || fragmentShader == 0)
            {
                id = 0;
                cast("Shader compilation failed", DebugLevel::Error);
                return;
            }

            id = glCreateProgram();
            if (!id) {
                cast("glCreateProgram returned 0", kern::DebugLevel::Error);
                return;
            }
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
            GLenum error;
            while ((error = glGetError()) != GL_NO_ERROR) {
                // Just clear
            }

            if (!id) {
                cast("Trying to bind shader with id=0!", kern::DebugLevel::Error);
                return;
            }

            GLint linked = 0;
            glGetProgramiv(id, GL_LINK_STATUS, &linked);
            if (linked == GL_FALSE) {
                cast("Shader program " + std::to_string(id) + " is not linked! Cannot use.", kern::DebugLevel::Error);
                return;
            }

            glUseProgram(id);

            GLenum err = glGetError();
            if (err == GL_INVALID_OPERATION) {
                cast("glUseProgram: GL_INVALID_OPERATION. Possible reasons: program not linked, or context not current.", DebugLevel::Error);
            } else if (err != GL_NO_ERROR) {
                cast("glUseProgram error: " + std::to_string(err) + " with ID: " + std::to_string(id), DebugLevel::Error);
            }
        }


        void unbind() const override
        {
            glUseProgram(0);
        }

        void setFloat(const std::string& name, float value) override
        {
            bind();
            GLint loc = getLocation(name);
            glUniform1f(loc, value);
        }

        void setVec2(const std::string& name, Vector2 value) override
        {
            bind();
            GLint loc = getLocation(name);
            glUniform2f(loc, value.x, value.y);
        }

        void setVec3(const std::string& name, Vector3 value) override
        {
            bind();
            GLint loc = getLocation(name);
            glUniform3f(loc, value.x, value.y, value.z);
        }

        void setMat4(const std::string& name, const Mat4& matrix) override {
            bind();
            GLint loc = getLocation(name);
            glUniformMatrix4fv(
                loc,
                1,
                GL_FALSE,
                glm::value_ptr(matrix)
            );
        }

        void setMat3(const std::string& name, const Mat3& matrix)override {
            bind();
            GLint loc = getLocation(name);
            glUniformMatrix3fv(
                loc,
                1, 
                GL_FALSE, 
                glm::value_ptr(matrix)
            );
        }

        void setSample2D(const std::string& name, const Texture& texture) override;

        unsigned int getId() const override { return id; }

        void setVertexLayout(const VertexLayout& layout) { vertexLayout = layout; }
        const VertexLayout& getVertexLayout() const { return vertexLayout; }

    private:
        VertexLayout vertexLayout;
        GLuint id;

        GLint getLocation(const std::string& name) const
        {
            if (id == 0) 
            {
                return -1;
            }
            GLint loc = glGetUniformLocation(id, name.c_str());
            if (loc == -1) {
                cast("Warning: Uniform '" + name + "' not found or optimized out", DebugLevel::Warning);
                return -1;
            }
            return loc;
        }

        std::string getShaderInfoLog(GLuint shader) {
            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength == 0) return "";

            std::string log(logLength - 1, '\0');  // -1 to skip null terminator
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            return log;
        }

        GLuint compileShader(const std::string& source, GLenum type, const std::string& path)
        {
            cast("Compiling shader: '" + path + "'", false);
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
                cast(" [FAILED]", DebugLevel::Everything, false);
                cast("Shader compile error: " + getShaderInfoLog(shader), DebugLevel::Error);
                glDeleteShader(shader);
                return 0;
            }

            cast(" [OK]", DebugLevel::Everything, false);

            return shader;
        }
    };

    inline OpenGLShaderProgram createShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
    {
        std::string vertexSource = kern::readFile(vertexFilepath);
        std::string fragmentSource = kern::readFile(fragmentFilepath);

        if (vertexSource.empty()) {
            cast("Vertex shader source empty", kern::DebugLevel::Error);
            return OpenGLShaderProgram("", "", vertexFilepath, fragmentFilepath); // Invalid
        }
        if (fragmentSource.empty()) {
            cast("Fragment shader source empty", kern::DebugLevel::Error);
            return OpenGLShaderProgram("", "", vertexFilepath, fragmentFilepath);
        }

        return OpenGLShaderProgram(vertexSource, fragmentSource, vertexFilepath, fragmentFilepath);
    }
}
