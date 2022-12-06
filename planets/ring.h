#ifndef RING_H
#define RING_H

#include "planets/drawable.h"

#include <glm/vec3.hpp>

#include <memory>
#include <vector>

class Sun;
class Cone;

class Ring : public Drawable
{
public:
    Ring(std::string name = "Ring",
        float radius = 1.f,
        float distance = 1.f,
        float hoursPerDay = 250.f,
        float daysPerYear = 365.f,
        std::string textureLocation = ":/res/images/ring.bmp");

    /**
     * @see Drawable::init()
     */
    virtual void init() override;

    /**
     * @see Drawable::recreate()
     */
    virtual void recreate() override;

    /**
     * @see Drawable::draw()
     */
    virtual void draw(glm::mat4 projection_matrix) const;

    /**
     * @see Drawable::update
     */
    virtual void update(float elapsedTimeMs, glm::mat4 modelViewMatrix) override;

    void setSun(std::shared_ptr<Sun> sun);

    void setLaser(std::shared_ptr<Cone> laser);

protected:
    float _radius;      /**< the radius of the planet */
    float _distance;    /**< the distance between this planet and its parent*/

    unsigned int _daysPerYear;  /**< the number of days the planet needs for a full (global) rotation */

    float _localRotation;       /**< the current local roation */
    float _localRotationSpeed;  /**< the speed at which the planet spins */

    std::shared_ptr<Sun> _sun;      /**< the sun that illuminates this planet */
    std::shared_ptr<Cone> _laser;   /**< the death star laser that could illuminate the planet */

    std::string _textureLocation;		/**< path to texture */
    GLuint _textureID;					/**< texture ID for shader */
    float _globalRotation;				/**< the current global rotation */
    float _globalRotationSpeed;			/**< the speed at which the planet spins around it's parrent */
    unsigned int _amountElements;		/**< number of elements for draw */



    /**
     * @see Drawable::getVertexShader
     */
    virtual std::string getVertexShader() const override;

    /**
     * @see Drawable::getFragmentShader
     */
    virtual std::string getFragmentShader() const override;

    /**
     * @see Drawable::createObject()
     */
    virtual void createObject() override;

};

#endif // RING_H
