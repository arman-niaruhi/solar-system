#ifndef SATURN_H
#define SATURN_H

#include "planets/planet.h"

class Ring;

class Saturn : public Planet
{
public:
    Saturn(std::string name = "Saturn",
        float radius = 1.0f,
        float distance = 10.0f,
        float hoursPerDay = 24.0f,
        float daysPerYear = 365.0f,
        std::string textureLocation = ":/res/images/moon.bmp");

    /**
     * @see Drawable::init()
     */
    virtual void init() override;

    /**
     * @see Drawable::recreate()
     */
    virtual void recreate() override;

    /**
     * @see Drawable::update(float, glm::mat4)
     */
    virtual void update(float elapsedTimeMs, glm::mat4 modelViewMatrix) override;

    /**
     * @see Drawable::draw(glm::mat4)
     */
    virtual void draw(glm::mat4 projection_matrix) const override;

    /**
     * @brief cone getter for the cone (laser)
     * @return the cone
     */
    std::shared_ptr<Ring> ring() const;

protected:
    std::shared_ptr<Ring> _ring;    /**< a ring, representing saturn's ring */
};

#endif // Saturn_H
