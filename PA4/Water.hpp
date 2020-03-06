//
//  Water.hpp
//  PA0
//
//  Created by Karl Wang on 3/4/20.
//

#ifndef Water_hpp
#define Water_hpp

#include <stdio.h>


#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <float.h>
#include <stdlib.h>     /* srand, rand */

class Water{
private:
    GLuint program;
    GLuint vao;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> texCoords;
    glm::mat4 model;
public:
    GLuint texture;
    Water(GLuint program,int size, float miny, float maxy,
          glm::mat4 model);
    void draw();
    void setTexID(GLuint texture);
  
};
#endif /* Water_hpp */
