<img src="assets/kern-logo.png" alt="Kern Logo" width="400" height="250"/>

**Kern** is a lightweight and easy-to-use C++ library for 2D and 3D graphics rendering.  
It provides a clean API for drawing shapes, handling input, working with shaders and textures, and using matrices — all without dealing with low-level OpenGL, Vulkan, or DirectX details.

Kern is designed for **learning, prototyping, and rapid development** while still being fast and flexible.

---

## Features

- **Clean and intuitive API**: draw triangles, lines, circles, or custom vertices with a single function call.
- **2D and 3D support**: `Vector2`, `Vector3`, and `Mat4` (backed by GLM for 3D math).
- **Shaders & textures**: easily load and use your own shaders and textures.
- **Input handling**: keyboard, mouse, and other input events.
- **Debug drawing**: draw shapes for testing and learning, similar to Python's `turtle`.
- **High performance**: hundreds of thousands of triangles at high FPS, even on integrated GPUs.

---

## Quick Start (2D Example)

![Triangle](assets/triangle.png)

```cpp
#include "kern.h"

int main() {
    kern::setDebug(kern::DebugLevel::Everything);
    kern::Window window = kern::initWindow(800, 600, "Kern Demo", kern::GraphicsAPI::OpenGL);

    while (window.isOpen()) {
        window.clear();
        window.clearColor(0.1f, 0.1f, 0.1f);

        if (window.wasKeyPressed(kern::Key::ESCAPE))
            window.close();

        // Draw a simple triangle
        window.tri(kern::Vector2(-0.5f, -0.5f),
                   kern::Vector2(0.0f, 0.5f),
                   kern::Vector2(0.5f, -0.5f),
                   kern::DARK_GRAY);

        window.present();
    }

    return 0;
}
```
## 3D Example

![Rotating Cube](assets/cube.png)

``` cpp
#include "kern.h"

int main() {
    kern::Window window = kern::initWindow(1280, 720, "Kern 3D", kern::GraphicsAPI::OpenGL);

    struct Vertex { kern::Vector3 pos; };
    std::vector<Vertex> cube = { /* cube vertices here */ };

    auto shader = kern::createShader("shaders/3d.vert", "shaders/3d.frag");

    shader.setVertexLayout(kern::VertexLayout{}.add<kern::Vector3>("a_Position"));

    while (window.isOpen()) {
        window.clear();
        window.clearColor(0.1f, 0.1f, 0.1f);

        // 3D Matrices
        kern::Mat4 model = kern::rotate(kern::Mat4(1.0f), window.getTime(), kern::toGlm(kern::Vector3(0.5f,1.0f,0.0f)));
        kern::Mat4 view = kern::translate(kern::Mat4(1.0f), kern::toGlm(kern::Vector3(0,0,-2)));
        kern::Mat4 projection = kern::perspective(kern::radians(60.0f), 1280.0f/720.0f, 0.1f, 100.0f);
        shader.setMat4("u_MVP", projection * view * model);

        window.draw(cube, shader);
        window.present();
    }
}
```

# API

Full API: [API Reference](docs/API.md)

# Installation
## Using CMake

``` cmake
add_subdirectory(path/to/Kern)
target_link_libraries(MyApp PRIVATE kern)
```

## Using headers only

Simply include src/ in your include path and include:

``` cpp
#include "kern.h"
```

---

# Requirements
- C++20 compiler
- OpenGL (or DirectX/Vulkan for future backends)
- GLFW (for OpenGL backend)
- GLM (for 3D math)

## License
Kern is released under the MIT License. See LICENSE for details.

## Notes

- Kern hides low-level details like VAO/VBO, letting you focus on learning and prototyping.
- Debug drawing functions (tri, line, circle) are great for educational purposes.
- For high-performance applications, users can extend Kern or implement custom renderers.