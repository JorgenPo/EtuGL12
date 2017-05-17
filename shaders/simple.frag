#version 330

in vec3 fragColor;
out vec4 finalColor;

void main() {
    finalColor = vec4(fragColor, 0.0f);
}
