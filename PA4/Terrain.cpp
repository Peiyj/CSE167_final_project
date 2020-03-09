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

Terrain::Terrain(GLuint program, int gridX, int gridZ, bool diamondSquare){
    this->program = program;
    counter = 0;
    modelLoc = glGetUniformLocation(program, "model");
    this->gridX = gridX;
    this->gridZ = gridZ;
    if(diamondSquare) VERTEX_COUNT = 128+1;
    else VERTEX_COUNT = 256;
    float** arr;
    if(diamondSquare){
        arr = new float *[VERTEX_COUNT];
        for(int i = 0; i < VERTEX_COUNT; i++){
            arr[i] = new float [VERTEX_COUNT];
        }
        for(int j = 0; j < VERTEX_COUNT;j++){
            for(int i = 0; i < VERTEX_COUNT; i++){
                arr[j][i] = NULL;
            }
        }
        //    arr[0][0] = random(VERTEX_COUNT);
        //
        //    arr[VERTEX_COUNT-1][0] = random(VERTEX_COUNT);
        //    arr[0][VERTEX_COUNT-1] = random(VERTEX_COUNT);
        //    arr[VERTEX_COUNT-1][VERTEX_COUNT-1] = random(VERTEX_COUNT);
        arr[0][0] = 0;
        arr[VERTEX_COUNT-1][0] = 0;
        arr[0][VERTEX_COUNT-1] = 0;
        arr[VERTEX_COUNT-1][VERTEX_COUNT-1] = 0;
        
        dsRecursion(arr, VERTEX_COUNT-1);
    }
    generateTerrain(arr, diamondSquare);
    
    model = glm::translate(glm::mat4(1), glm::vec3(-SIZE/2 + SIZE*gridX, -(miny+(maxy-miny)/2), -SIZE/2+SIZE*gridZ));
}
void Terrain::generateTerrain(float** height_map, bool diamondSquare){
    // loop through z
    maxy = -FLT_MAX;
    miny = FLT_MAX;
    //    maxy = 0;
    //    miny = 0;
    unsigned char* hmap_data;
    if(!diamondSquare) hmap_data = heightmap();
    for(float i = 0; i < VERTEX_COUNT; i++){
        // loop through x
        for(float j = 0; j < VERTEX_COUNT; j++){
            float x = j/((float)VERTEX_COUNT - 1) * SIZE;
            float y = 0;
            if(diamondSquare)  y  = (height_map[int(j)][int(i)])/((float)VERTEX_COUNT-1)*SIZE;
            else  y = getHeight(j, i, hmap_data);
            maxy = (y > maxy)?y:maxy;
            miny = (y < miny)?y:miny;
            float z = i/((float)VERTEX_COUNT - 1) * SIZE;
            vertices.push_back(glm::vec3(x,y,z));
            
            //            normals.push_back(calculateNormal(j, i, hmap_data));
            normals.push_back(glm::vec3(0,1,0));
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
    

    
    
    //pass in miny, maxy

    

    glUseProgram(program);

    glUniform1f(glGetUniformLocation(program, "maxy"), maxy);
    glUniform1f(glGetUniformLocation(program, "miny"), miny);
    
    glUniform1i(glGetUniformLocation(program, "grass"), 0);
    glUniform1i(glGetUniformLocation(program, "sand"), 1);
    glUniform1i(glGetUniformLocation(program, "snow"), 2);
    // create the texture

    glGenTextures(1, &grassID);
    glGenTextures(1, &sandID);
    glGenTextures(1, &snowID);
    
//    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassID);
    
    glBindBuffer(GL_TEXTURE_2D, grassID);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


//    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sandID);


    glBindBuffer(GL_TEXTURE_2D, sandID);
    data  =stbi_load("./sand.jpg", &width, &height, &nrChannels, 0);
    //    unsigned char *data = 0;
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(1);
    glBindTexture(GL_TEXTURE_2D, 1);

    
    
    
//    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, snowID);
    
    
    glBindBuffer(GL_TEXTURE_2D, snowID);
    data  =stbi_load("./snow.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sandID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, snowID);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    // VAO knows to use EBO, which is the last slot in VAO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
//    glActiveTexture(0);
}
glm::vec3 Terrain::calculateNormal(int x, int z, unsigned char *data){
    float heightL = getHeight(x-1, z, data);
    float heightR = getHeight(x+1, z, data);
    float heightD = getHeight(x, z-1, data);
    float heightU = getHeight(x, z+1, data);
    glm::vec3 normal = glm::vec3(heightL - heightR, 2, heightD-heightU);
    normal = normalize(normal);
    return normal;
    return glm::vec3(0);
}
void Terrain::diamondSquare(float **arr, int startW, int endW, int startH, int endH,
                            bool diamond, float r){
    //-  -
    //
    //-  -
    int half = (endW-startW)/2;
    if(half==0) half = 1;
    if(diamond){
        
        float topLeft = arr[startH][startW];
        float topRight = arr[startH][endW];
        float bottomLeft = arr[endH][startW];
        float bottomRight = arr[endH][endW];
        float avg = (topLeft+topRight+bottomLeft+bottomRight)/4;
        avg+=r;
        int midpointH = (endH-startH)/2 + startH;
        int midpointW = (endW-startW)/2 + startW;
        if(arr[midpointH][midpointW] != NULL) return;
        arr[midpointH][midpointW] = avg;
    }
    //  -
    //-   -
    //  -
    else{
        int middleH = startH + (endH-startH)/2;
        int middleW = startW + (endW-startW)/2;
        if (arr[middleH][middleW] != NULL) return;
        int counter = 0;
        float avg = 0;
        if (startH >= 0 && startH < VERTEX_COUNT){
            float top = arr[startH][middleW];
            avg += top;
            counter++;
        }
        if (endH >= 0 && endH < VERTEX_COUNT){
            float bottom = arr[endH][middleW];
            avg += bottom;
            counter++;
        }
        if (startW >= 0 && startW < VERTEX_COUNT){
            float left = arr[middleH][startW];
            avg += left;
            counter++;
        }
        if (endW >= 0 && endW < VERTEX_COUNT){
            float right = arr[middleH][endW];
            avg += right;
            counter++;
        }
        if(counter > 2){
            avg /= counter;
            avg += r;
        }
        else return;
        arr[middleH][middleW] = avg;
    }
}
void Terrain::dsRecursion(float **arr, int size){
    // take care of left
    counter++;
    if (size == 0) return;
    // perform square step
    for(int j = 0; j < VERTEX_COUNT; j += size){
        for(int i = 0; i < VERTEX_COUNT; i += size){
            //            if((i-size < 0 && j-size < 0 ))
            float r = random(size);
            diamondSquare(arr, i-size, i+size, j-size, j+size, false, r);
        }
    }
    
    // perform diamond step
    for(int j = 0; j < VERTEX_COUNT-size; j += size){
        for(int i = 0; i < VERTEX_COUNT-size; i += size){
            //            if((i-size < 0 && j-size < 0 ))
            
            float r = random(size);
            diamondSquare(arr, i, i+size, j, j+size, true,r);
        }
    }
    
    
    // recursively calcualte top left
    dsRecursion(arr, size/2);
    
}
float Terrain::random(int range){
    if(range == 0) return 1;
    float res = ((float)rand()/(float)RAND_MAX - 0.5) * range;
    return res;
}
void Terrain::convert(float** heightmap, float minHeight, float maxHeight){
    float max = -FLT_MAX;
    float min = FLT_MAX;
    for(int j = 0; j < VERTEX_COUNT; j++){
        for(int i = 0; i < VERTEX_COUNT; i++){
            max = (heightmap[j][i] > max)?heightmap[j][i]:max;
            min = (heightmap[j][i] < min)?heightmap[j][i]:min;
        }
    }
    for(int j = 0; j < VERTEX_COUNT; j++){
        for(int i = 0; i < VERTEX_COUNT; i++){
            if(min>0){
                heightmap[j][i] -= min;
            }
            else{
                heightmap[j][i] += min;
            }
            heightmap[j][i] *= (maxHeight - minHeight)/(max-min);
            heightmap[j][i] -= minHeight;
        }
    }
    
    
    
    
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
glm::mat4 Terrain::getModel(){
    return model;
}
vector<glm::vec3> Terrain::getVertices(){
    return vertices;
}
vector<unsigned int> Terrain::getIndices(){
    return indices;
}
float Terrain::getMiny(){
    return miny;
}
float Terrain::getMaxy(){
    return maxy;
}
int Terrain::getSize(){
    return SIZE;
}
