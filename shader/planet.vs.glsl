#version 400
uniform mat4 projection_matrix; //uniform because it would be used in the other fragment shaders
uniform mat4 modelview_matrix;  //uniform because it would be used in the other fragment shaders

// get position from vertex array object
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;


// send color to fragment shader
out vec4 vpos;
out vec4 vnormal;
out vec2 vtexcoord;

void main(void)
{
    // calculate position and normals in model view projection space
       vpos = modelview_matrix * vec4(aPos, 1.0f);
       vnormal = inverse(transpose(modelview_matrix)) * vec4(aNormal, 0.0f);

       gl_Position = projection_matrix * vpos;

       //send texture coordinates to fragment shader
       vtexcoord = aTex;

}

