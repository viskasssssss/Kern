#version 330 core

in vec2 a_Position;
in vec2 a_UV;

out vec2 v_UV;

uniform vec2 u_Offset;

void main()
{
    vec2 pos = a_Position + u_Offset;
    gl_Position = vec4(pos, 0.0, 1.0);
    v_UV = a_UV;
}
