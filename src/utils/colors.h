#pragma once

namespace kern
{
    class Color
    {
        public:
            float r, g, b, a;
            constexpr Color(float r, float g, float b, float a)
            : r(r), g(g), b(b), a(a)
            {}
            constexpr Color(float r, float g, float b)
            : r(r), g(g), b(b), a(1.0f)
            {}
            constexpr Color()
            : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
            {}
    };

    constexpr Color operator"" _rgb(unsigned long long value)
    {
        return Color(
            ((value >> 16) & 0xFF) / 255.0f,
            ((value >> 8)  & 0xFF) / 255.0f,
            (value & 0xFF) / 255.0f
        );
    }

    constexpr Color RED     = Color(1.0f, 0.0f, 0.0f, 1.0f);
    constexpr Color GREEN   = Color(0.0f, 1.0f, 0.0f, 1.0f);
    constexpr Color BLUE    = Color(0.0f, 0.0f, 1.0f, 1.0f);
    constexpr Color WHITE   = Color(1.0f, 1.0f, 1.0f, 1.0f);
    constexpr Color BLACK   = Color(0.0f, 0.0f, 0.0f, 1.0f);
    constexpr Color YELLOW  = Color(1.0f, 1.0f, 0.0f, 1.0f);
    constexpr Color CYAN    = Color(0.0f, 1.0f, 1.0f, 1.0f);
    constexpr Color MAGENTA = Color(1.0f, 0.0f, 1.0f, 1.0f);
    constexpr Color ORANGE  = Color(1.0f, 0.5f, 0.0f, 1.0f);
    constexpr Color PURPLE  = Color(0.5f, 0.0f, 0.5f, 1.0f);
    constexpr Color GRAY    = Color(0.5f, 0.5f, 0.5f, 1.0f);
    constexpr Color LIGHT_GRAY = Color(0.8f, 0.8f, 0.8f, 1.0f);
    constexpr Color DARK_GRAY  = Color(0.25f, 0.25f, 0.25f, 1.0f);
    constexpr Color BROWN   = Color(0.65f, 0.33f, 0.12f, 1.0f);
    constexpr Color PINK    = Color(1.0f, 0.75f, 0.8f, 1.0f);
    constexpr Color LIME    = Color(0.75f, 1.0f, 0.0f, 1.0f);
    constexpr Color INDIGO  = Color(0.29f, 0.0f, 0.51f, 1.0f);
    constexpr Color TEAL    = Color(0.0f, 0.5f, 0.5f, 1.0f);
}