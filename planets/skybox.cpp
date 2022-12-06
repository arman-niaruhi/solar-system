#include <GL/glew.h>

#include "skybox.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glbase/gltool.hpp"
#include <image/image.h>



Skybox::Skybox(std::string name): Drawable(name)
{
}

void Skybox::init()
{
    /// TODO: your code here
    Drawable::init();
    TextureID = loadTexture();
}

void Skybox::draw(glm::mat4 projection_matrix) const
{
    /// TODO: your code here
    if (_program == 0) {
        std::cerr << "Planet" << _name << "not initialized. Call init() first." << std::endl;
        return;
    }


    // Load program
    glUseProgram(_program);

    //help to put everything beyond our cube
    glDepthMask(GL_FALSE);

    // bind VAO and texture
    glBindVertexArray(_vertexArrayObject);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

    // set parameter
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));
    glUniform4fv(glGetUniformLocation(_program, "pos123"), 1,&(_modelViewMatrix[3].x));

    //select the number of vertecies = 6 side X 2 triangels * 3 = 36
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    glDepthMask(GL_TRUE);
    // check for errors
    VERIFY(CG::checkError());

}

void Skybox::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    _modelViewMatrix = modelViewMatrix;
}

std::string Skybox::getVertexShader() const
{
    return Drawable::loadShaderFile(":/shader/skybox.vs.glsl");
}

std::string Skybox::getFragmentShader() const
{
    return Drawable::loadShaderFile(":/shader/skybox.fs.glsl");
}

GLuint Skybox::loadTexture()
{
    // Hint: you can use GL_TEXTURE_CUBE_MAP instead of default textures

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    Image img = Image(":/res/images/stars.bmp");
    int width = img.getWidth();
    int height = img.getHeight();
    uchar *data = img.getData();

    //A cubemap is a texture that contains 6 individual 2D textures that each form one side of a cube: a textured cube
    //more details => https://learnopengl.com/Advanced-OpenGL/Cubemaps
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    return  textureID;
}

void Skybox::createObject()
{
    float skyboxPositions[] = {
       //using the triangles

//-------------------------------------------------first Side
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
//------------------------------------------------- second Side
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
//------------------------------------------------- 3.Side
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
//------------------------------------------------- 4.Side
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
//------------------------------------------------- 5.Side
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
//------------------------------------------------- 6.Side
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,

         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    if (_vertexArrayObject == 0)
        glGenVertexArrays(1, &_vertexArrayObject);           // Erzeugt ID für Vertex Array Object
    glBindVertexArray(_vertexArrayObject);                    // Aktiviert VAO
    // Initialisiere Positionsdaten: 0-tes Attribut; 3D Koord. der Vertices
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);                        // Erzeuge Buffer ID
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);           // Aktiviere Buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxPositions), &skyboxPositions, GL_STATIC_DRAW);     // Erzeuge VBO Speicher & kopiere Daten
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);     // Location (=0) und Format (3 floats  pro Vertex) festlegen
    glEnableVertexAttribArray(0);                               // aktiviere 0-tes VAO Attribut

    VERIFY(CG::checkError());


}

