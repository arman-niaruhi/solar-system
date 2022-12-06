#include <GL/glew.h>
#define GLM_SWIZZLE
#include "sun.h"

#include "planets/sphere.h"

#include "glbase/gltool.hpp"

Sun::Sun(std::string name, float radius, float distance, float hoursPerDay, float daysPerYear, std::string textureLocation):
    Planet(name, radius, distance, hoursPerDay, daysPerYear, textureLocation)
{

}

glm::vec3 Sun::getPosition() const
{
    /// TODO: return the position of the sun in model view coordinates

    // Hint: the model view matrix is part of the super class
    return (_modelViewMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz();
}

