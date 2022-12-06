#include <GL/glew.h>
#include "path.h"

#include <glm/gtc/type_ptr.hpp>

#include "glbase/gltool.hpp"

#include "gui/config.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <glm/gtx/transform.hpp>

int _amountIndices;
std::vector<glm::vec3> _pos;

Path::Path(std::string name):
    Drawable(name)
{

}

void Path::draw(glm::mat4 projection_matrix) const
{
    /// TODO: your code here
    glUseProgram(_program);

    glEnable(GL_DEPTH_TEST);

    // bin vertex array object
    glBindVertexArray(_vertexArrayObject);

    // set parameter matrices
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));

    //call draw
    glDrawElements(GL_LINE_STRIP, _amountIndices, GL_UNSIGNED_INT, 0);

    // unbind vertex array object
    glBindVertexArray(0);
}

void Path::createObject()
{
    /// TODO: your code her

   // Set up a vertex array object for the geometry
   if (_vertexArrayObject == 0)
       glGenVertexArrays(1, &_vertexArrayObject);
   glBindVertexArray(_vertexArrayObject);

   // fill vertex array object with data
//   GLuint position_buffer;
//   glGenBuffers(1, &position_buffer);
//   glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
//   glBufferData(GL_ARRAY_BUFFER, _pos.size() * 3 * sizeof(float), _pos.data(), GL_STATIC_DRAW);
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//   glEnableVertexAttribArray(0);

//   GLuint index_buffer;
//   glGenBuffers(1, &index_buffer);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

//   // unbind vertex array object
//   glBindVertexArray(0);
//   // delete buffers (the data is stored in the vertex array object)
//   glDeleteBuffers(1, &position_buffer);
//   glDeleteBuffers(1, &index_buffer);

//   //save number of elements for draw
//   _amountIndices = indices.size();

   VERIFY(CG::checkError());
}

void Path::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    /// TODO: your code here
}

void Path::addPosition(glm::vec3 position)
{
    /// TODO: your code here
    _pos.push_back(position);
}

std::string Path::getVertexShader() const
{
    /// TODO: your code here
    return Drawable::loadShaderFile(":/shader/path.vs.glsl");
}
std::string Path::getFragmentShader() const
{
    /// TODO: your code here
    return Drawable::loadShaderFile(":/shader/path.fs.glsl");
}
