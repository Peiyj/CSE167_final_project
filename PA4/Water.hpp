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
    
    GLuint reflectTex;
    GLuint refractTex;
    GLuint dudvTex;
    GLuint refractionDepthTex;
    float moveFactor;
    
    
    float waveTime;
    
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec4> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> texCoords;
    glm::mat4 model;
    bool drawLine;
    GLint normalColor;
    GLint depthMap;
    GLint noDistortion;
    GLint noDUDV;

    float y;
public:
    Water(GLuint program,int size, float miny, float maxy,
          glm::mat4 model);
    void draw();
    void setTexID(GLuint reflectionTex, GLuint refractionTex, GLuint refractionDepthTex);
    void moveWater(float y);
    float getHeight();
    void update();
    glm::vec4 calculateOffset(int index1, int index2, int index3);
    void switchToDrawLine();
    void switchToNormal();
    void switchToDepthMap();
    void switchToNoDistortion();

};
#endif /* Water_hpp */
