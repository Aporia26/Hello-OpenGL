#include<glad/glad.h>       
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

// EBO - Element Buffer Object 

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)){
    glfwSetWindowShouldClose(window, true);
  }
}

// 1. SHADERS (Support both position and color tracks)
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 
out vec3 ourColor;
void main(){
  gl_Position = vec4(aPos, 1.0);
  ourColor = aColor; 
}
)glsl";      

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
void main(){
  FragColor = vec4(ourColor, 1.0f);                             
}                                                      
)glsl";                               


int main(){
    // 2. INITIALIZATION & WINDOW SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL EBO Quad", NULL, NULL);
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

    glViewport(0, 0, 800, 600);   
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    // 3. COMPILE AND LINK SHADERS
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexShader);
    glAttachShader(shaderprogram, fragmentShader);
    glLinkProgram(shaderprogram);

    glDeleteShader(vertexShader);                  
    glDeleteShader(fragmentShader);

    // 4. COMBINED VERTEX DATA (Positions X,Y,Z + Colors R,G,B for 4 corners)
    float rectvertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Top Right (Red)
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Right (Green)
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Left (Blue)
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f   // Top Left (Yellow)
    };
    
    unsigned int indices[] = {
        0, 1, 3, // First triangle (Top-Right, Bottom-Right, Top-Left)
        1, 2, 3  // Second triangle (Bottom-Right, Bottom-Left, Top-Left)
    };

    // 5. THE VAO, VBO, & EBO STORAGE SETUP
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO First! It records all downstream VBO and EBO attachments
    glBindVertexArray(VAO);

    // Bind and fill Vertex Data (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectvertices), rectvertices, GL_STATIC_DRAW);

    // Bind and fill Index Data (EBO) - VAO records this connection automatically!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Track 0: Positions Layout Map (Stride: 6 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Track 1: Colors Layout Map (Stride: 6 floats, Offset: 3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO safely to preserve settings
    glBindVertexArray(0);

    // 6. THE ACTIVE RENDER LOOP
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        // Clear canvas
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Execution: Activate pipeline, mount VAO container, and draw indices
        glUseProgram(shaderprogram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draws the 2 joined triangles!

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // 7. CLEAN UP
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderprogram);

    glfwTerminate();
    return 0;
} 