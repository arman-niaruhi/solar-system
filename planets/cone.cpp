#include <GL/glew.h>

#include "cone.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>


#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glbase/gltool.hpp"

#include "gui/config.h"
int _numInd;

Cone::Cone(std::string name, float distance):
    Drawable(name),
    _distance(distance), _angle(.0f)
{

}

void Cone::draw(glm::mat4 projection_matrix) const
{
    /// TODO: your code here

    // Error handling
    if(_program == 0){
        std::cerr << "Cone is not working" << std::endl;}
    // Load program if program != 0
    glUseProgram(_program);

    // bin vertex array object
    glBindVertexArray(_vertexArrayObject);

    // set parameter matrices
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));

    // call draw
    glDrawElements(GL_TRIANGLES, _numInd, GL_UNSIGNED_INT, 0);

    // unbind vertex array object
    glBindVertexArray(0);
}

void Cone::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    /// TODO: your code here

    float lengthOfLaser;
    std::stack<glm::mat4> modelViewstack;

    modelViewstack.push(modelViewMatrix);

        //we need here just a rotation and scale

        // rotate cone so the top is at the edge of the deathstar
        modelViewstack.top() = glm::rotate(modelViewstack.top(), glm::radians(Config::angleOfLaser * 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // add scaling to the stack
        modelViewstack.top() = glm::scale(modelViewstack.top(), glm::vec3(0.05, Config::laser_length * 1.0f / 2.2, 0.05));

        _modelViewMatrix = glm::mat4(modelViewstack.top());

        modelViewstack.pop();
}

std::string Cone::getVertexShader() const
{
    /// TODO: your code here
    return Drawable::loadShaderFile(":/shader/path.vs.glsl");
}

std::string Cone::getFragmentShader() const
{
    /// TODO: your code here
    return Drawable::loadShaderFile(":/shader/path.fs.glsl");
}

void Cone::createObject()
{
    /// TODO: your code here
    std::vector<glm::vec3> positions;
    std::vector<unsigned int> indices;

    float a = (2.0f * M_PI) / Config::u;    // circumference OF CIRCLE DIVIDED by resolution
    for (int phi = 0; phi < Config::u; phi++) {
        float phis = a *phi;

        //circle
        positions.push_back(glm::vec3(sin(phis), 0.0f, cos(phis)));
    }
    //top of cone
    positions.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    //indices
    for(int i = 0; i < Config::u; i++){
        indices.push_back( i );
        indices.push_back( i+1 );
        indices.push_back( Config::u );
    }

    // Set up a vertex array object for the geometry
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

    //add textures


    // save number of elements for draw
    _numInd = indices.size();
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // unbind vertex array object
    glBindVertexArray(0);
    // delete buffers (the data is stored in the vertex array object)
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &index_buffer); 
}

glm::vec3 Cone::getDirection() const
{
    return _direction;
}

glm::vec3 Cone::getPosition() const
{
    return _position;
}

float Cone::getAngle() const
{
    return _angle;
}

