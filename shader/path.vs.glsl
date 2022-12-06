#version 400

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform vec3 apos;

// get position from vertex array object
layout(location = 0) in vec3 pos;

// send color to fragment shader
out vec3 vcolor;

void main(void)
{
    // calculate position in model view projection space
    gl_Position = projection_matrix * modelview_matrix * vec4(pos, 1);
    // set red color
    vcolor = vec3(1, 0, 0);
}
