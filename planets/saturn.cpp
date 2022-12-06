#include "saturn.h"

#include "planets/ring.h"

#include "gui/config.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <glm/gtx/transform.hpp>

#include "planets/orbit.h"
#include "planets/path.h"

#define GLM_FORCE_RADIANS

#ifndef M_PI
#define M_PI 3.14159265359
#endif

Saturn::Saturn(std::string name, float radius, float distance, float hoursPerDay, float daysPerYear, std::string textureLocation) :
    Planet(name, radius, distance, hoursPerDay, daysPerYear, textureLocation)
{
    _ring = std::make_shared<Ring>("Saturn Ring", 0.7f, 0.0f, hoursPerDay, daysPerYear, ":/res/images/ring.bmp");
    _orbit = std::make_shared<Orbit>(name + " Orbit", _distance);
    _path = std::make_shared<Path>(name + " Pfad");
}

void Saturn::init()
{
    /// TODO: your code here
    Planet::init();
    _ring->init();
}

void Saturn::recreate()
{
    /// TODO: your code here
    Planet::recreate();
    _ring->recreate();
}

void Saturn::update(float elapsedTimeMs, glm::mat4 modelViewMatrix)
{
    /// TODO: your code here

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

    //update orbit and path
    _orbit->update(elapsedTimeMs, modelview_stack.top());
    _path->update(elapsedTimeMs, modelview_stack.top());


    // rotate around y-axis for global rotation angle
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    //translate distance
    modelview_stack.top() = glm::translate(modelview_stack.top(), glm::vec3(_distance, 0.0f, 0.0f));

    // rotate backwards to compensate rotation
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(-_globalRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    // rotate around y-axis (local rotation)
    modelview_stack.top() = glm::rotate(modelview_stack.top(), glm::radians(_localRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    //save checkpoint for cone
    modelview_stack.push(modelview_stack.top());

    //scale to planet size
    modelview_stack.top() = glm::scale(modelview_stack.top(), glm::vec3(_radius, _radius, _radius));

    _modelViewMatrix = glm::mat4(modelview_stack.top());

    modelview_stack.pop();

    //update cone
    _ring->setSun(_sun);
    _ring->setLaser(_laser);
    _ring->update(elapsedTimeMs, modelview_stack.top());

    modelview_stack.pop();
}

void Saturn::draw(glm::mat4 projection_matrix) const
{
    /// TODO: your code here
    Planet::draw(projection_matrix);
    _ring->draw(projection_matrix);
}

std::shared_ptr<Ring> Saturn::ring() const
{
    /// TODO: your code here
    return _ring;
}
