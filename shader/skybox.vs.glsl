#version 400
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform vec4 pos123;

void main()
{
    TexCoords = aPos;
    vec4 pos =  modelview_matrix * vec4(aPos, 1.0);
        pos.xyz-=pos123.xyz;
    gl_Position = projection_matrix * pos;
}
