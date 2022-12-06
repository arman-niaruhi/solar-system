#ifndef CONE_H
#define CONE_H

#include "planets/drawable.h"

#include <glm/vec3.hpp>

/**
 * @brief The Cone class handles a cone object
 *
 * The cone object should belong to a death star and
 * represents its laser
 */
class Cone : public Drawable
{
public:
    /**
     * @brief Cone constructor
     * @param name the name for the cone
     * @param distance the distance between a planets core and its surface
     */
    Cone(std::string name, float distance);

public:
    /**
     * @see Drawable::draw
     */
    virtual void draw(glm::mat4 projection_matrix) const override;

    /**
     * @see Drawable::update
     */
    virtual void update(float elapsedTimeMs, glm::mat4 modelViewMatrix) override;

    /**
     * @brief getAngle getter for the angle of the cone
     * @return the angle of the cone in degrees
     *
     * The angle is computed from the cone's length and radius
     */
    float getAngle() const;

    /**
     * @brief getPosition getter for cone's position
     * @return the cone's position in model view space
     */
    glm::vec3 getPosition() const;

    /**
     * @brief getDirection getter for the cone's direction
     * @return the cone's direction in model view space
     */
    glm::vec3 getDirection() const;

protected:

    /**
     * @see Drawable::getVertexShader
     */
    virtual std::string getVertexShader() const override;

    /**
     * @see Drawable::getFragmentShader
     */
    virtual std::string getFragmentShader() const override;

    /**
     * @see Drawable::createObject
     */
    virtual void createObject() override;

    float _distance;        /**< the distance between planet's core and surface */
    float _angle = 0.45f;           /**< angle between direction and side */
    glm::vec3 _position;    /**< current position in model view space */
    glm::vec3 _direction;   /**< direction in model view space */
    unsigned int _numVertices; /**< stores the number of vertices used by the cone */
};

#endif // CONE_H
