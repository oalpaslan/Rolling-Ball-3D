#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>\

using namespace std;
float vertices[] = {    //In OpenGL, every range value is between -1 and 1
    -0.5f, -0.5f, 0.0f, //Since it's a triangle, z values are 0
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //For resizing the window for any size the user wants


}

void processInput(GLFWwindow* window)   //
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); //When ESC pressed, window should close
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //We tell that the major and minor version are v3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    unsigned int VBO;
    glGenBuffers(1, &VBO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    //First argument is the type of the buffer we want to copy data into
    //the vertex buffer object currently bound to the GL_ARRAY_BUFFER target. 
    //The second argument specifies the size of the data (in bytes) we want to pass to the buffer; 
    //a simple sizeof of the vertex data suffices. 
    //The third parameter is the actual data we want to send.
    //The fourth parameter specifies how we want the graphics card to manage the given data.This can take 3 forms:
        //GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        //GL_STATIC_DRAW : the data is set only once and used many times.
        //GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rolling Ball 3D", NULL, NULL); //Creating a window with size 800x600px
    if (window == NULL) //If the window have not opened
    {
        std::cout << "Failed to create GLFW window" << std::endl;  
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //GLAD load check
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

 /*   glViewport(0, 0, 800, 600);  //For telling OpenGL the size of rendering window.
                                        //First two parameters are for setting the location of the lower left corner of the window
                                        //3rd and 4th parameters set width and height of the rendering window (in pixels) */

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //We do have to tell GLFW we want to call size callback function on every window resize by registering it
    
    while (!glfwWindowShouldClose(window))  //glfwWindowShouldClose checks if GLFW told to close.
    {
        //input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //At the start of frame we want to clear the screen. 
        glClear(GL_COLOR_BUFFER_BIT);           //Otherwise we would still see the results from the previous frame
                                                //glClearColor: The color to be filled with glClear
                                                //glClear: Wee pass in buffer bits to specify which buffer we would like to clear.
        

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate(); //As soon as the project finished we clean/delete all of the GLFW's resources
	return 0;
}