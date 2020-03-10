//
//  Water.cpp
//  PA0
//
//  Created by Karl Wang on 3/4/20.
//

#include "Water.hpp"
#include "stb_image.h"

using namespace std;
Water::Water(GLuint program, int size, float miny, float maxy,
             glm::mat4 model){
    this->program = program;
    this->model = model;
    moveFactor = 0;
    //set percentile 0.2 as the water
    y = 0.2*(maxy-miny)+miny;
    int VERTEX_COUNT = 129;
    for(int i = 0; i < VERTEX_COUNT; i++){
        for(int j = 0; j < VERTEX_COUNT; j++){
            float x = j/((float)VERTEX_COUNT - 1) * size;
            float z = i/((float)VERTEX_COUNT - 1) * size;
            vertices.push_back(glm::vec3(x,y,z));
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
        // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbind from the VAO.
    glBindVertexArray(0);
    
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "reflectionTex"), 3);
    glUniform1i(glGetUniformLocation(program, "refractionTex"), 4);
    glUniform1i(glGetUniformLocation(program, "dudvTex"), 5);
    glUniform1i(glGetUniformLocation(program, "depthMap"), 6);
    
    
    glBindTexture(GL_TEXTURE_2D, dudvTex);

    glBindBuffer(GL_TEXTURE_2D, dudvTex);
    int width, height, nrChannels;
    unsigned char *data  =stbi_load("./waterDUDV.png", &width, &height, &nrChannels, 0);
    //    unsigned char *data = 0;
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindBuffer(GL_TEXTURE_2D, 0);

//
    y = (model*glm::vec4(0,y,0,1)).y;
}
void Water::draw(){
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, reflectTex);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, refractTex);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, dudvTex);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, refractionDepthTex);

//    glm::vec3 color = glm::vec3(0, 0.4, 0.8);
//    glm::vec3 color = glm::vec3(1);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
//    glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));
    glUniform1f(glGetUniformLocation(program, "moveFactor"), moveFactor);
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    // VAO knows to use EBO, which is the last slot in VAO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    
}

void Water::setTexID(GLuint reflectTex, GLuint refractTex, GLuint refractionDepthTex){
    this->reflectTex = reflectTex;
    this->refractTex = refractTex;
    this->refractionDepthTex = refractionDepthTex;
}
void Water::moveWater(float y){
    model = glm::translate(glm::mat4(1), glm::vec3(0,y,0)) * model;
    this->y+=y;
}
float Water::getHeight(){
    return y;
}
void Water::update(){
//    moveFactor += 0.000005;
    moveFactor += 0.00005;
    moveFactor = fmod(moveFactor, 1.0f);
}
