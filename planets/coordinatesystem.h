#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include "planets/drawable.h"

class CoordinateSystem : public Drawable
{
public:
    CoordinateSystem(std::string name = "Coordinate System");

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
    virtual std::string getVertexShader() const;

    /**
     * @see Drawable::getFragmentShader()
     */
    virtual std::string getFragmentShader() const;

    /**
     * @see Drawable::createObject()
     */
    virtual void createObject();
};

#endif // COORDINATESYSTEM_H
