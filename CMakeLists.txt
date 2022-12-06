# Copyright (C) 2013, 2014
# Computer Graphics Group, University of Siegen
# Written by Martin Lambers <martin.lambers@uni-siegen.de>
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and this
# notice are preserved. This file is offered as-is, without any warranty.

cmake_minimum_required(VERSION 2.8.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0017 NEW)
cmake_policy(SET CMP0020 NEW)

project("GraPra 2015 - Tycho Brahe" C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#if(CMAKE_COMPILER_IS_GNUCXX)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Wextra")
#endif()

# Required libraries
find_package(Qt5OpenGL 5.4.0 REQUIRED)

# process .ui files
############################
# List your ui files here  #
# (if you need any)        #
############################
qt5_wrap_ui(ui_mainwindow.h gui/mainwindow.ui)

#process resource files
############################
# List your qrc files here #
# (if you need any)        #
############################
qt5_add_resources (ResourceSources shader.qrc)

# Optional libraries
find_package(GTA QUIET)

# The utility library
add_subdirectory(glbase)

# The program
################################
# List your project files here #
################################
add_executable(tychobrahe
    main.cpp
    ${ResourceSources}
    image/image.cpp
    gui/mainwindow.ui
    gui/mainwindow.cpp
    gui/glwidget.cpp
    gui/glwidget.hpp
    gui/config.cpp
    planets/cone.cpp
    planets/coordinatesystem.cpp
    planets/deathstar.cpp
    planets/drawable.cpp
    planets/orbit.cpp
    planets/path.cpp
    planets/planet.cpp
    planets/skybox.cpp
    planets/sun.cpp
    planets/sphere.cpp
    planets/ring.cpp
    planets/saturn.cpp

    shader/cube.vs.glsl
    shader/cube.fs.glsl
    shader/planet.vs.glsl
    shader/planet.fs.glsl
#    shader/skybox.fs.glsl
#    sheder/skybox.vs.glsl
)

include_directories(${CMAKE_SOURCE_DIR}/glbase ${OPENGL_INCLUDE_DIR})

####################################
# Build with correct opengl lib    #
# (this could fail on your system) #
####################################
if(WIN32 OR CYGWIN)
        target_link_libraries(tychobrahe opengl32 libglbase Qt5::OpenGL ${OPENGL_gl_LIBRARY})
else()
        target_link_libraries(tychobrahe GL libglbase Qt5::OpenGL ${OPENGL_gl_LIBRARY})
endif()

if(GTA_FOUND)
        add_definitions(-DHAVE_GTA)
        include_directories(${GTA_INCLUDE_DIR})
        target_link_libraries(tychobrahe ${GTA_LIBRARIES})
endif()

# copy images
add_custom_command(TARGET tychobrahe POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:tychobrahe>/images)

install(TARGETS tychobrahe RUNTIME DESTINATION bin)
