#version 330

in vec2 position;
in vec2 color;

out vec2 fragColor;

void main(void)
{
    gl_Position = position;
    fragColor = color;
}
