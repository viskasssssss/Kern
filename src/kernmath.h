#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace kern
{
    using Mat4 = glm::mat4;
    using Mat3 = glm::mat3;

    inline Mat4 rotate(Mat4 mat, float angle, glm::vec3 axis)
    {
        return glm::rotate(mat, angle, axis);
    }

    inline Mat4 translate(Mat4 mat, glm::vec3 pos)
    {
        return glm::translate(mat, pos);
    }

    inline Mat4 scale(Mat4 mat, glm::vec3 scale)
    {
        return glm::scale(mat, scale);
    }

    inline Mat4 lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
    {
        return glm::lookAt(pos, target, up);
    }

    inline Mat4 perspective(float fov, float aspect, float near, float far)
    {
        if (fov > 0)
            return glm::perspective(fov, aspect, near, far);
        else
            return glm::ortho(-aspect, aspect, -1.0f, 1.0f, near, far);
    }

    inline glm::vec3 toGlm(Vector3 vec)
    {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    inline glm::vec2 toGlm(Vector2 vec)
    {
        return glm::vec2(vec.x, vec.y);
    }

    inline float radians(float degrees)
    {
        return glm::radians(degrees);
    }
}