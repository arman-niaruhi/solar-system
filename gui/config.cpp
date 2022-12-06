#include "config.h"

/*
 *  Default values. Change them as needed.
 */

float Config::animationSpeed = 1.0f;

bool Config::localRotation = true;

bool Config::globalRotation = true;

bool Config::resetCam = false;

int Config::u = 45;

int Config::v = 45;

int Config::ring_res = 10;

float Config::height = 783.0f;

float Config::width = 691.0f;

bool Config::rotationCircle = true;

int Config::laser_length = 4;               //here is the default value of lenght that the laser has

bool Config::laserActive = true;

bool Config::gitter = false;

int Config::angleOfLaser = 90;

bool Config::lightOn = false;

glm::mat4 generalCameraMatrix = glm::mat4();
