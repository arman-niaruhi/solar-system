#include <GL/glew.h>

#include "orbit.h"
#include "planet.h"
#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glbase/gltool.hpp"

#include "gui/config.h"
#include <stack>
using namespace glm;
Orbit::Orbit(std::string name, float radius):
    Drawable(name),
    _radius(radius)
{

}

void Orbit::draw(glm::mat4 projection_matrix) const
{
    /// TODO: your code here
    if (_program == 0) {
        std::cerr << "Planet" << _name << "not initialized. Call init() first." << std::endl;
        return;
    }
    glUseProgram(_program);

    // bin vertex array object
    glBindVertexArray(_vertexArrayObject);

    // set parameter
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));
    
    glDrawElements(GL_LINE_STRIP, _numVertices, GL_UNSIGNED_INT, 0);
   
    glBindVertexArray(0);
    VERIFY(CG::checkError());



} 

void Orbit::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    /// TODO: your code here
}

std::string Orbit::getVertexShader() const
{
   /// TODO: your code here
    //my code
    return Drawable::loadShaderFile(":/shader/orbit.vs.glsl");
   return "";
}

std::string Orbit::getFragmentShader() const
{
    /// TODO: your code here
    //my code
    return Drawable::loadShaderFile(":/shader/orbit.fs.glsl");
    return "";
}
void create_orbit(
    std::vector<vec3>& positions,
    std::vector<vec3>& normals,
    std::vector<unsigned int>& indices,
    int slices,
    float radius){

    assert(slices >= 4);

    positions.clear();
    normals.clear();
    indices.clear();
    
    indices.clear();
    for (int i = 0; i <= slices; i++) {
        float tx = static_cast<float>(i) / slices;
        float lat = tx * 2 * pi<float>();
        float sinlat = sin(lat);
        float coslat = cos(lat);
        
        vec3 pos = vec3(radius*sinlat , 0.0f, radius*coslat);
        positions.push_back(pos);
        normals.push_back(pos);

        if(i<slices){
        indices.push_back(i);
        indices.push_back(i + 1);
        }
    }

}
void Orbit::createObject()
{
   /// TODO: your code here
   // my code
    std::vector<glm::vec3> positions;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
    int  slices = 32;
    
    create_orbit(
        positions,
        normals,
        indices,
        slices,
        _radius);
    _numVertices = indices.size();

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
