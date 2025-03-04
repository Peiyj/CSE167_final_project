//
//  Terrain.hpp
//  PA0
//
//  Created by Karl Wang on 3/1/20.
//

#ifndef Terrain_hpp
#define Terrain_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <float.h>
#include <stdlib.h>     /* srand, rand */
class Terrain {
private:
    GLuint program;

    GLuint modelLoc;
    GLuint vao;
    int VERTEX_COUNT;
    int SIZE = 800;
    int MAX_HEIGHT = 20;
    int MAX_PIXEL_COLOR = 256*256*256;
    float maxy;
    float miny;
    int hmapWidth;
    int hmapHeight;
    int gridX;
    int gridZ;
    int counter;
    glm::mat4 model;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> texCoords;
    
    GLuint vbo[3];
    GLuint ebo;
    GLint toneShading;
public:
    GLuint grassID;
    GLuint sandID;
    GLuint snowID;
    ~Terrain();
    Terrain(GLuint program, int gridX, int gridZ, bool diamondSquare);
    void generateTerrain(float**, bool diamondSquare);
    unsigned char* heightmap();
    void draw();
    glm::vec3 calculateNormal(int x, int z, unsigned char* data);
    void diamondSquare(float**, int, int, int, int, bool, float);
    void dsRecursion(float**, int);
    float random(int);
    void convert(float**, float minHeight, float maxHeight);
    float getHeight(int x, int z, unsigned char* data);
    glm::mat4 getModel();
    std::vector<glm::vec3> getVertices();
    std::vector<unsigned int> getIndices();
    float getMiny();
    float getMaxy();
    int getSize();
    void switchToToneShading();
    
    
};
#endif /* Terrain_hpp */
