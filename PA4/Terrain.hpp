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
class Terrain {
private:

    GLuint modelLoc;
    GLuint vao;
    GLuint texID;
    
    int SIZE = 200;
    int MAX_HEIGHT = 20;
    int MAX_PIXEL_COLOR = 256*256*256;
    float maxy;
    float miny;
    int hmapWidth;
    int hmapHeight;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> texCoords;
    
public:
    Terrain(GLuint program, int gridX, int gridZ);
    void generateTerrain();
    unsigned char* heightmap();
    void draw();
    float getHeight(int x, int z, unsigned char* data);
    glm::vec3 calculateNormal(int x, int z, unsigned char* data);
};
#endif /* Terrain_hpp */
