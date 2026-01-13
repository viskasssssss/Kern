#version 330 core

uniform sampler2D u_Texture;

in vec2 v_UV;

out vec4 fragColor;

void main()
{
    fragColor = texture(u_Texture, v_UV);
}
