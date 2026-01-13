#include "kern.h"

int main()
{
    kern::Window window =
        kern::initWindow(800, 600, "Kern - Input");

    while (window.isOpen())
    {
        window.clear();
        window.clearColor(0.1f, 0.1f, 0.1f);

        if (window.wasKeyPressed(kern::Key::ESCAPE))
            window.close();

        kern::Vector2 mouseNDC =
            kern::screenToNDC(window.getMousePosition(), window.getSize());

        window.circle(mouseNDC, 0.03f, kern::WHITE);

        if (window.isMouseButtonPressed(kern::MouseButton::LEFT))
            window.line({0, 0}, mouseNDC, kern::RED, 2);

        window.present();
    }

    return 0;
}
