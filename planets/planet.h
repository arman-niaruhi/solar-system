#ifndef PLANET_H
#define PLANET_H

#include "planets/drawable.h"


#include <memory>
#include <vector>

class Orbit;
class Path;
class Sun;
class Cone;
class ring;

class Planet : public Drawable
{
public:
    Planet(std::string name = "UNNAMED PLANET",
            float radius = 1.0f,
            float distance = 10.0f,
            float hoursPerDay = 24.0f,
            unsigned int daysPerYear = 365,
            std::string textureLocation = ":/res/images/earth.bmp"
            );

    /**
     * @see Drawable::init()
     */
    virtual void init() override;

    /**
     * @see Drawable::recreate()
     */
    virtual void recreate() override;

    /**
     * @see Drawable::draw(glm::mat4)
     */
    virtual void draw(glm::mat4 projection_matrix) const override;

    /**
     * @see Drawable::update(float, glm::mat4)
     */
    virtual void update(float elapsedTimeMs, glm::mat4 modelViewMatrix) override;

    /**
     * @brief setLights setter for the light sources
     * @param sun the sun
     * @param laser the death star cone
     *
     * Hint: you probably only need to call this once.
     */
    virtual void setLights(std::shared_ptr<Sun> sun, std::shared_ptr<Cone> laser);

    /**
     * @brief addChild adds a child to the hierarchy
     * @param child the child to add
     */
    virtual void addChild(std::shared_ptr<Planet> child);

    /**
     * @brief calculatePath precalculates the path
     * @param modelViewMatrix the modelViewMatrix at which to calculate
     *
     * This method precalculates the absolute path of every planet
     * and its child planets. It simulates multiple update steps
     * and saves the respective position.
     *
     * Hint: This should only be called by the earth.
     */
    virtual void calculatePath(glm::mat4 modelViewMatrix);

    ~Planet();


protected:

    std::vector<std::shared_ptr<Planet>> _children; /**< All children that move around this planet */

    float _radius;      /**< the radius of the planet */
    float _distance;    /**< the distance between this planet and its parent*/

    unsigned int _daysPerYear;  /**< the number of days the planet needs for a full (global) rotation */
    
    float _globalRotation;        /**< the current global roation */
    float _globalRotationSpeed;  /**< tglobal speed around sun */

    float _localRotation;       /**< the current local roation */
    float _localRotationSpeed;  /**< the speed at which the planet spins */

    std::shared_ptr<Orbit> _orbit;  /**< the local orbit of this planet */
    std::shared_ptr<Path> _path;    /**< the global path of this orbit */

    std::shared_ptr<Sun> _sun;      /**< the sun that illuminates this planet */
    std::shared_ptr<Cone> _laser;   /**< the death star laser that could illuminate the planet */
    
     std::string _textureLocation;  /**< texture id of this planet and locaton of that */
     GLuint _texturID;               /**< texture id of this planet and locaton of that */

     std::vector<glm::vec3> _positions;
     std::vector<glm::vec2> _texcoords;
     std::vector<GLuint> _indices;
     std::vector<glm::vec3> _normals;
//     sphere planetKugel;   

    /**
     * @see Drawable::createObject()
     */
    virtual void createObject() override;

    /**
     * @see Drawable::getVertexShader()
     */
    virtual std::string getVertexShader() const override;

    /**
     * @see Drawable::getFragmentShader()
     */
    virtual std::string getFragmentShader() const override;

    /**
     * @brief getCommonYears calculates the number of days until two planets amde full rotations
     * @param other days per year of the other planet
     * @return days until both planets are at the same spot again
     *
     * The method calculates the least common multiple between this
     * planet's days per year and the days per year of another planet.
     * It also takes all children of this planet into account. The
     * result is a value in days at which both planets and this planet's
     * children are guaranteed to be at the same spot as where they were
     * when the method was called.
     *
     * Hint: You can rename this method if you can think of a better name.
     */
    unsigned int getCommonYears(unsigned int other);

    /**
     * @brief greatestCommonDivisor calcluates the GCD of two numbers
     * @param a first number
     * @param b second number
     * @return the greatest common divisor of the two numbers
     *
     * This is a helper method for getCommonYears()
     */
    unsigned int greatestCommonDivisor(unsigned int a, unsigned int b);

    /**
     * @brief updatePath updates the current position and saves a path point
     * @param elapsedTimeMs the number of days that should be simulated
     *
     * This method should only be used when the path is
     * precalculated.
     */
    virtual void updatePath(float elapsedTimeMs, glm::mat4 modelViewMatrix);
    /**
     * @brief addPathPoint adds the current position to the path
     */
    virtual void addPathPoint();
    /**
     * @brief createPath creates the path of this planet (and its children)
     *
     * Hint: This should only be called once
     */
    virtual void createPath();
};

#endif // PLANET_H
