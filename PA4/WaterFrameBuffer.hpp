//
//  WaterFrameBuffer.hpp
//  PA0
//
//  Created by Karl Wang on 3/4/20.
//

#ifndef WaterFrameBuffer_hpp
#define WaterFrameBuffer_hpp

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
class WaterFrameBuffer{
private:
    int REFLECTION_WIDTH = 1536;
    int REFLECTION_HEIGHT = 960;
    int REFRACTION_WIDTH = 1536;
    int REFRACTION_HEIGHT = 960;
//    int REFLECTION_WIDTH = 320;
//    int REFLECTION_HEIGHT = 180;
//    int REFRACTION_WIDTH = 320;
//    int REFRACTION_HEIGHT = 180;
    int original_width;
    int original_height;
    GLuint reflectionFrameBuffer;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;
    
    GLuint refractionFrameBuffer;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;
public:
    WaterFrameBuffer(int width, int height);
    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindFrameBuffer();
    GLuint getReflectionTexture();
    GLuint getRefractionTexture();
    GLuint getRefractionDepthTexture();
    void setWidthHeight(int width, int height);
    
        
};


#endif /* WaterFrameBuffer_hpp */
