# scop
A 3D object renderer with OpenGL, written in C++

# About
``scop`` is a 3D graphics project that allows parsing and rendering `.obj` files in perspective. The program displays 3D objects with proper rotation around their main axis of symmetry and allows users to move both the objects and the camera freely along all axes. It features a Phong lighting model and supports both colored and textured rendering, with smooth transitions between the two modes.

This project was entirely developed in C++ from scratch, including matrix operations, object parsing, and shader management.

# Features
- Load and render `.obj` 3D models
- Perspective projection with proper depth scaling
- Rotation of objects around their main axis of symmetry
- Object and camera movement:
    - Move objects along X, Y, Z axes
    - Camera rotation (left/right, up/down)
    - Zoom in/out
- Rendering modes:
    - Colored faces with distinguishable colors
    - Textured mode with smooth transition
    - Optional polygon mode (`glPolygonMode`) for wireframe rendering
- Lighting:
    - Phong lighting model implemented in shaders
- UI using ImGUI
- Shader management fully implemented from scratch
- Matrix operations implemented from scratch

# Technical Details
- **Language:** C++
- **Graphics API:** OpenGL
- **Build system:** Makefile
- **External libraries:** GLFW used for window and event management
- No external libraries for `.obj` parsing, matrix math, or shaders

# How to run
## Dependencies
Project requires:
- make
- g++ compiler
- `libgl1-mesa-dev` package

## Download compile and run
Clone the project:
```bash
git clone https://github.com/Zuraw7/scop.git
```

Compile the project:
```bash
make
```

Run the program with `.obj` file and texture
```bash
./scop <path_to_obj_file> <path_to_texture>
```

Project provides basic objects and textures inside `/res` directory