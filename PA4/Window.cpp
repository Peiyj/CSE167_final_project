#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
namespace
    {
    int width, height;
    std::string windowTitle("GLFW Starter Project");
    int state = 0;
    glm::vec3 lastPoint;
    
    glm::vec2 lastXY;
    
    double xpos, ypos;
    glm::vec3 eye(0, 0, 20); // Camera position.
    glm::vec3 center(0, 0, 0); // The point we are looking at.
    glm::vec3 up(0, 1, 0); // The up direction of the camera.
    float fovy = 60;
    float near = 1;
    float far = 1000;
    glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
    glm::mat4 projection; // Projection matrix.
    
    
    
    
    GLuint program; // The shader program id.
    GLuint projectionLoc; // Location of projection in shader.
    GLuint viewLoc; // Location of view in shader.
    
    OBJObject* teapot;
    };

bool Window::initializeProgram()
{
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    // This shader program is for displaying your rasterizer results

    // Check the shader program.
    if (!program)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    return true;
}

bool Window::initializeObjects()
{
    
    // Create a point cloud consisting of cube vertices.

    teapot = new OBJObject("./teapot.obj", program);
    


    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.

    glDeleteProgram(program);
    
    
    
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#ifdef __APPLE__ 
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    
    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window.
    glfwMakeContextCurrent(window);
    
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    
    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif
    
    // Set swap interval to 1.
    glfwSwapInterval(0);
    // Initialize the quad that will be textured with your image
    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    //    width = w;
    //    height = h;
    // Set the viewport size.
    glViewport(0, 0, width, height);
    // Set the projection matrix.
    projection = glm::perspective(glm::radians(fovy),
                                  (float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.
//    teapot->update();

}

void Window::displayCallback(GLFWwindow* window)
{
    
    
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    teapot->draw();
    // set the function for mouse click
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // set the function to acquire cursor position
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    glfwGetCursorPos(window, &xpos, &ypos);
    
    glfwSetScrollCallback(window, scroll_callback);
    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
    
}
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}
void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    glm::vec3 direction;
    
    float rot_angle;
    glm::vec3 curPoint;
//    glm::vec2 curXY;
    if (state == ROTATE) {
        curPoint = trackBallMapping(xpos, ypos);
        direction = curPoint - lastPoint;
        float velocity = glm::length(direction);
        if (velocity > 0.0001) {
            glm::vec3 rotAxis = glm::cross(lastPoint, curPoint);
            rot_angle = velocity * 90.0f;
            view = glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis) * view;
        }
        lastPoint = curPoint;
    }
}


glm::vec3 Window::trackBallMapping(double xpos, double ypos) {
    glm::vec3 v;
    float d;
    v.x = (2.0f * xpos - width) / width;
    v.y = (height - 2.0f * ypos) / height;
    
    v.z = 0.0;
    
    d = glm::length(v);
    d = (d < 1.0f)?d:1.0f;
    v.z = glm::sqrt(1.001 - d * d);
    v = glm::normalize(v);
    return v;
    
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                lastPoint = trackBallMapping(xpos, ypos);
                state = ROTATE;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                lastXY = glm::vec2(xpos, ypos);
                state = TRANSLATE;
                break;
        }
    }
    else if (action == GLFW_RELEASE) {
        state = NONE;
    }
    
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
                
            case GLFW_KEY_W:
                view = glm::translate(glm::mat4(1), glm::vec3(0,0,2))*view;
                break;
            case GLFW_KEY_S:
                view = glm::translate(glm::mat4(1), glm::vec3(0,0,-2))*view;
                break;
            case GLFW_KEY_A:
                view = glm::translate(glm::mat4(1), glm::vec3(2,0,0))*view;
                break;
            case GLFW_KEY_D:
                view = glm::translate(glm::mat4(1), glm::vec3(-2,0,0))*view;
                break;
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_F1:
                // Set currentObj to cube
                
                break;
            case GLFW_KEY_F2:
                // Set currentObj to cubePoints
                
                break;
            case GLFW_KEY_F3:

                break;
                
            case GLFW_KEY_R:
            {
                view = glm::lookAt(eye, center, up);
                break;
            }
            default:
                break;
        }
    }
}
