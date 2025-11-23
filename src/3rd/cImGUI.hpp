/**
 * @file cImGUI.hpp
 * @author Patryk
 * @brief cImGUI class declaration
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_IMGUI_HPP
#define SCOP_IMGUI_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_glfw.h"
#include "../../lib/imgui/imgui_impl_opengl3.h"
#include "../core/Camera.hpp"
#include "../core/Object.hpp"

extern Camera gCamera;

/**
 * @brief Wrapper for ImGui to simplify GUI creation.
 *
 * This class provides an easy interface for initializing ImGui, creating frames,
 * displaying text and FPS, and rendering the GUI.
 */
class cImGUI {
public:
    cImGUI();

    void init(GLFWwindow* window);
    void createNewFrame();
    void displayFPS();
    void displayText(const std::string &name, int posx, int posy, const std::string &text);
    void displayHUD(const std::unique_ptr<Object> &object);
    void render();
    void cleanup();

private:
    float m_mainScale;
    ImGuiWindowFlags m_windowFlags;

    void createContext();
    void scale();
    void setStyle();
    void implementBackendGLFWOpenGL(GLFWwindow* window);
};


#endif //SCOP_IMGUI_HPP