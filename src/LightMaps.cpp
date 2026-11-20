#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

 #define STB_IMAGE_IMPLEMENTATION
 #include "stb_image.h"

#include <iostream>
#include <vector>
#include <cmath>

#include "../shaders/Shader.hpp"

// --- FORCE NVIDIA DRIVER HIGH PERFORMANCE SPECS ---
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// Screen dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallBack(GLFWwindow* window, double xpos, double ypos);
void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);
unsigned int createBlackTexture();

// Camera globals 
float pitch = -10.0f;
float yaw   = -90.0f; 
float fov   = 45.0f;  

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Frame Timing 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 1.5f, 3.6f);  
glm::vec3 cameraFront = glm::vec3(0.0f, -0.17f, -0.98f); 
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);  

// Light position 
glm::vec3 lightPos(1.5f, 1.4f, 1.5f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

    GLFWwindow* Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Night Walker!", NULL, NULL);
    if (Window == NULL)
    {
        std::cerr << "Failed to Load GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, FrameBufferSizeCallBack);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Window, MouseCallBack);
    glfwSetScrollCallback(Window, ScrollCallBack);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to Load GLAD!\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "\n==================================================\n";
    std::cout << " GPU Hardware Layer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << " OpenGL Version:     " << glGetString(GL_VERSION) << "\n";
    std::cout << "==================================================\n";
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);   
    glEnable(GL_DEPTH_TEST);

    // Load Shaders
    Shader lamp("shaders/lightcube.vs", "shaders/lightcube.fs");
    Shader VHS("shaders/lightmap.vs", "shaders/lightmap.fs");
    Shader solidShader("shaders/solid.vs", "shaders/solid.fs");

    // 24-Vertex Cube for EBO (used for VHS Tape, Table, and Light Cube)
  float CubeVertices[] = {
        // Positions          // Normals           // Texture Coords (U, V)
        // 0-3: Front face (+Z -> Main VHS Cover)
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,

        // 4-7: Back face (-Z -> Main VHS Cover)
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

        // 8-11: Left face (-X -> Left Grip Strip in side atlas: u 0.00..0.10, v 0.01..0.99)
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.00f, 0.01f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.10f, 0.01f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.10f, 0.99f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.00f, 0.99f,

        // 12-15: Right face (+X -> Right Spine Label Strip in side atlas: u 0.90..1.00, v 0.01..0.99)
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.90f, 0.01f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.00f, 0.01f,
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.00f, 0.99f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.90f, 0.99f,

        // 16-19: Top face (+Y -> Top Tape Flap Strip in side atlas: u 0.12..0.88, v 0.01..0.28)
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.12f, 0.01f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.88f, 0.01f,
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.88f, 0.28f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.12f, 0.28f,

        // 20-23: Bottom face (-Y -> Screw Casing Strip in side atlas: u 0.12..0.88, v 0.41..0.80)
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.12f, 0.41f,
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.88f, 0.41f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.88f, 0.80f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.12f, 0.80f
    };

    // Order indices so Front & Back (first 12) and the 4 Sides (next 24) can be drawn separately!
    unsigned int indices1[] = {
        // Front & Back Faces (12 indices)
         0,  1,  2,   2,  3,  0,
         4,  5,  6,   6,  7,  4,

        // 4 Side Edges (24 indices)
         8,  9, 10,  10, 11,  8, // Left
        12, 13, 14,  14, 15, 12, // Right
        16, 17, 18,  18, 19, 16, // Top
        20, 21, 22,  22, 23, 20  // Bottom
    };

    // Configure VAO, VBO, and EBO 
    unsigned int VBO, EBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Configure Light Cube VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Load Diffuse, Specular, and Emission Maps
    unsigned int diffuseMap  = loadTexture("texture/VHS_diff.png");
    unsigned int specularMap = loadTexture("texture/VHS_spec.png");
    unsigned int emissionMap = loadTexture("texture/VHS_emis.png");

    // Load Side Edges Textures (Diffuse, Specular) + 1x1 Black Dummy Emission
    unsigned int sideDiffuseMap = loadTexture("texture/VHS_sidediff.png");
    unsigned int sideSpecularMap= loadTexture("texture/VHS_sidespec.png");
    unsigned int blackEmisMap   = createBlackTexture();

    // Assign texture units to VHS shader
    VHS.use(); 
    VHS.setInt("material.diffuse",  0);
    VHS.setInt("material.specular", 1);
    VHS.setInt("material.emission", 2);

    // Liminal Ground Plane Geometry (Flat room floor at Y = -0.5f)
    float PlaneVertices[] = {
        // positions             // normals
        -25.0f, -0.5f, -25.0f,   0.0f, 1.0f, 0.0f,
         25.0f, -0.5f, -25.0f,   0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f,  25.0f,   0.0f, 1.0f, 0.0f,
         25.0f, -0.5f,  25.0f,   0.0f, 1.0f, 0.0f
    };

    unsigned int indices2[] = {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int PlaneVAO, PlaneVBO, PlaneEBO;
    glGenVertexArrays(1, &PlaneVAO);
    glGenBuffers(1, &PlaneVBO);
    glGenBuffers(1, &PlaneEBO);

    glBindVertexArray(PlaneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, PlaneVBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertices), PlaneVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PlaneEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(Window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(Window);

        // Orbit the light cube around VHS tape
        float orbitRadius = 1.9f;
        float orbitSpeed  = 0.8f;
        lightPos.x = sin(currentFrame * orbitSpeed) * orbitRadius;
        lightPos.z = cos(currentFrame * orbitSpeed) * orbitRadius;
        lightPos.y = 1.1f + sin(currentFrame * 1.5f) * 0.35f; // vertical wave

        glClearColor(0.06f, 0.07f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // ============================================================
        // DRAW VHS CASSETTE 
        // ============================================================
        VHS.use();
        VHS.setVec3("light.position", lightPos);
        VHS.setVec3("viewPos", cameraPos);

        VHS.setVec3("light.ambient",  0.20f, 0.20f, 0.20f); 
        VHS.setVec3("light.diffuse",  0.75f, 0.75f, 0.75f);
        VHS.setVec3("light.specular", 1.0f,  1.0f,  1.0f);
        VHS.setFloat("material.shininess", 64.0f);

        VHS.setMat4("projection", projection);
        VHS.setMat4("view", view);

        // Standing vertically on its bottom edge on top of the table:
        // Table top is at Y = 0.1f. Standing height is 1.04f (half-height = 0.52f).
        // Center Y = 0.1f + 0.52f = 0.62f so it sits flush on the table!
        glm::mat4 modelVHS = glm::mat4(1.0f);
        modelVHS = glm::translate(modelVHS, glm::vec3(0.0f, 0.62f, 0.0f));
        modelVHS = glm::rotate(modelVHS, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelVHS = glm::scale(modelVHS, glm::vec3(1.88f, 1.04f, 0.25f)); // Width x Height x Thickness
        VHS.setMat4("model", modelVHS);
        glBindVertexArray(cubeVAO);

        // Bind diffuse, specular, and emission maps
        // A: Draw Front & Back Faces (First 12 indices) with Main Textures + Emission
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);
        // B: Draw 4 Side Edges (Remaining 24 indices) with Side Textures + No Emission
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sideDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sideSpecularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, blackEmisMap);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

        // ============================================================
        // DRAW LIMINAL FLOOR & TABLE 
        // ============================================================
        solidShader.use();
        solidShader.setVec3("light.position", lightPos);
        solidShader.setVec3("viewPos", cameraPos);
        solidShader.setVec3("light.ambient", 0.25f, 0.26f, 0.24f);
        solidShader.setVec3("light.diffuse", 0.65f, 0.65f, 0.60f);
        solidShader.setVec3("light.specular", 0.30f, 0.30f, 0.30f);
        solidShader.setMat4("projection", projection);
        solidShader.setMat4("view", view);

        // A. Ground Floor 
        solidShader.setVec3("objectColor", 0.56f, 0.58f, 0.52f);
        glm::mat4 modelPlane = glm::mat4(1.0f);
        solidShader.setMat4("model", modelPlane);
        glBindVertexArray(PlaneVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // B. Rectangular Table (Floor is at -0.5f, height is 0.6f -> center at -0.2f, top at +0.1f)
        solidShader.setVec3("objectColor", 0.26f, 0.22f, 0.19f);
        glm::mat4 modelTable = glm::mat4(1.0f);
        modelTable = glm::translate(modelTable, glm::vec3(0.0f, -0.2f, 0.0f));
        modelTable = glm::scale(modelTable, glm::vec3(3.2f, 0.6f, 2.2f));
        solidShader.setMat4("model", modelTable);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // ============================================================
        // DRAW LIGHT CUBE
        // ============================================================
        lamp.use();
        lamp.setMat4("projection", projection);
        lamp.setMat4("view", view);
        glm::mat4 modelLamp = glm::mat4(1.0f);
        modelLamp = glm::translate(modelLamp, lightPos);
        modelLamp = glm::scale(modelLamp, glm::vec3(0.15f));
        lamp.setMat4("model", modelLamp);
        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &PlaneVAO);
        glDeleteBuffers(1, &PlaneVBO);
        glDeleteBuffers(1, &PlaneEBO);

    glfwTerminate();
    return 0;
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data)
    {
    GLenum format = GL_RGB;

    if (nrComponents == 1)      format = GL_RED;
    else if (nrComponents == 3) format = GL_RGB;
    else if (nrComponents == 4) format = GL_RGBA;
         glBindTexture(GL_TEXTURE_2D, textureID);
         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(GL_TEXTURE_2D);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         stbi_image_free(data);
    }
    else
    {
         std::cout << "Texture failed to load at path: " << path << std::endl;
         stbi_image_free(data);
    }
    return textureID;
}
// Creates a tiny 1x1 pure black texture so side edges don't glow
unsigned int createBlackTexture()
{
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    unsigned char blackPixel[3] = { 0, 0, 0 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, blackPixel);
    return texID;
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 2.5f * deltaTime;
    glm::vec3 fpsForward = glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
    if (glm::length(fpsForward) > 0.0f) {
        fpsForward = glm::normalize(fpsForward);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * fpsForward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * fpsForward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(fpsForward, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(fpsForward, cameraUp)) * cameraSpeed;
}

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void MouseCallBack(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= static_cast<float>(yoffset);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
