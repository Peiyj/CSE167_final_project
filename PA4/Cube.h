#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
    GLuint texColorLoc;
    GLuint texModelLoc;
    GLuint program;
    
    unsigned int cubemapTex;
public:
	Cube(float size, GLuint program);
	~Cube();
    void callback();
	void draw();
	void update();
    unsigned int loadCubemap();
    unsigned char* loadPPM(const char*, int&, int&);
	void spin(float deg);
};

#endif

