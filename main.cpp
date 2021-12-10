#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" 
"layout (location = 1) in vec3 aColor;\n" 
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";


const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor,1.0f);\n"        //vec4(red, green, blue, alpha(opacity) 
"}\n\0";


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


    //First argument is the type of the buffer we want to copy data into
    //the vertex buffer object currently bound to the GL_ARRAY_BUFFER target. 
    //The second argument specifies the size of the data (in bytes) we want to pass to the buffer; 
    //a simple sizeof of the vertex data suffices. 
    //The third parameter is the actual data we want to send.
    //The fourth parameter specifies how we want the graphics card to manage the given data.This can take 3 forms:
        //GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times. 
        //GL_STATIC_DRAW : the data is set only once and used many times.
        //GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rolling Ball 3D", NULL, NULL); //Creating a window with size 800x600px
    if (window == NULL) //If the window have not opened
    {
        std::cout << "Failed to create GLFW window" << std::endl;  
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //GLAD load check
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

 /*   glViewport(0, 0, 800, 600);  //For telling OpenGL the size of rendering window.
                                        //First two parameters are for setting the location of the lower left corner of the window
                                        //3rd and 4th parameters set width and height of the rendering window (in pixels) */

    
    //We do have to tell GLFW we want to call size callback function on every window resize by registering it
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //We create the object "vertexShader", then we provide the type 
                                                                  //in glCreateShader as the parameter

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   //We attach the vertex shader using glShaderSource
                                                                  //1st argument: Shader object will be attached
                                                                  //2nd: How many strings we are passing as source code.
                                                                  //3rd: Actual source code of the vertex
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512]; // For error messages
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //Check if compilation was successful
    if (!success)   //Output error msg if there is a compile error
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;                                  //After the vertex shader, we initialize the fragment shader.
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);          //Like before
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;             //Creating a shader program for linking multiple shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);    //We attach the shaders and then link them wth glLinkProgram
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)   //Output error msg if there is a link error
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    float vertices[] = {   
        // positions    |     // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };


    unsigned int VBO, VAO; //VBO: Vertex Buffer Object, VAO: Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Copy our vertices array in a buffer for OpenGL to use

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);          //Takes vertices as parameter


    //Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //The first parameter specifies which vertex attribute we want to configure. 
        //We specified the location of the position vertex attribute in the vertex shader with "layout (location = 0)"[ln.8]
        //This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
    //2nd: Size of the vertex attribute. The vertex attribute is a vec3 [ln.8] so it is composed of 3 values.
    //3rd: Type of the data which is GL_FLOAT 
    //4th: If we want the data to be normalized. 
        //If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 
        //(or -1 for signed data) and 1 when converted to float. Not relevant
    //5th: Known as "stride". The space between consecutive vertex attributes.
        //Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride.
    
    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //We can say that since we set the location to 1
    glEnableVertexAttribArray(1);


   
    //glDeleteShader(vertexShader);   //Once we activated the program, we don't need shaders anymore,
    //glDeleteShader(fragmentShader); //since we linked them in the program

    glUseProgram(shaderProgram); //We activate the program

    while (!glfwWindowShouldClose(window))  //glfwWindowShouldClose checks if GLFW told to close.
    {
        //input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //At the start of frame we want to clear the screen. 
        glClear(GL_COLOR_BUFFER_BIT);           //Otherwise we would still see the results from the previous frame
                                                //glClearColor: The color to be filled with glClear
                                                //glClear: Wee pass in buffer bits to specify which buffer we would like to clear.

        
        
         

        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate(); //As soon as the project finished we clean/delete all of the GLFW's resources
	return 0;
}