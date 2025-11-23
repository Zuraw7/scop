/**
 * @file main.cpp
 * @author Patryk
 * @brief
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <iostream>
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/Object.hpp"
#include "core/Camera.hpp"
#include "textures/TextureManager.hpp"
#include "graphics/Shader.hpp"
#include "render/Renderer.hpp"
#include "utils/utils.hpp"
#include "3rd/cImGUI.hpp"

#include "../lib/imgui/imgui.h"

std::unique_ptr<TextureManager> gTextureManager;

Camera gCamera({0.0f, 0.0f, 2.0f},
              {0.0f, 0.0f, 0.0f},
              {0.0f, 1.0f, 0.0f});

void clearExit(GLFWwindow *window, cImGUI &imgui);

int main(int argc, char **argv) {
    if (argc != 3 ) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "./scop <obj_path> <texture_path>\n");
        return 1;
    }

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Configure GLFW to use OpenGL 3.3 Core Profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SCOP", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize glew\n");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    /* Set Callbacks */
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    cImGUI imgui;
    imgui.init(window);

    gTextureManager = std::unique_ptr<TextureManager>(new TextureManager());

    std::unique_ptr<Object> object = Object::create(argv[1]);
    if (!object) {
        clearExit(window, imgui);
        return 1;
    }

    const std::shared_ptr<Texture2D> texture = gTextureManager->loadTexture2D(argv[2]);
    if (!texture) {
        clearExit(window, imgui);
        return 1;
    }
    object->setTexture2D(texture);

    Shader shader("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");

    Renderer renderer;
    renderer.setBackgroundColor(0.3f, 0.13f, 0.01f, 1.0f);

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    double lastFrame = glfwGetTime();
    /* Game loop */
    while (!glfwWindowShouldClose(window))
    {
        const double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, object, renderer, deltaTime);

        /* Render here */
        renderer.clear();

        object->updateRotationMatrixY(deltaTime);

        renderer.draw(object, shader, deltaTime);

        imgui.createNewFrame();
        imgui.displayHUD(object);
        imgui.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    clearExit(window, imgui);
    return 0;
}

void clearExit(GLFWwindow *window, cImGUI &imgui) {
    imgui.cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}
