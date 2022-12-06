#include <GL/glew.h>
#include "sphere.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif
#include "gui/config.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include <stack>


sphere::sphere(float radius):
    _radius(radius)
{
      float phi = (2.0f * M_PI) / Config::u;                                            //the size of phi that we've got in each step and section
      float theta = M_PI / Config::v;                                                   //the size of theta that we've got in each step and section

      int totaL_number_of_row_sections = Config::u + 1;

      for (int v = 0; v <= Config::v; v++) {                                              // WE need two for loops to go over of all the vertecies that we want to have
          float thetas = theta * v;                                                            //number of actual theta an phi
          for (int h = 0; h <= Config::u; h++) {
              float phis = phi * h;

              // mathematical intepretation of sphere formula:
              // sin(phis) X sin(thetas) , cos(thetas) , cos(phis) X sin(thetas)
              positions.push_back(_radius * glm::vec3(sin(phis) * sin(thetas), cos(thetas), cos(phis) * sin(thetas)));         //save all vertecies in position vector


              norms.push_back(_radius * glm::vec3(sin(phis) * sin(thetas), cos(thetas), cos(phis) * sin(thetas)));             //save all norms in norm vector


              texcoords.push_back(glm::vec2((1.0f * h) / Config::u, (1.0f * v) / Config::v));                           //save all Coordinates in a vector to reuse it for texture mapping



              if (((v + 1) <= Config::v) &  (h != Config::u)) {
                      // generate index vector of sphere triangles
                      // k2 = v * totaL_number_of_row_sections + h
                      // k2 + 1 = v * totaL_number_of_row_sections + (h + 1)
                      // k1 + 1 = (v + 1) * totaL_number_of_row_sections + (h + 1)

                      // k1--k1+1
                      // |  / |
                      // | /  |
                      // k2--k2+1


                      // k1 -> k2 -> k2+1
                      indices.push_back(v * totaL_number_of_row_sections + h);                             //we save the indices for example in this order: 0 -> 1 -> totaL_number_of_row_sections + 1 ( go to upper line )
                      indices.push_back(v * totaL_number_of_row_sections + (h + 1));
                      indices.push_back((v + 1) * totaL_number_of_row_sections + (h + 1));

                      // k1+1 -> k2 -> k2+2
                      indices.push_back((v + 1) * totaL_number_of_row_sections + (h + 1));                 //we save the indices for example in this order: totaL_number_of_row_sections -> totaL_number_of_row_sections - 1 -> 0 )
                      indices.push_back((v + 1) * totaL_number_of_row_sections + h);
                      indices.push_back(v * totaL_number_of_row_sections + h);
                  }
      }
          }
}
    std::vector<unsigned int> sphere ::getIndices(){
        return indices;
    }
    std::vector<glm::vec3> sphere :: getPositions(){
        return positions;
    }
    std::vector<glm::vec2> sphere :: getCoord(){
        return texcoords;
    }
    std::vector<glm::vec3> sphere :: getNormal(){
        return norms;
    }

