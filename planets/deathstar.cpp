#include "deathstar.h"

#include "planets/cone.h"

#include "gui/config.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <glm/gtx/transform.hpp>


//this Star is maden with two types of geometrie : Sphere and also cone

DeathStar::DeathStar(std::string name, float radius, float distance, float hoursPerDay, float daysPerYear, std::string textureLocation):
    Planet(name, radius, distance, hoursPerDay, daysPerYear, textureLocation)
{
    _cone = std::make_shared<Cone>("Death Ray", radius);
}

void DeathStar::init()
{
    /// TODO: your code here
    Planet::init();
    if( Config::laserActive)
        _cone->init();
}

void DeathStar::recreate()
{
}

void DeathStar::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    /// TODO: your code here

    // calculate new global rotation
    if (Config::globalRotation)
        _globalRotation += elapsedTimeMs * _globalRotationSpeed * Config::animationSpeed;

    //keep rotation between 0 and 360
    while (_globalRotation >= 360.0f)
        _globalRotation -= 360.0f;
    while (_globalRotation < 0.0f)
        _globalRotation += 360.0f;

    // calculate new global rotation
    if (Config::localRotation)
        _localRotation += elapsedTimeMs * _localRotationSpeed * Config::animationSpeed;

    // keep rotation between 0 and 360
    while (_localRotation >= 360.f)
        _localRotation -= 360.0f;
    while (_localRotation < 0.0f)
        _localRotation += 360.0f;


    std::stack<glm::mat4> modelview_stack;

    modelview_stack.push(modelViewMatrix);

    // rotate around y-axis for global rotation angle
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    modelview_stack.top() = glm::translate(modelview_stack.top(), glm::vec3(_distance, 0.0f, 0.0f));

    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(-_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    modelview_stack.push(glm::rotate(modelview_stack.top(), glm::radians(_localRotation), glm::vec3(0.0f, 1.0f, 0.0f)));

    //scale to planet size
    modelview_stack.push(glm::scale(modelview_stack.top(), glm::vec3(_radius)));

    _modelViewMatrix = glm::mat4(modelview_stack.top());

    modelview_stack.pop();

    //update cone
    if( Config::laserActive)
        _cone->update(elapsedTimeMs, modelview_stack.top());

    modelview_stack.pop();
}

void DeathStar::draw(glm::mat4 projection_matrix) const
{
    Planet::draw(projection_matrix);
    if( Config::laserActive)
        _cone->draw(projection_matrix);
}

std::shared_ptr<Cone> DeathStar::cone() const
{
    /// TODO: your code here
    return _cone;
}


