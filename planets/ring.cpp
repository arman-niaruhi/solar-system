#include <GL/glew.h>

#define GLM_SWIZZLE

#include "ring.h"

#include <vector>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glbase/gltool.hpp"

#include "gui/config.h"

#include "planets/cone.h"
#include "planets/sun.h"
#include "planets/orbit.h"

#include <algorithm>
#include <iostream>
#include <stack>

#define GLM_FORCE_RADIANS

#ifndef M_PI
#define M_PI 3.14159265359
#endif

Ring::Ring(std::string name, float radius, float distance, float hoursPerDay, float daysPerYear, std::string textureLocation) :
    Drawable(name),
    _radius(radius),
    _distance(distance),
    _localRotation(0),
    _localRotationSpeed(0),
    _daysPerYear(daysPerYear),

    //own
    _textureLocation(textureLocation),
    _globalRotation(0),
    _globalRotationSpeed(0)
{
    _globalRotationSpeed = 1.0f / daysPerYear; // for global rotation: one step equals one day

    _localRotationSpeed = 1.0f / hoursPerDay; // for local rotation:one step equals one hour
}

void Ring::init()
{
    Drawable::init();
    _textureID = Drawable::loadTexture(":/res/images/ring.bmp");
}

void Ring::recreate()
{
    createObject();
}

void Ring::draw(glm::mat4 projection_matrix) const
{
    if (_program == 0) {
        std::cerr << "Planet" << _name << "not initialized. Call init() first." << std::endl;
        return;
    }

    // Load program
    glUseProgram(_program);

    glEnable(GL_DEPTH_TEST);

    // bin vertex array object
    glBindVertexArray(_vertexArrayObject);

    // set parameter matrices
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glUniform1i(glGetUniformLocation(_program, "tx"), 0);

    glDrawElements(GL_TRIANGLES, _amountElements, GL_UNSIGNED_INT, 0);    // call draw of the geometries  using the triangles

    glBindVertexArray(0);
}

void Ring::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{

        recreate();

    // calculate new local rotation
    if (Config::localRotation)
        _localRotation += elapsedTimeMs * _localRotationSpeed * Config::animationSpeed;

    // keep rotation between 0 and 360
    while (_localRotation >= 360.f)
        _localRotation -= 360.0f;
    while (_localRotation < 0.0f)
        _localRotation += 360.0f;

    // calculate new global rotation
    if (Config::globalRotation)
        _globalRotation += elapsedTimeMs * _globalRotationSpeed * Config::animationSpeed;

    //keep rotation between 0 and 360
    while (_globalRotation >= 360.0f)
        _globalRotation -= 360.0f;
    while (_globalRotation < 0.0f)
        _globalRotation += 360.0f;


    // apply global rotation to model view matrix
    // Hint: The stack is currently useless, but could be useful for you
    std::stack<glm::mat4> modelview_stack;

    modelview_stack.push(modelViewMatrix);

    // rotate around y-axis for global rotation angle
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    //translate distance
    modelview_stack.top() = glm::translate(modelview_stack.top(), glm::vec3(_distance, 0.0f, 0.0f));

    // rotate backwards to compensate rotation
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(-_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    // rotate around y-axis (local rotation)
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_localRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    //scale to planet size
    modelview_stack.top() = glm::scale(modelview_stack.top(), glm::vec3(_radius, _radius, _radius));

    _modelViewMatrix = glm::mat4(modelview_stack.top());

    modelview_stack.pop();
}

void Ring::setSun(std::shared_ptr<Sun> sun)
{
    _sun = sun;
}

void Ring::setLaser(std::shared_ptr<Cone> laser)
{
    _laser = laser;
}

std::string Ring::getVertexShader() const
{
    return Drawable::loadShaderFile(":/shader/planet.vs.glsl");
}

std::string Ring::getFragmentShader() const
{
    return Drawable::loadShaderFile(":/shader/sun.fs.glsl");
}

void Ring::createObject()
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<unsigned int> indices;

    int res_u = Config::u;
    float a = (2.0f * M_PI) / res_u;
    int x = res_u + 1;
    float epsilon = 0.0075f;

    //TODO: Create Ring
    for (int h = 0; h <= res_u; h++) {
        float phi = a * h;

        //Top inner circle
        positions.push_back(glm::vec3(sin(phi), 0.0f, -cos(phi)));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        texcoords.push_back(glm::vec2(0.0f, (1.0f * h) / res_u));
    }

    for (int h = 0; h <= res_u; h++) {
        float phi = a * h;

        //Top outer circle
        positions.push_back(glm::vec3(sin(phi) * 2.0f, 0.0f, -cos(phi) * 2.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        texcoords.push_back(glm::vec2(1.0f, (1.0f * h) / res_u));
    }

    for (int h = 0; h <= res_u; h++) {
        float phi = a * h;

        //Bottom inner circle
        positions.push_back(glm::vec3(sin(phi), -epsilon, -cos(phi)));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        texcoords.push_back(glm::vec2(0.0f, (1.0f * h) / res_u));
    }

    for (int h = 0; h <= res_u; h++) {
        float phi = a * h;

        //Bottom outer circle
        positions.push_back(glm::vec3(sin(phi) * 2.0f, -epsilon, -cos(phi) * 2.0f));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        texcoords.push_back(glm::vec2(1.0f, (1.0f * h) / res_u));
    }

    //indices

    for (int h = 0; h < res_u; h++) {

        //connect top inner / outer circle
        indices.push_back(x + h);
        indices.push_back(x + h + 1);
        indices.push_back(h + 1);

        indices.push_back(h + 1);
        indices.push_back(h);
        indices.push_back(x + h);

        //connect bottom inner / outer circle
        indices.push_back(2.0f * x + h);
        indices.push_back(2.0f * x + h + 1);
        indices.push_back(3.0f * x + h);

        indices.push_back(2.0f * x + h + 1);
        indices.push_back(3.0f * x + h + 1);
        indices.push_back(3.0f * x + h);


        //connect top and bottom circle at inner circle
        indices.push_back(h);
        indices.push_back(h + 1);
        indices.push_back(2 * x + h + 1);

        indices.push_back(2 * x + h + 1);
        indices.push_back(2 * x + h);
        indices.push_back(h);

        //connect top and bottom circle at outer circle
        indices.push_back(x + h + 1);
        indices.push_back(x + h);
        indices.push_back(3 * x + h);

        indices.push_back(3 * x + h);
        indices.push_back(3 * x + h + 1);
        indices.push_back(x + h + 1);

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


    // initialize normal buffer
    GLuint normal_buffer;
    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(1);


    // initialize texture coordinates buffer
    GLuint texture_buffer;
    glGenBuffers(1, &texture_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * 2 * sizeof(float), texcoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(2);



    // Hint: the texture coordinates buffer is missing

    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // unbind vertex array object
    glBindVertexArray(0);
    // delete buffers (the data is stored in the vertex array object)
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteBuffers(1, &texture_buffer);
    glDeleteBuffers(1, &index_buffer);

    //save number of elements for draw
    _amountElements = indices.size();
}
