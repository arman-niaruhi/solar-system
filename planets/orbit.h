#ifndef ORBIT_H
#define ORBIT_H

#include "planets/drawable.h"

/**
 * @brief The Orbit class describes the local orbit of a planet.
 */
class Orbit : public Drawable
{
public:
    /**
     * @brief Orbit constructor
     * @param name The name for the orbit
     * @param radius The radius of the orbit
     */
    Orbit(std::string name = "UNKNOWN ORBIT", float radius = 1.f);

public:
    /**
     * @see Drawable::draw(glm::mat4)
     */
    virtual void draw(glm::mat4 projection_matrix) const override;

    /**
     * @see Drawable::update(float, glm::mat4)
     */
    virtual void update(float elapsedTimeMs, glm::mat4 modelViewMatrix) override;

protected:
    /**
     * @see Drawable::getVertexShader()
     */
    virtual std::string getVertexShader() const override;

    /**
     * @see Drawable::getFragmentShader()
     */
    virtual std::string getFragmentShader() const override;

    /**
     * @see Drawable::createObject()
     */
    virtual void createObject() override;

    unsigned int _numVertices;
    float _radius;       /**< The radius of the orbit */
};

#endif // ORBIT_H
