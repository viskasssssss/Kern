# Kern API Reference

**Kern** — lightweight C++ library for 2D/3D graphics.  
This document describes the main classes, functions, and usage patterns.

---

## Table of Contents

1. [Window](#window)
2. [Colors](#colors)
3. [Vector2 & Vector3](#vector2--vector3)
4. [Matrix (Mat4)](#matrix-mat4)
5. [Shader](#shader)
6. [Texture](#texture)
7. [Input](#input)
8. [Utility Functions](#utility-functions)
9. [Examples](#examples)

---

## Window

`kern::Window` is the main object for creating a window, rendering, and handling input.

### Initialization

```cpp
#include "kern.h"

kern::Window window = kern::initWindow(1280, 720, "My App", kern::GraphicsAPI::OpenGL);
```

### Parameters:
- `width`, `height` — window size in pixels.
- `title` — window title.
- `GraphicsAPI` — choose `OpenGL`, `Vulkan`, or `DirectX`. Default: `OpenGL`.

p.s: for now only OpenGL is available

### Basic Methods
``` cpp
window.clear();                         // Clear the framebuffer
window.clearColor(kern::Color(0.1f,0.1f,0.1f)); // Set background color
window.present();                       // Swap buffers, display current frame
```

### Drawing 2D Shapes
``` cpp
window.tri(Vector2 a, Vector2 b, Vector2 c, Color color); // Draw a triangle
window.line(Vector2 start, Vector2 end, Color color, float thickness = 1.0f); // Draw a line
window.circle(Vector2 center, float radius, Color color); // Draw a circle
```

- These debug methods are immediate-mode and perfect for learning or rapid prototyping.

### Window Status
``` cpp
window.isOpen();                  // Check if window is open
window.close();                   // Close window
window.getFPS();                  // Get current frames per second
window.getDeltaTime();            // Get current delta time
window.setTitle("My App");        // Set window title dynamically
window.getTime();                 // Time in seconds since window was created
```

---

## Colors
`kern::Color` class represents RGBA colors.

``` cpp
kern::Color white(1.0f, 1.0f, 1.0f, 1.0f);
kern::Color red(1.0f, 0.0f, 0.0f, 1.0f);
```

Predefined constants: `kern::WHITE`, `kern::BLACK`, `kern::RED`, `kern::DARK_GRAY`, etc.

## Vector2 & Vector3
### Vector2
``` cpp
kern::Vector2 v1(1.0f, 2.0f);
kern::Vector2 v2 = v1 * 2.0f;     // Scalar multiplication
float len = v1.length();           // Length of the vector
v1.normalize();                     // Normalize the vector
float dotProd = v1.dot(v2);        // Dot product
kern::Vector2 perpV = v1.perp();   // Perpendicular vector
```

### Vector3
``` cpp
kern::Vector3 v3(1.0f, 2.0f, 3.0f);
float len3 = v3.length();
v3.normalize();
```

## Matrix (Mat4)

Matrices are used for 3D transformations. Kern internally uses `glm::mat4:`
``` cpp
using Mat4 = glm::mat4; // You can directly use glm functions if needed
```
### *Example: Model-View-Projection*
``` cpp
kern::Mat4 model = kern::rotate(kern::Mat4(1.0f), time, kern::toGlm(kern::Vector3(0.5f,1.0f,0.0f)));
kern::Mat4 view = kern::translate(kern::Mat4(1.0f), kern::toGlm(kern::Vector3(0,0,-2)));
kern::Mat4 projection = kern::perspective(kern::radians(60.0f), 1280.0f/720.0f, 0.1f, 100.0f);
kern::Mat4 mvp = projection * view * model;
```

- Pass mvp to shader uniform:
``` cpp
shader.setMat4("u_MVP", mvp);
```

## Shader

Custom shaders can be loaded and used for advanced rendering.

``` cpp
auto shader = kern::createShader("vertex.vert", "fragment.frag");

// Register vertex attributes
shader.setVertexLayout(
    kern::VertexLayout{}
        .add<kern::Vector3>("a_Position")
        .add<kern::Vector2>("a_UV")
);
```

### *Set uniforms:*

``` cpp
shader.setMat4("u_MVP", mvp);
```

## Texture

Load and bind textures:

``` cpp
auto texture = kern::loadTexture("cat.jpeg");
texture.bind(0); // Bind to slot 0
```

- `setSample2D` automatically binds the texture in the shader.

``` cpp
shader.setSample2D("u_Texture", texture);
```

## Input

Handle keyboard and mouse easily:

``` cpp
if(window.wasKeyPressed(kern::Key::ESCAPE)) window.close();

if(window.isMouseButtonPressed(kern::MouseButton::LEFT)) {
    // Draw something under mouse
}

kern::Vector2 mousePos = window.getMousePosition();
```

- `window.getPressedKeys()` returns currently pressed keys.

- `screenToNDC(position, size)` converts screen coordinates to normalized device coordinates (-1 to 1).

## Utility Functions

- `kern::radians(float degrees)` — convert degrees to radians
- `kern::toGlm(Vector3)` — convert Kern vectors to glm vectors
- `kern::setDebug(DebugLevel level)` — enable debug logging

## Examples

See examples/ folder in the repository:

- `2D_demo.cpp` — basic shapes, lines, triangles, circles
- `3D_demo.cpp` — 3D cube with textures and shader
- `Input_demo.cpp `— mouse and keyboard events
- `texture_demo.cpp `— rendering textures

---

This completes the __main API reference__. Kern is designed to be __minimal, fast, and easy to use__, while still allowing full control over shaders, textures, and rendering pipelines.