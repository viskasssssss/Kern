#include "kern.h"
#include <iostream>

int main()
{
    kern::setDebug(kern::DebugLevel::Everything);
    kern::Window window = kern::initWindow(800, 600, "Test", kern::GraphicsAPI::OpenGL);
    // Define your vertex
    struct MyVertex {
        kern::Vector2 pos;
        kern::Vector2 uv;
        float intensity;
    };

    // Load shader
    auto shader = kern::createShader("testShaders/test.vert", "testShaders/test.frag");

    // Register what the shader expects
    shader.setVertexLayout(
        kern::VertexLayout{}
            .add<kern::Vector2>("a_Position")
            .add<kern::Vector2>("a_UV")
            .add<float>("a_Intensity")
    );

    std::vector<MyVertex> verts = {
        { {-0.5, -0.5}, {0,0}, 1.0f },
        { { 0.5, -0.5}, {1,0}, 0.8f },
        { { 0.0,  0.5}, {0.5,1}, 0.3f }
    };

    while (window.isOpen()) {
        window.clear();

        window.draw(verts, shader); // <--- SO CLEAN!

        window.present();
    }

    return 0;
}
