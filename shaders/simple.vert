#version 330

in vec2 position;
in vec3 color;

out vec3 fragColor;

void main(void)
{
    gl_PointSize = 50.0f;
    gl_Position = vec4(position, 0.0f, 1.0f);
    fragColor = color;
}
