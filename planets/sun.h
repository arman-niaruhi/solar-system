#ifndef SUN_H
#define SUN_H

#include "planets/planet.h"

#include <glm/vec3.hpp>

class Sun : public Planet
{
public:
    Sun(std::string name = "Sun",
        float radius = 1.f,
        float distance = 1.f,
        float hoursPerDay = 250.f,
        float daysPerYear = 365.f,
        std::string textureLocation = ":/res/images/sun.bmp");

    /**
     * @brief getPosition Getter for the position
     * @return the position in model view coordinates
     */
    glm::vec3 getPosition() const;

    /// TODO: add a draw mechanism

};

#endif // SUN_H
