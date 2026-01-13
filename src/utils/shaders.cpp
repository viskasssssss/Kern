#include "utils/shaders.h"
#include "utils/textures.h"

namespace kern {

void OpenGLShaderProgram::setSample2D(const std::string& name, const Texture& texture)
{
    bind();
    texture.bind();
    GLint loc = glGetUniformLocation(id, name.c_str());
    if (loc == -1) return; // uniform not found

    if (texture.getID() != 0) {
        glUniform1i(loc, 0); 
    }
}

} // namespace kern
