#include<glad/glad.h>       
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

// ********************************************* HELLOW TRIANGLE ****************************************************

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)){
    glfwSetWindowShouldClose(window,true);
  }
}

//writing our vertex shader in GLSL
//Wrap the GLSL shader inside a C++ raw string literal otherwise cant write glsldirect in c++ file
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 
out vec3 ourColor;
void main(){
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourColor = aColor; 
}
  )glsl";      // <-- The closing tag turns it into a valid C++ string variable

//writing our fragment shader for color
const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
void main(){


    vec3 color = ourColor;
    
    
    color = (color - 0.5) * 1.5 + 0.5;
    
    
    color = clamp(color, 0.0, 1.0);
    
    FragColor = vec4(color, 1.0f);
                               
}                                                      
  )glsl";                               //---RGBA   //FragColor = vec4(ourColor, 1.0f);


int main(){

glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

GLFWwindow* window = glfwCreateWindow(800, 600, "What's Up TRIANGLE!", NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
std::cout << "Failed to initialize GLAD" << std::endl;
return -1;
}

glViewport(0, 0, 800, 600);   //tells our rederin window size
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //handles if user resizes the window

glEnable(GL_FRAMEBUFFER_SRGB); // Pre-corrects your math colors for standard monitors!



 //in OpenGL, coordinate system origin at centre of screen,
// and NDC till -1 and 1 in (x,y,z)

 // Vertex Data, three vertices for our triangle
float vertices[] = {
  //positions            //colors
   0.0f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,  // Top Vertex (Red)
  -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // Bottom-Left Vertex (Green)              
   0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,  // Bottom-Left Vertex (Blue)               
};

// Vertex shader(putting data into gpu memory)
unsigned int VBO;
glGenBuffers(1, &VBO);                            // vertex buffer object is obj of opengl(since opengl functions cant be accessed directly)
                                                  // vbo has its ID which we refer using glad, here its glgenbuffer.
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
          
//compile our vertex shader
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

int success;
char infolog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if(!success){
glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
infolog << std::endl;
}

//compiling our fragment shader
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);

glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
if(!success){
glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
infolog << std::endl;
}

//A shader program object is the final linked version of multiple shaders combined
unsigned int shaderprogram;
shaderprogram = glCreateProgram();
glAttachShader(shaderprogram, vertexShader);
glAttachShader(shaderprogram, fragmentShader);
glLinkProgram(shaderprogram);
glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
if(!success) {
glGetProgramInfoLog(shaderprogram, 512, NULL, infolog);
  std::cout << "ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" <<
  infolog << std::endl;
}

glUseProgram(shaderprogram);

glDeleteShader(vertexShader);                  //we dont need them anymore
glDeleteShader(fragmentShader);

//vertex array object VAO
unsigned int VAO;
glGenVertexArrays(1, &VAO);

glBindVertexArray(VAO);
//copy our vertices array in a buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//then set our vertex attributes pointers
//linking the vertex attributes
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);          //to give each vertex unique color


//render loop
while(!glfwWindowShouldClose(window)){

  processInput(window);


  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
 glClear(GL_COLOR_BUFFER_BIT);



  //Drawing our TRIANGLE object!
   // use our shader program when we want to render an object
   glUseProgram(shaderprogram);
   // now draw the object
   glBindVertexArray(VAO);
   glDrawArrays(GL_TRIANGLES, 0, 3);


 
  //check and call events and swap buffers.
  glfwPollEvents();
  glfwSwapBuffers(window);
}

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderprogram);

glfwTerminate();
    return 0;
}