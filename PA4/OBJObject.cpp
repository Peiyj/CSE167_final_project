//
//  OBJObject.cpp
//  PA0
//
//  Created by Karl Wang on 2/7/20.
//

#include "OBJObject.hpp"
#include <iostream>
#include <algorithm>    // std::min

using namespace std;
OBJObject::OBJObject(const char* obj, GLuint program)
{
    FILE* fp;
    
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> textures;
    
    vector<unsigned int> vertices_idx;
    vector<unsigned int> normals_idx;
    vector<unsigned int> textures_idx;
    
    float x,y,z;
    float r,g,b;
    int c1, c2;
    int i1, n1, i2, n2, i3, n3, t1, t2, t3, i4, n4, t4;
    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    
    float minx, miny, minz;
    float maxx, maxy, maxz;
    minx = miny = minz = FLT_MAX;
    maxx = maxy = maxz = -FLT_MAX;
    fp = fopen(&obj[0], "rb");
    if (fp == NULL){
        cerr << "error loading file" << endl;
        exit(-1);
    }
    
    while(1){
        c1 = fgetc(fp);
        // get until c1 is v
        while (!(c1 == 'v' ||  c1 == EOF || c1 == 'f')){
            c1 = fgetc(fp);
        }
        
        if(c1 == EOF) break;
        c2 = fgetc(fp);
        if(c1 == 'v' && c2 == ' '){
            fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            minx = min(x, minx);
            miny = min(y, miny);
            minz = min(z, minz);
            maxx = max(x, maxx);
            maxy = max(y, maxy);
            maxz = max(z, maxz);
            vertices.push_back(glm::vec3(x,y,z));
        }
//        else if(c1 == 'v' && c2 == 't'){
//            fscanf(fp, "%f %f", &x, &y);
//            textures.push_back(glm::vec2(x,y));
//        }
        else if(c1 == 'v' && c2 == 'n'){
            fscanf(fp, "%f %f %f", &x, &y, &z);
            normals.push_back(glm::vec3(x,y,z));
        }
        else if(c1 == 'f' && c2 == ' '){
//            fscanf(fp, "%d//%d %d//%d %d//%d", &i1, &n1, &i2, &n2,
//                   &i3, &n3);
            fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &i1, &t1, &n1, &i2, &t2, &n2,
                   &i3, &t3, &n3);
            vertices_idx.push_back(i1-1);
            vertices_idx.push_back(i2-1);
            vertices_idx.push_back(i3-1);
            normals_idx.push_back(n1-1);
            normals_idx.push_back(n2-1);
            normals_idx.push_back(n3-1);
//            textures_idx.push_back(t1-1);
//            textures_idx.push_back(t2-1);
//            textures_idx.push_back(t3-1);
        }
        
    }
    //    cout << glm::to_string(indices[0]) <<endl;
    fclose(fp);
    for(int i = 0; i < vertices_idx.size(); i++){
        this->vertices.push_back(vertices[vertices_idx[i]]);
        this->normals.push_back(normals[normals_idx[i]]);
//        this->textures.push_back(textures[textures_idx[i]]);
        indices.push_back(i);
    }
    
    /*
     * Get the tangent/bitangent
     */
    
    /*
     * Normalize the object to fit in the screen.
     */
    float midx, midy, midz;
    float scalex, scaley, scalez;
    midx = minx + (maxx-minx)/2;
    midy = miny + (maxy-miny)/2;
    midz = minz + (maxz-minz)/2;
    glm::vec3 center = glm::vec3(midx, midy, midz);
    scalex = 2/(maxx-minx);
    scaley = 2/(maxy-miny);
    scalez = 2/(maxz-minz);
    
    float scale_factor = min(min(scalex, scaley), scalez);
    scale_factor = 10;
    
    model = glm::scale(glm::mat4(1), glm::vec3(scale_factor))*glm::translate(glm::mat4(1.0f), -center)*glm::mat4(1.0f);
    original_model = model;
    moved_model = glm::translate(glm::mat4(1), glm::vec3(0,0,0));
    scaled_model = glm::scale(glm::mat4(1), glm::vec3(1));
    rotated_model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1));
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, &vbo[0]);
    
    // Bind to the VAO.
    // This tells OpenGL which data it should be paying attention to
    glBindVertexArray(vao);
    
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
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->normals.size(),
                 this->normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    modelLoc = glGetUniformLocation(program, "model");
    colorLoc = glGetUniformLocation(program, "color");
    // shadow mapping
    //create frame buffer to render depth map
}

OBJObject::~OBJObject()
{
    // Delete the VBO and the VAO.
    // Failure to delete your VAOs, VBOs and other data given to OpenGL
    // is dangerous and may slow your program and cause memory leaks
//    delete(material);
    glDeleteBuffers(2, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
    
}

void OBJObject::draw()
{
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1)));

    // Bind to the VAO.
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OBJObject::update()
{
//    model = moved_model*rotated_model*scaled_model*original_model;
    
    
    //rotate around the origin center with this
//    model = rotated_model*moved_model*scaled_model*original_model;
    
}

void OBJObject::moveAxis(GLfloat x, GLfloat y, GLfloat z){
    moved_model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))*moved_model;
}
void OBJObject::scaleModel(GLfloat scalar) {
    scaled_model = glm::scale(glm::mat4(1), glm::vec3(scalar))*scaled_model;
}

void OBJObject::reset(){
    
    moved_model = glm::translate(glm::mat4(1), glm::vec3(0,0,0));
    scaled_model = glm::scale(glm::mat4(1), glm::vec3(1));
    rotated_model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1));
    
}

void OBJObject::mouseRotate(float rot_angle, glm::vec3 rotAxis) {
    
    rotated_model = glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis) * rotated_model;
    
}
