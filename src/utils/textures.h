#pragma once

#include <cstdint>
#include <string>
#include <stb/stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/vectors.h"
#include "utils/colors.h"
#include "utils/shaders.h"
#include "utils/vertexlayout.h"
#include <unordered_map>

namespace kern {

class Texture {
public:
    virtual ~Texture() = default;

    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind() const = 0;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual unsigned int getID() const = 0;
};

class OpenGLTexture2D : public Texture {
public:
    OpenGLTexture2D(const std::string& path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* bytes = stbi_load(path.c_str(), &width, &height, &channels, 0);
        m_Width = width;
        m_Height = height;
        if (!bytes) {
            cast("Failed to load texture!", kern::DebugLevel::Error);
            return;
        }

        glGenTextures(1, &m_ID);
        bind();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLuint format;
        if (channels == 4) {
            format = GL_RGBA;
        } else if (channels == 3) {
            format = GL_RGB;
        }else if (channels == 2) {
            format = GL_RG;
        }else if (channels == 1) {
            format = GL_RED;
        } else {
            cast("Unsupported texture format", kern::DebugLevel::Error);
            stbi_image_free(bytes);
            unbind();
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(bytes);
        unbind();
    }
    ~OpenGLTexture2D()
    {
        if (m_ID) {
            glDeleteTextures(1, &m_ID);
        }
    }

    void bind(uint32_t slot = 0) const override
    {
        if (m_ID) {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_ID);
        }
    }

    void unbind() const override
    {
        if (m_ID) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    uint32_t getWidth() const override { return m_Width; }

    uint32_t getHeight() const override { return m_Height; }

    unsigned int getID() const override { return m_ID; }

private:
    unsigned int m_ID;
    int m_Width, m_Height, m_Channels;
};

inline OpenGLTexture2D loadTexture(const std::string& path)
{
    return OpenGLTexture2D(path);
}
}
