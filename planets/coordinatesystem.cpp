#include <GL/glew.h>

#include "coordinatesystem.h"

#include <vector>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glbase/gltool.hpp"

#include "gui/config.h"
using namespace glm;

CoordinateSystem::CoordinateSystem(std::string name) :
    Drawable(name)
{

}

void CoordinateSystem::draw(glm::mat4 projection_matrix) const
{
    if (_program == 0) {
        std::cerr << "Planet" << _name << "not initialized. Call init() first." << std::endl;
        return;
    }
    // Load program
    glUseProgram(_program);

    // bin vertex array object
    glBindVertexArray(_vertexArrayObject);

    // set parameter
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));
    
    glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    VERIFY(CG::checkError());
}

void CoordinateSystem::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    /// TODO: your code here
}

std::string CoordinateSystem::getVertexShader() const
{
    /// TODO: your code here
     //my code
    return Drawable::loadShaderFile(":/shader/coordinatsystem.vs.glsl");
    return "";
}

std::string CoordinateSystem::getFragmentShader() const
{
    /// TODO: your code here
      //my code
    return Drawable::loadShaderFile(":/shader/coordinatsystem.fs.glsl");
    return "";
}

void CoordinateSystem::createObject()
{
    /// TODO: your code here
    std::vector<glm::vec3> positions;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
    float scalar = 1.0f;

    vec3 Origine = vec3(0.0f, 0.0f, 0.0f); //p0
    positions.push_back(Origine);
    vec3 xAchse = vec3(scalar, 0.0f, 0.0f);// p1
    positions.push_back(xAchse);
    vec3 yAchse = vec3(0.0f, scalar, 0.0f);// p2
    positions.push_back(yAchse);
    vec3 zAchse = vec3(0.0f, 0.0f, scalar);//p3
    positions.push_back(zAchse);

    //xachse
    indices.push_back(0);
    indices.push_back(1);
    // yAchse
    indices.push_back(0);
    indices.push_back(2);
    //zAchse
    indices.push_back(0);
    indices.push_back(3);

    if (_vertexArrayObject == 0)
        glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    // fill vertex array object with data
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * 3 * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Hint: the texture coordinates buffer is missing

    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // unbind vertex array object
    glBindVertexArray(0);
    // delete buffers (the data is stored in the vertex array object)
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &index_buffer);

    // check for errors
    VERIFY(CG::checkError());
}
