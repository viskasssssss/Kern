#include "kern.h"
#include <vector>

struct Vertex
{
    kern::Vector3 pos;
};

int main()
{
    kern::Window window =
        kern::initWindow(1280, 720, "Kern - 3D Cube");

    auto shader = kern::createShader(
        "examples/3d.vert",
        "examples/3d.frag"
    );

    shader.setVertexLayout(
        kern::VertexLayout{}
            .add<kern::Vector3>("a_Position")
    );

    std::vector<Vertex> cube = {
        // front
        {{-0.5f,-0.5f, 0.5f}},
        {{0.5f,-0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}},
        {{-0.5f, 0.5f, 0.5f}},
        {{-0.5f,-0.5f, 0.5f}},

        // back
        {{-0.5f,-0.5f,-0.5f}},
        {{-0.5f, 0.5f,-0.5f}},
        {{0.5f, 0.5f,-0.5f}},
        {{0.5f, 0.5f,-0.5f}},
        {{0.5f,-0.5f,-0.5f}},
        {{-0.5f,-0.5f,-0.5f}},

        // left
        {{-0.5f, 0.5f, 0.5f}},
        {{-0.5f, 0.5f,-0.5f}},
        {{-0.5f,-0.5f,-0.5f}},
        {{-0.5f,-0.5f,-0.5f}},
        {{-0.5f,-0.5f, 0.5f}},
        {{-0.5f, 0.5f, 0.5f}},

        // right
        {{0.5f, 0.5f, 0.5f}},
        {{0.5f,-0.5f,-0.5f}},
        {{0.5f, 0.5f,-0.5f}},
        {{0.5f,-0.5f,-0.5f}},
        {{0.5f, 0.5f, 0.5f}},
        {{0.5f,-0.5f, 0.5f}},

        // top
        {{-0.5f, 0.5f,-0.5f}},
        {{-0.5f, 0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}},
        {{0.5f, 0.5f,-0.5f}},
        {{-0.5f, 0.5f,-0.5f}},

        // bottom
        {{-0.5f,-0.5f,-0.5f}},
        {{0.5f,-0.5f, 0.5f}},
        {{-0.5f,-0.5f, 0.5f}},
        {{0.5f,-0.5f, 0.5f}},
        {{-0.5f,-0.5f,-0.5f}},
        {{0.5f,-0.5f,-0.5f}}
    };

    while (window.isOpen())
    {
        window.clear();

        float t = window.getTime();

        kern::Mat4 model = kern::rotate(
            kern::Mat4(1.0f), t,
            kern::toGlm(kern::Vector3(0.3f, 1.0f, 0.2f))
        );

        kern::Mat4 view = kern::translate(
            kern::Mat4(1.0f),
            kern::toGlm(kern::Vector3(0,0,-2))
        );

        kern::Mat4 proj = kern::perspective(
            kern::radians(60.0f),
            1280.0f / 720.0f,
            0.1f, 100.0f
        );

        shader.setMat4("u_MVP", proj * view * model);
        window.draw(cube, shader);

        window.present();
    }

    return 0;
}
