/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#include <iostream>
#include <GL/glew.h>
#include <string>

using namespace std;

#include "glwidget.hpp"

#include <QMouseEvent>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>

#include "gui/config.h"
#include "gui/mainwindow.hpp"
#include "planets/coordinatesystem.h"
#include "planets/deathstar.h"
#include "planets/planet.h"
#include "planets/sun.h"
#include "planets/skybox.h"
#include "planets/ring.h"
#include "planets/saturn.h"

float mouseX = 0.0f;                                    //for find the position of the mouse on the window( widget )
float mouseY = 0.0f;

//here the methode, which is used => A Novel Camera Calibration Method Based on Polar Coordinate
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 7.0f); //camera-position in polar coordinates on a sphere: x = angle phi, y = angle theta, z = radius as global variable
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);       //This should be always (0,1,0) because it is in direction of y aches
glm::vec3 frontVector = glm::vec3(0.0f, 0.0f, 1.0f);    // alway in direction of -z aches
glm::vec3 curentCameraCoordinates;
                           // We went from Camera Space (all vertices defined relatively to the camera) to Homogeneous Space (all vertices defined in a small cube.
                                                        //Everything inside the cube is onscreen). But first of all we need a view mat

float cameraSpeed = 0.001f;
float zoomSpeed = 0.005f;

GLWidget::GLWidget(QWidget *&parent) : QOpenGLWidget(parent),//static_cast<QWidget*>(0)),
    _updateTimer(this), _stopWatch()
{
    // update the scene periodically
    QObject::connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(animateGL()));
    _updateTimer.start(18);
    _stopWatch.start();


    // create all drawable elements
    _skybox = std::make_shared<Skybox>("Skybox");
    _coordSystem = std::make_shared<CoordinateSystem>("Coordinate system");

    /***************************
     * DO NOT CHANGE days/year *
     * *************************/                         // radius, distance, h/day, days/year
    _earth          = std::make_shared<Planet> ("Erde",     1.0,    0.0,    24.0,   1, ":/res/images/earth.bmp");
    auto moon       = std::make_shared<Planet>("Mond",      0.215,  2.0,    27.3,   27, ":/res/images/moon.bmp");
    auto sun        = std::make_shared<Sun>("Sonne",        1.2,    6.0,    50.0,   350, ":/res/images/sun.bmp");
    auto mercury    = std::make_shared<Planet>("Merkur",    0.34,   2.32,   1407.5, 150, ":/res/images/mercury.bmp");
    auto venus      = std::make_shared<Planet>("Venus",     0.34,   3.0,    2802.0, 100, ":/res/images/venus.bmp");

    auto mars       = std::make_shared<Planet>("Mars",      0.453,  10.6,   24.7,   700, ":/res/images/mars.bmp");
    auto jupiter    = std::make_shared<Planet>("Jupiter",   0.453,  13.32,  9.9,    3500, ":/res/images/jupiter.bmp");
    auto saturn     = std::make_shared<Saturn>("Saturn",    0.453,  15.92,  10.6,   10500, ":/res/images/saturn.bmp");

    // jupiter moons
    auto io         = std::make_shared<Planet>("Io",        0.036,  0.8,    10.6,   30, ":/res/images/moon.bmp");
    auto europa     = std::make_shared<Planet>("Europa",    0.031,  1.0,    10.6,   60, ":/res/images/moon.bmp");
    auto ganymede   = std::make_shared<Planet>("Ganymed",   0.052,  1.2,    10.6,   120, ":/res/images/moon.bmp");
    auto callisto   = std::make_shared<Planet>("Callisto",  0.048,  1.8,    10.6,   350, ":/res/images/moon.bmp");

    auto deathStar  = std::make_shared<DeathStar>("Todesstern", 0.315,  2.0,    27.3,    50, ":/res/images/moon.bmp");

   // auto saturnRing  = std::make_shared<ring>("Ring", 0.453,  15.92,  10.6,   10500, ":/res/images/ring.bmp");

    // create hierarchy
    _earth->addChild(moon);
    _earth->addChild(sun);

    sun->addChild(mercury);
    sun->addChild(venus);
    sun->addChild(mars);
    sun->addChild(jupiter);
    sun->addChild(saturn);
    mars->addChild(deathStar);

    jupiter->addChild(io);
    jupiter->addChild(europa);
    jupiter->addChild(ganymede);
    jupiter->addChild(callisto);

    _earth->setLights(sun, deathStar->cone());
   }

void GLWidget::show()
{
    QOpenGLWidget::show();

    // check for a valid context
    if (!isValid() || !context()->isValid() || context()->format().majorVersion() != 4) {
        QMessageBox::critical(this, "Error", "Cannot get a valid OpenGL 4 context.");
        exit(1);
    }
}

void GLWidget::initializeGL()
{
    /* Initialize OpenGL extensions */
    glewExperimental = GL_TRUE; // otherwise some function pointers are NULL...
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glGetError(); // clear a gl error produced by glewInit

    // make sure the context is current
    makeCurrent();

    /// TODO: Init all drawables here
    _skybox->init();
    _earth->init();
}

void GLWidget::resizeGL(int width, int height)
{
    // update the viewport

    glViewport(0, 0, width, height);
    /// TODO: store the resolution in the config in case someone needs it
}

void GLWidget::paintGL()
{
    /// TODO: recreate the scene if needed
    // Render: set up view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

//   resizeGL(Config::width,Config::height);

//    Projection matrix : 45° Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 1.2f, 0.1f, 100.0f);                    //http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/#the-view-matrix

    if (Config::gitter) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

    /// TODO: call draw for all drawables
    _skybox->draw(projection_matrix);
    _earth->draw(projection_matrix);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
//    Config::resetCam = true;
    if (Config::resetCam == true){
        mouseX = 0;
        mouseY = 0;
        update();
    }else{
        mouseX = event->x();
        mouseY = event->y();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // ignore if it was not the left mouse button
    if (Config::resetCam == true){
        mouseX = 0;
        mouseY = 0;
        update();
    }else{
        mouseX = event->x();
        mouseY = event->y();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (Config::resetCam == true){
        mouseX = 0;
        mouseY = 0;
        Config::resetCam = false;
    }else
    {
        float mouseHorizontal = event->x() - mouseX;                 //Calculate the differents of mouse position between Press the mouse and Release it
        float mouseVertical = event->y() - mouseY;

        float phi = mouseHorizontal * cameraSpeed;                    //calculate the changes of phi and theta due to the changes in 2 Direction
        float theta = mouseVertical * cameraSpeed;

        //control the camera position if its get bigger than 2 * PI because we turn back to the first place

        cameraPosition = cameraPosition + glm::vec3(phi, theta, 0.0f);   //camera speed is just a scale here
        while (cameraPosition.x >= 2 * M_PI) {                           //control the camera position if its get bigger than 2 * PI because we turn back to the first place
            cameraPosition.x -= 2 * M_PI;
        }
        while (cameraPosition.x < 0) {
            cameraPosition.x += 2 * M_PI;
        }
        //handling 90 and -90 degrees for checking the changes in up and down direction
        if (cameraPosition.y > M_PI / 2.0f) {
            theta = M_PI / 2.0f - (cameraPosition.y - theta);
            cameraPosition.y = M_PI / 2.0f;
        }
        if (cameraPosition.y < -M_PI / 2.0f) {
            theta = -M_PI / 2.0f - (cameraPosition.y - theta);
            cameraPosition.y = -M_PI / 2.0f;
        }

        //rotating up and front vector according to cameramovement based on two different matrix
        upVector = (glm::rotate(-phi, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(upVector, 0.0f)).xyz();
        frontVector = (glm::rotate(-phi, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(frontVector, 0.0f)).xyz();

        glm::vec3 rightVector = glm::normalize(glm::cross(frontVector, upVector));

        //force our camera to rotate around the earth, however if we eliminate two next line we can not handle the camera movement
        upVector = (glm::rotate(theta, rightVector) * glm::vec4(upVector, 0.0f)).xyz();
        frontVector = (glm::rotate(theta, rightVector) * glm::vec4(frontVector, 0.0f)).xyz();}

    update();

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    float cameraZoom = event->angleDelta().ry() * zoomSpeed;                     //how much we rotate the scroll button
    if (cameraPosition.z - cameraZoom <= 0.0f) {
        cameraPosition = glm::vec3(cameraPosition.x, cameraPosition.y, 0.0f);
    }
    else {
        cameraPosition = glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z - cameraZoom);
    }
}

void GLWidget::animateGL()
{
    // make the context current in case there are glFunctions called
    makeCurrent();

    // get the time delta
    float timeElapsedMs = _stopWatch.nsecsElapsed() / 1000000.0f;
    // restart stopwatch for next update
    _stopWatch.restart();

    // calculate current modelViewMatrix for the default camera
    /// TODO: use your camera logic here
    // radius = z, x =theta , y = phi
    //here what we used is it just mathematical part of the polar coordination
    float x = cameraPosition.z * (sin(cameraPosition.x) * cos(cameraPosition.y));
    float y = cameraPosition.z * (-sin(cameraPosition.y));
    float z = cameraPosition.z * (-cos(cameraPosition.x) * cos(cameraPosition.y));

    curentCameraCoordinates = glm::vec3(x, y, z);

    // View Matrix is Look At matrix of 3 Direction : https://medium.com/@carmencincotti/lets-look-at-magic-lookat-matrices-c77e53ebdf78
    glm::mat4 modelViewMatrix = glm::lookAt(curentCameraCoordinates, curentCameraCoordinates + frontVector, upVector); //curentCameraCoordinates + frontVector => view direction

    if ( Config::resetCam == true) {
        modelViewMatrix = glm::lookAt(glm::vec3(.0, 0.0, -7.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
        }
    // update drawables
    /// TODO update all drawables
    _skybox->update(timeElapsedMs, modelViewMatrix);
    _earth->update(timeElapsedMs, modelViewMatrix);
    // update the widget (do not remove this!)
    update();
}
