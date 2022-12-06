#include <GL/glew.h>
#include "planet.h"

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>
#include <stack>

#include "glbase/gltool.hpp"

#include "gui/config.h"

#include "planets/cone.h"
#include "planets/sun.h"
#include "planets/orbit.h"
#include "planets/path.h"
#include "planets/sphere.h"
#include "image/image.h"

int amountOfIndices;

Planet::Planet(std::string name,
               float radius,
               float distance,
               float hoursPerDay,
               unsigned int daysPerYear,
               std::string textureLocation):
    Drawable(name),
    _radius(radius),
    _distance(distance),
    _localRotation(0),
    _localRotationSpeed(0),
    /// TODO: init global rotation parameters
    //added as parameters for the global movement
    _globalRotationSpeed(0),
    _globalRotation(0),
    _daysPerYear(daysPerYear),
    _textureLocation(textureLocation)

{
    _localRotationSpeed = 1.0f / hoursPerDay; // for local rotation:one step equals one hour
//    _globalRotationSpeed = 1.0f / hoursPerDay; // for local rotation:one step equals one hour
    _globalRotationSpeed = 1.0f / getCommonYears(daysPerYear);

    _orbit = std::make_shared<Orbit>(name + " Orbit", _distance);
    _path = std::make_shared<Path>(name + " Pfad");

}

void Planet::init()
{
    Drawable::init();

    /// TODO: load texture
    _texturID = Drawable::loadTexture(_textureLocation);

    /// TODO: init children, orbit and path
    for (auto& child : _children) {
            child->init();
        }

}

void Planet::recreate()
{
    Drawable::initShader();
}

void Planet::draw(glm::mat4 projection_matrix) const
{

    /// TODO: replace this with your code

    /// TODO: call draw on all drawables that belong to the planet  
    if(_program == 0){
        std::cerr << "Planet" << _name << "not initialized. Call init() first." << std::endl;
        return;
    }

    // Load program
    glUseProgram(_program);

    // bind vertex array object
    glBindVertexArray(_vertexArrayObject);

    // bind texture
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, _texturID);

   glm::vec3 sunlightPosition = _sun->getPosition();
   glUniform3fv(glGetUniformLocation(_program, "sunlightPosition"), 1, glm::value_ptr(sunlightPosition));
   glm::vec3 sunlightColor(1.0f, 1.0f, 1.0f);
   glUniform3fv(glGetUniformLocation(_program, "sunlightColor"), 1, glm::value_ptr(sunlightColor));
   float shininess = 2.0f;
   glUniform1f(glGetUniformLocation(_program, "shininess"), shininess);
   glm::vec3 ka(0.1f, 0.1f, 0.1f);
   glUniform3fv(glGetUniformLocation(_program, "ka"), 1, glm::value_ptr(ka));
   glm::vec3 kd(1.0f, 1.0f, 1.0f);
   glUniform3fv(glGetUniformLocation(_program, "kd"), 1, glm::value_ptr(kd));
   glm::vec3 ks(1.0f, 1.0f, 1.0f);
   glUniform3fv(glGetUniformLocation(_program, "ks"), 1, glm::value_ptr(ks));

    // set parameter
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));
    glUniform1i(glGetUniformLocation(_program, "planet"), 0);        // call the uniform sampler2D named planet in fragment shader

    glDrawElements(GL_TRIANGLES, amountOfIndices, GL_UNSIGNED_INT, 0);    // call draw of the geometries  using the triangles


    glBindVertexArray(0);

//    _orbit->draw(projection_matrix);
//    _path->draw(projection_matrix);

    //call draw on all drawables that belong to the this planet
    for (auto& child : _children) {
            child->draw(projection_matrix);
        }

    // check for errors
    VERIFY(CG::checkError());
}



void Planet::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{

    recreate();
    if (Config::globalRotation)
        _globalRotation += elapsedTimeMs * _localRotationSpeed * Config::animationSpeed;

    //keep rotation between 0 and 360
    while (_globalRotation >= 360.0f)
        _globalRotation -= 360.0f;
    while (_globalRotation < 0.0f)
        _globalRotation += 360.0f;

    // calculate new local rotation
    if (Config::localRotation)
        _localRotation += elapsedTimeMs * _localRotationSpeed * Config::animationSpeed;

    // keep rotation between 0 and 360
    while (_localRotation >= 360.f)
        _localRotation -= 360.0f;
    while (_localRotation < 0.0f)
        _localRotation += 360.0f;



    // apply local rotation to model view matrix
    // Hint: The stack is currently useless, but could be useful for you
    std::stack<glm::mat4> modelview_stack;

    modelview_stack.push(modelViewMatrix);

    //update orbit
    _orbit->update(elapsedTimeMs, modelview_stack.top());

    // rotate around y-axis for global rotation angle
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    //translate distance
    modelview_stack.top() = glm::translate(modelview_stack.top(), glm::vec3(_distance, 0.0f, 0.0f));

    //according to Übung 6 or 7 of the CG1 we have to come back to the normal position and then  continue to the add of next steps
    modelview_stack.push(modelview_stack.top());

    // rotate backwards to come in th first place /back to normal coordination)
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(-_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    // rotate around y-axis (local rotation)
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_localRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    //scale to planet size
    modelview_stack.top() = glm::scale(modelview_stack.top(), glm::vec3(_radius));

    _modelViewMatrix = glm::mat4(modelview_stack.top());       //create the Model matrix

    modelview_stack.pop();

    //update children
    for (auto child : _children)
        child->update(elapsedTimeMs, modelview_stack.top());

    modelview_stack.pop();
}

void Planet::setLights(std::shared_ptr<Sun> sun, std::shared_ptr<Cone> laser)
{
    _sun = sun;
    _laser = laser;
    for(auto child : _children)
        child->setLights(sun, laser);
}

void Planet::addChild(std::shared_ptr<Planet> child)
{
    _children.push_back(child);
}


void Planet::createObject(){

    // draw the geometries of the planets
    sphere planetKugel = sphere(_radius);
    _positions = planetKugel.getPositions();
    _texcoords = planetKugel.getCoord();
    _indices = planetKugel.getIndices();
    _normals = planetKugel.getNormal();

    // Set up a vertex array object for the geometry
    if(_vertexArrayObject == 0)                   // Erzeugt ID für Vertex Array Object
        glGenVertexArrays(1, &_vertexArrayObject);  // Aktiviert VAO
    glBindVertexArray(_vertexArrayObject);

     // Initialisiere Positionsdaten: 0-tes Attribut; 3D Koord. der Vertices
    // fill vertex array object with data
    GLuint position_buffer;                     // Aktiviere Buffer
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, _positions.size() * 3 * sizeof(float), _positions.data(), GL_STATIC_DRAW);   // Erzeuge VBO Speicher & kopiere Daten
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // Location (=0) und Format (3 floats  pro Vertex) festlegen
    glEnableVertexAttribArray(0);                              // aktiviere 0-tes VAO Attribut

    // Hint: the texture coordinates buffer is missing

    GLuint normal_buffer;
    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * 3 * sizeof(float), _normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint textur_buffer;                                       // Aktiviere Buffer
    glGenBuffers(1, &textur_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, textur_buffer);
    glBufferData(GL_ARRAY_BUFFER, _texcoords.size() * 2 * sizeof(float), _texcoords.data(), GL_STATIC_DRAW);
    //the first indices of next two one should be the same
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, 0);       // Location (=1) und Format (2 floats  pro Vertex) festlegen
    glEnableVertexAttribArray(2);                               // aktiviere 0-tes VAO Attribut


    //store the order of the correspondences indices and the vertecies
    amountOfIndices = _indices.size();
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);


    // unbind vertex array object
    glBindVertexArray(0);
//    glBindVertexArray(1);
    glBindVertexArray(1);

    // delete buffers (the data is stored in the vertex array object)
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &textur_buffer);

    // check for errors
    VERIFY(CG::checkError());

    // Hint: save the number of vertices for drawing

}

std::string Planet::getVertexShader() const
{
    return Drawable::loadShaderFile(":/shader/planet.vs.glsl");

}

std::string Planet::getFragmentShader() const
{
    if ( _name == "Sonne" || _name == "Mond" )
        return Drawable::loadShaderFile(":/shader/sun.fs.glsl");
    if (Config::lightOn)
        return Drawable::loadShaderFile(":/shader/planet.fs.glsl");
    else
        return Drawable::loadShaderFile(":/shader/sun.fs.glsl");
}

Planet::~Planet(){
}


/*************************************************
 * Hint: You don't need to change the code below *
 *************************************************/

void Planet::calculatePath(glm::mat4 modelViewMatrix)
{
    // Hint: if you have some time left you can optimize this method
    for(auto child : _children){
        unsigned int longestCommonMultiple = child->getCommonYears(_daysPerYear);
        for(unsigned int i = 0; i <= longestCommonMultiple; i++){
            float step = 360.0f * _daysPerYear / Config::animationSpeed;
            child->updatePath(step, modelViewMatrix);
        }
    }
     createPath();
}


unsigned int Planet::getCommonYears(unsigned int other)
{
    unsigned int tmp = other * _daysPerYear / greatestCommonDivisor(other, _daysPerYear);
    unsigned int result = tmp;
    for(auto child : _children){
        result = std::max(result, child->getCommonYears(tmp));
    }
    return result;
}

unsigned int Planet::greatestCommonDivisor(unsigned int a, unsigned int b)
{
    if (b == 0)
        return a;
    else
        return greatestCommonDivisor(b, a % b);
}

void Planet::updatePath(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    update(elapsedTimeMs, modelViewMatrix);
    addPathPoint();
}

void Planet::addPathPoint(){
    _path->addPosition(glm::vec3(_modelViewMatrix * glm::vec4(0,0,0,1)));
    for(auto child : _children)
        child->addPathPoint();
}

void Planet::createPath(){
    _path->recreate();
    for(auto child : _children)
        child->createPath();
}



