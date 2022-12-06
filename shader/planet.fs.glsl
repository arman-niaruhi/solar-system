#version 400

// get color, normals and texture coordinates from vertex shader
in vec4 vpos;
in vec4 vnormal;
in vec2 vtexcoord;

uniform sampler2D planet;

uniform vec3 sunlightPosition;
uniform vec3 sunlightColor;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess; // is called 'n' on the CG1 slides

// send color to screen
layout(location = 0) out vec4 fcolor;

void main(void)
{

    // Normalize input from the vertex shader
   vec3 n = normalize(vnormal.xyz); //normal
   vec3 l = normalize(sunlightPosition - vpos.xyz); //light direction
   vec3 v = normalize(-vpos.xyz); //viewing vector

   // reflected view
   vec3 r = reflect(-l, n);

   //ambient light
   vec3 ambient = ka * sunlightColor;

   //diffuse light
   vec3 diffuse = kd * sunlightColor * max(dot(l, n), 0.0);

   //specular ligth
   vec3 specular = ks * sunlightColor * pow(max(dot(r, v), 0.0), shininess);


    vec3  Result = ambient + diffuse + specular;
    // set fragment color
    fcolor = texture(planet, vtexcoord) * vec4(Result, 0.0f);
}



