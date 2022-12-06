#include <GL/glew.h>

#include "drawable.h"

#include <iostream>

#include <QFile>
#include <QTextStream>

#include "glbase/gltool.hpp"

#include "image/image.h"

Drawable::Drawable(std::string name):
    _name(name), _vertexArrayObject(0), _modelViewMatrix(1.0f)
{

}

void Drawable::init()
{
    initShader();
    createObject();
}

void Drawable::recreate()
{
    createObject();
}

void Drawable::initShader()
{
    GLuint vs = CG::createCompileShader(GL_VERTEX_SHADER, getVertexShader()); VERIFY(vs);
    GLuint fs = CG::createCompileShader(GL_FRAGMENT_SHADER, getFragmentShader()); VERIFY(fs);

    _program = glCreateProgram();
    glAttachShader(_program, vs);
    glAttachShader(_program, fs);
    _program = CG::linkProgram(_program);

    VERIFY(_program);
}


std::string Drawable::loadShaderFile(std::string path) const
{
    QFile f(path.c_str());
    if (!f.open(QFile::ReadOnly | QFile::Text))
        std::cout << "Could not open file " << path << std::endl;
    QTextStream in(&f);
    return in.readAll().toStdString();
}

GLuint Drawable::loadTexture(std::string path){

    /// TODO your code here
    Image image(path);                       // load the image by using the path
    uchar *data  = image.getData();          // get character format of image

    GLuint textureID;

    float width = image.getWidth();
    float height = image.getHeight();

    // we've got some helps from this website for thenext 5 lines => https://gamedev.stackexchange.com/questions/163245/opengl-texture-rendered-as-solid-color-why
    glActiveTexture(GL_TEXTURE0);      //here we activate the texture of each object due to the .bmp file
    glGenTextures(1, &textureID);      //here we generate a texture ID and in next line we bind it to the generated texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);   //in this line we map the image to the geometrie and also scaled the width and height
//    In computer graphics, mipmaps (also MIP maps) or pyramids are pre-calculated, optimized sequences of images, each of which is a progressively lower resolution representation
//    of the previous. The height and width of each image, or level, in the mipmap is a factor of two smaller than the previous level. Mipmaps do not have to be square. They are intended to
//    increase rendering speed and reduce aliasing artifacts. A high-resolution mipmap image is used for high-density samples, such as for objects close to the camera; lower-resolution images
//    are used as the object appears farther away. This is a more efficient way of downfiltering (minifying) a texture than sampling all texels in the original texture that would contribute
//    to a screen pixel; it is faster to take a constant number of samples from the appropriately downfiltered textures. Mipmaps are widely used in 3D computer games, flight simulators, other
//    3D imaging systems for texture filtering, and 2D and 3D GIS software. Their use is known as mipmapping. The letters MIP in the name are an acronym of the Latin phrase multum in parvo, meaning "much in little".
    glGenerateMipmap(GL_TEXTURE_2D);                                                              // And here we use the Mip map technik

    return textureID;
}
