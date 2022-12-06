#ifndef CONFIG_H
#define CONFIG_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>


/**
 * @brief The global configuration
 *
 * This class contains global configuration parameters
 * that are used all over the program.
 *
 * Hint: Add as many parameters as you need.
 */
class Config{
public:

    static float animationSpeed;     /**< the number of steps per ms */

    static bool localRotation;       /**< rotate the planets locally */

    static bool globalRotation;        /**< rotate the planets globally */

    static bool resetCam;             /**< Rest the camera */

    static int u,v,ring_res;         /**< change the resolution */

    static float height,width;        /**< save the resolution */

    static bool rotationCircle;

    static int laser_length ;        /**< change the length of laser */

    static bool laserActive;          /**< set up of laser activation */

    static int angleOfLaser;           /**< change the laser angle */

    static bool gitter;

    static bool lightOn;

    static glm::mat4 generalCameraMatrix;

};

#endif // CONFIG_H
