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

    inline Mat4 lookAt(Vector3 pos, Vector3 target, Vector3 up)
    {
        return glm::lookAt(toGlm(pos), toGlm(target), toGlm(up));
    }

    inline float radians(float degrees)
    {
        return glm::radians(degrees);
    }

    inline Vector3 cross(Vector3 a, Vector3 b)
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    inline float dot(Vector3 a, Vector3 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
}