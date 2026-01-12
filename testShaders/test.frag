#version 330 core

in vec2 v_UV;
in float v_Intensity;

out vec4 fragColor;

void main()
{
    fragColor = vec4(v_UV, v_Intensity, 1.0);
}
