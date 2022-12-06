#version 400

// get color from vertex shader
in vec3 color;
in vec2 vtexcoord;

// send color to screen
out vec4 fcolor;

uniform sampler2D sun;

void main(void)
{
    // set fragment color
    fcolor = texture(sun, vtexcoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
