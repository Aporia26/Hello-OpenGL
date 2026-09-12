# MY OPENGL PRACTICE
# 🌌 Hello, OpenGL!

A step-by-step progression of graphics pipeline milestones, documenting my journey into low-level rendering context execution, custom GLSL shader compilation, and coordinate texture mapping.

> ⚠️ **Note on Compiling:** This repository uses localized absolute directory paths for asset parsing and graphics dependencies. To build this on your local environment, ensure your `.vscode/tasks.json` and asset paths match your system setup.


---

## 🗺️ Learning Roadmap & Visual Milestones

Every file inside the `src/` directory represents a fully isolated and functional graphic rendering stage, linked to its corresponding GPU output viewport:

*   **`Window_GLFW.cpp`**
    *   *Technical Focus:* Initializing the GLFW environment, configuring viewport dimensions, managing Glad graphics context pointers, and constructing the fundamental event-polling render loop.
    *   *Output Output Preview:* *(Spins up a clean, responsive double-buffered empty window frame).*

*   **`Triangle_Shader.cpp`**
    *   *Technical Focus:* Stepping into the GPU pipeline. Generating Vertex Buffer Objects (VBO), configuring Vertex Array Objects (VAO), and executing hardcoded vertex/fragment shader pipelines.
    *   *Render Output:*
        <br><img src="images/Triangle.png" width="50%" alt="OpenGL Triangle Render" /><br>

*   **`EBO.cpp`**
    *   *Technical Focus:* Optimizing index configurations by implementing Element Buffer Objects (EBO) to render a quad cleanly while reducing vertex memory redundancies.
    *   *Render Output:*
        <br><img src="images/EBO.png" width="50%" alt="OpenGL EBO Quad Render" /><br>

*   **`More_Shaders.cpp`**
    *   *Technical Focus:* Experimenting with vertex attributes, passing dynamic color data down the graphics pipeline, and playing with custom fragment shader math.
    *   *Render Output:*
        <br><img src="images/More_Shaders.png" width="50%" alt="OpenGL Gradient Shaders Render" /><br>

*   **`shader.h`**
    *   *Technical Focus:* A custom header class blueprint built to automate reading, compiling, and linking GLSL shader source code files smoothly.

*   **`Textures.cpp`**
    *   *Technical Focus:* Experimenting with Texture generation, passing dynamic color data down the graphics pipeline, and playing with custom fragment shader math, and stb_image.h.
    *   *Render Output:*
        <br><img src="images/doorTexture.png" width="50%" alt="OpenGL Texture Render" /><br>
        <br><img src="images/doorTextureRGB.png" width="50%" alt="OpenGL Texture with funky Render" /><br>
        <br><img src="images/doubleTexture.png" width="50%" alt="OpenGL double Texture Render" /><br>
        
*   **`Transformation.cpp`**
    *   *Technical Focus:* Applying the vectors and matrix calculations and math with glm to get transformation on the rendered shape, understanding glm.
    *  *Render Output:*
       <br><img src="images/rotatingFace.gif" alt="Rotation" width="60%" /><br>

*   **`Coordinate_sys.cpp`**
    *   *Technical Focus:* Understood different coordinates spaces like local,world,view,clip ect, there significanceand conversion, and how projections works..
    *  *Render Output:*
       
       <br><img src="images/Perspective.png" alt="Rotation" width="40%" /><br>
       <img src="images/3D_cubeRender.gif" alt="Rotation" width="40%" />
      <br><img src="images/Z-buffer.gif" alt="Rotation" width="40%" /><br>
      <img src="images/Manycubes.png" alt="Rotation" width="50%" />

*   **`Fly_Camera.cpp`**
    *   *Technical Focus:* understanding camera vectors, how the LookAt matrix works, GLFW mouse inputs and how to capture the mouse and tracking. Also, understood view matrices in more depth.
    *   *Output Output Preview:* *(shows a smooth mouse track and wasd movement flying in the 3d environment).*




---

## 🛠️ Environment & Toolkit
- **Language Architecture:** C++ (ISO Standard)
- **Graphics API:** OpenGL (Core Profile Layout)
- **Context Handling:** GLFW 3 + GLAD

---
*Documenting my computer graphics and low-level pipeline foundations during Semester 1 of B.Tech CSE.*
