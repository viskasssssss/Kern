#include "kern.h"

int main()
{
    kern::Window window =
        kern::initWindow(800, 600, "Kern - 2D Shapes");

    while (window.isOpen())
    {
        window.clear();
        window.clearColor(kern::DARK_GRAY);

        window.line({-0.8f, 0.0f}, {0.8f, 0.0f}, kern::RED, 3.0f);
        window.circle({0.0f, 0.0f}, 0.2f, kern::GREEN);
        window.tri(
            {-0.3f, -0.3f},
            { 0.3f, -0.3f},
            { 0.0f,  0.4f},
            kern::BLUE
        );

        window.present();
    }

    return 0;
}