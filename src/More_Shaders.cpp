#include<glad/glad.h>       
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

// ***************************************** SHADERS *********************************************************

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)){
    glfwSetWindowShouldClose(window,true);
  }
}
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos; // position has attribute position 0
void main()
{
gl_Position = vec4(aPos, 1.0); // we give a vec3 to vec4’s constructor
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;
void main()
{
FragColor = ourColor;
}
)glsl";

int main(){

glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

GLFWwindow* window = glfwCreateWindow(800, 600, "SHADERS, YAY!", NULL, NULL);
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

float vertices[] = {
  0.0f, 0.5f, 0.0,
 -0.5f, -0.5f, 0.0f,
  0.5f,-0.5f, 0.0f
};

    // 5. THE VAO, VBO, STORAGE SETUP
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO First! It records all downstream VBO and EBO attachments
    glBindVertexArray(VAO);

    // Bind and fill Vertex Data (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Track 0: Positions Layout Map 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

//int nrAttributes;
//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
//<< std::endl;

// 3. COMPILE AND LINK SHADERS
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
   
glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
if(!success){
glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
infolog << std::endl;
}

    unsigned int shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexShader);
    glAttachShader(shaderprogram, fragmentShader);
    glLinkProgram(shaderprogram);
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
if(!success) {
glGetProgramInfoLog(shaderprogram, 512, NULL, infolog);
  std::cout << "ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" <<
  infolog << std::endl;
}

    glDeleteShader(vertexShader);                  
    glDeleteShader(fragmentShader);

    
  // Unbind VAO safely to preserve settings
    glBindVertexArray(0);

// 6. THE ACTIVE RENDER LOOP
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        // Clear canvas
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderprogram);

        //chaging color with time using " UNIFORMS! "
      float timevalue = glfwGetTime();
      float greenvalue = (sin(timevalue)/2.0f) + 0.5f;   //vary color from 0-1 using sin
      int vertexColorLocation = glGetUniformLocation(shaderprogram, "ourColor");
      glUniform4f(vertexColorLocation, 0.0f, greenvalue, 0.0f, 1.0f);

      // now render the triangle
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

   glfwTerminate();
   return 0;
} 