//
//  Terrain.cpp
//  PA0
//
//  Created by Karl Wang on 3/1/20.
//

#include "Terrain.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

Terrain::Terrain(GLuint program, int gridX, int gridZ){
    modelLoc = glGetUniformLocation(program, "model");
//    SIZE = gridX*gridZ;
    generateTerrain();
}
void Terrain::generateTerrain(){
    // loop through z
    maxy = -FLT_MAX;
    miny = FLT_MAX;
    unsigned char* hmap_data = heightmap();
    int VERTEX_COUNT = hmapHeight;
    for(float i = 0; i < VERTEX_COUNT; i++){
        // loop through x
        for(float j = 0; j < VERTEX_COUNT; j++){
            float x = j/((float)VERTEX_COUNT - 1) * SIZE;
            float y = getHeight(j, i, hmap_data);
            maxy = (y > maxy)?y:maxy;
            miny = (y < maxy)?y:miny;
            float z = i/((float)VERTEX_COUNT - 1) * SIZE;
            vertices.push_back(glm::vec3(x,y,z));
            
            normals.push_back(calculateNormal(j, i, hmap_data));
            float u = j/((float)VERTEX_COUNT - 1);
            float v = i/((float)VERTEX_COUNT - 1);
            texCoords.push_back(glm::vec2(u,v));
        }
    }
    for(int z = 0; z < VERTEX_COUNT - 1; z++){
        for(int x = 0; x < VERTEX_COUNT - 1; x++){
            int topLeft = (z*VERTEX_COUNT)+x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1)*VERTEX_COUNT+x;
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    
    GLuint vbo[3];
    GLuint ebo;
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(3, &vbo[0]);
    
    // Bind to the VAO.
    // This tells OpenGL which data it should be paying attention to
    glBindVertexArray(vao);
    
    // pass in vertices
    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Pass in the data.
    //static draw means that no longer need to edit the data after storing
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->vertices.size(),
                 this->vertices.data(), GL_STATIC_DRAW);
    
    // Generate a EBO, it is stored in VAO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(),
                 indices.data(), GL_STATIC_DRAW);
    
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    // put vbo to vao, first is attribute num, which is 0
    // second is 3, which specifies the length of each vector, 3 dimensional
    // third is type of the data, which is float for each vertex
    // fourth specifies if the data is normalize
    // fifth is the distance between each vertices
    // sixth is offset, whether we should start at the beginning of the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // pass in normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->normals.size(),
                 this->normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    
    
    // pass in tex coords
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*this->texCoords.size(),
                 this->texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
    //    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    // Unbind from the VAO.
    glBindVertexArray(0);
    
    
    
    // create the texture
    glGenTextures(1, &texID);
    glBindBuffer(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data  =stbi_load("./grass.jpg", &width, &height, &nrChannels, 0);
//    unsigned char *data = 0;
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    stbi_image_free(hmap_data);
    cout << miny << "," << maxy << endl;
}
unsigned char* Terrain::heightmap(){
    int width, height, nrChannels;
    
    unsigned char *data = stbi_load("./heightmap.png", &width, &height, &nrChannels, 0);
    hmapWidth = width;
    hmapHeight = height;
//    for(int j = 0; j < height; j++){
//        for(int i = 0; i < width; i++){
//            cout << (int)data[3*j*width + 3*i] << "," <<(int)data[3*j*width + 3*i+1] << "," <<(int)data[3*j*width + 3*i+2]<<endl;
//        }
//    }
    return data;
}
void Terrain::draw(){
    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(-SIZE/2, -miny + (maxy-miny)/2 - 20, -SIZE/2));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Bind to the VAO.
    glBindVertexArray(vao);
//    glBindTexture(GL_TEXTURE_2D, texID);
    // VAO knows to use EBO, which is the last slot in VAO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //    glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
float Terrain::getHeight(int x, int z, unsigned char *data){
    if(x<0 || x> hmapWidth || z < 0 || z > hmapHeight) return 0;
    int r = data[(z*hmapWidth+x)*4];
    int g = data[(z*hmapWidth+x)*4+1];
    int b = data[(z*hmapWidth+x)*4+2];
    float height = r << 16 | g << 8 | b;
    height += MAX_PIXEL_COLOR/2.0f;
    height /= MAX_PIXEL_COLOR/2.0f;
    height *= MAX_HEIGHT;
    return  height;
}
glm::vec3 Terrain::calculateNormal(int x, int z, unsigned char *data){
    float heightL = getHeight(x-1, z, data);
    float heightR = getHeight(x+1, z, data);
    float heightD = getHeight(x, z-1, data);
    float heightU = getHeight(x, z+1, data);
    glm::vec3 normal = glm::vec3(heightL - heightR, 2, heightD-heightU);
    normal = normalize(normal);
    return normal;
}
