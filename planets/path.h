#ifndef PATH_H
#define PATH_H

#include "planets/orbit.h"

/**
 * @brief The Path class describes the absolute path a planet follows
 */
class Path : public Drawable
{
public:
    /**
     * @brief Path constructor
     * @param name the name of the path
     */
    Path(std::string name = "UNKNOWN PATH");

    /**
     * @see Drawable::draw(glm::mat4)
     */
    virtual void draw(glm::mat4 projection_matrix) const override;

    /**
     * @see Drawable::createObject()
     */
    virtual void createObject() override;

    /**
     * @see Drawable::update(float, glm::mat4)
     */
    virtual void update(float elapsedTimeMs, glm::mat4 modelViewMatrix);

    /**
     * @brief addPosition Adds a position to the path
     * @param position The position to add
     */
    virtual void addPosition(glm::vec3 position);

protected:

    // Hint: All paths (probably) use the same model view matrix.
    // You could use a static variable.

    /**
     * @see Drawable::getVertexShader()
     */
    virtual std::string getVertexShader() const;

    /**
     * @see Drawable::getFragmentShader()
     */
    virtual std::string getFragmentShader() const;
};

#endif // PATH_H
