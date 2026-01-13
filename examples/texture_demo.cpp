#include "kern.h"
#include <vector>

struct Vertex
{
    kern::Vector2 pos;
    kern::Vector2 uv;
};

int main()
{
    kern::Window window =
        kern::initWindow(800, 600, "Kern - Textures");

    auto shader = kern::createShader(
        "examples/textured.vert",
        "examples/textured.frag"
    );

    shader.setVertexLayout(
        kern::VertexLayout{}
            .add<kern::Vector2>("a_Position")
            .add<kern::Vector2>("a_UV")
    );

    auto texture = kern::loadTexture("assets/cube.png");

    std::vector<Vertex> quad = {
        {{-0.5f,-0.5f},{0,0}},
        {{ 0.5f,-0.5f},{1,0}},
        {{ 0.5f, 0.5f},{1,1}},
        {{ 0.5f, 0.5f},{1,1}},
        {{-0.5f, 0.5f},{0,1}},
        {{-0.5f,-0.5f},{0,0}},
    };

    while (window.isOpen())
    {
        window.clear();

        shader.setSample2D("u_Texture", texture);
        window.draw(quad, shader);

        window.present();
    }

    return 0;
}
