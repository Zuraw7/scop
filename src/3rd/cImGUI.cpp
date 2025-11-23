/**
 * @file cImGUI.cpp
 * @author Patryk
 * @brief cImGUI class implementation
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "cImGUI.hpp"

/**
 * @brief Constructs the cImGUI object with default settings.
 *
 * Sets initial GUI scale to 0 and initializes default window flags:
 * - No decoration
 * - Auto-resize
 * - No background
 * - No saved settings
 */
cImGUI::cImGUI() : m_mainScale(0), m_windowFlags(0) {
    m_windowFlags = ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_AlwaysAutoResize |
                    ImGuiWindowFlags_NoBackground |
                    ImGuiWindowFlags_NoSavedSettings;
}

/**
 * @brief Initializes ImGui for the specified GLFW window.
 *
 * Performs the following steps:
 * 1. Creates the ImGui context.
 * 2. Applies DPI scaling.
 * 3. Sets the GUI style (dark theme by default).
 * 4. Initializes the GLFW + OpenGL3 backend for rendering.
 *
 * @param window Pointer to the GLFW window to attach ImGui to.
 */
void cImGUI::init(GLFWwindow *window) {
    createContext();
    scale();
    setStyle();
    implementBackendGLFWOpenGL(window);
}

/**
 * @brief Starts a new ImGui frame.
 *
 * Prepares ImGui for rendering a new frame. Must be called at the
 * beginning of each frame before any ImGui draw calls.
 */
void cImGUI::createNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


/**
 * @brief Displays the current FPS in a small window.
 */
void cImGUI::displayFPS() {
    ImGui::Begin("FPS", nullptr, m_windowFlags & ~ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(10, 10));
    ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
    ImGui::End();
}

/**
 * @brief Displays a custom text window.
 *
 * Creates an ImGui window at the specified position and displays the
 * given text.
 *
 * @param name The name (title) of the ImGui window.
 * @param posx X-coordinate of the window position.
 * @param posy Y-coordinate of the window position.
 * @param text The text content to display inside the window.
 */
void cImGUI::displayText(const std::string &name, int posx, int posy, const std::string &text) {
    ImGui::Begin(name.c_str(), nullptr, m_windowFlags);
    ImGui::SetWindowPos(ImVec2(posx, posy));
    ImGui::Text("%s", text.c_str());
    ImGui::End();
}

/**
 * @brief Displays HUD with useful information
 *
 * HUD shows:
 *  - FPS counter
 *  - movement instructions for both object and camera
 *  - mesh mode control buttons
 *  - object's world position
 *  - camera's world position
 *
 * @param object Reference to the object whose position will be displayed
 */
void cImGUI::displayHUD(const std::unique_ptr<Object> &object) {
    displayFPS();
    displayText("Object", 120, 10, "Move object: WASD + N/M");
    displayText("Camera", 310, 10, "Move camera: ARROW KEYS + SPACE/LEFT SHIFT");
    displayText("Mesh", 640, 10, "Mesh modes: T/P");

    std::string objPos = "Object position: x" + std::to_string(object->getPosition()[0]) + " y " + std::to_string(object->getPosition()[1]) + " z " + std::to_string(object->getPosition()[2]);
    std::string camPos = "Camera position: x" + std::to_string(gCamera.getPosition()[0]) + " y " + std::to_string(gCamera.getPosition()[1]) + " z " + std::to_string(gCamera.getPosition()[2]);
    displayText("Object Pos", 10, HEIGHT - 40, objPos);
    displayText("Camera Pos", WIDTH - 400, HEIGHT - 40, camPos);
}

/**
 * @brief Renders the ImGui draw data to the screen.
 *
 * Must be called after all ImGui windows and draw calls have been created
 * in the current frame. Sends the draw data to OpenGL for rendering.
 */
void cImGUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 * @brief Cleans up ImGui and its backends.
 *
 * Shuts down the OpenGL3 and GLFW backends and destroys the ImGui context.
 * Should be called before the application exits.
 */
void cImGUI::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/**
 * @brief Creates the ImGui context.
 *
 * Checks the ImGui version and initializes a new context for the GUI.
 * Must be called before any other ImGui functions.
 */
void cImGUI::createContext() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}

/**
 * @brief Applies DPI scaling to the GUI.
 *
 * Retrieves the content scale for the primary monitor and scales
 * all ImGui sizes and fonts accordingly.
 */
void cImGUI::scale() {
    float xscale = 1.0f, yscale = 1.0f;

    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    m_mainScale = xscale;

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(m_mainScale);

    ImGui::GetIO().FontGlobalScale = m_mainScale;
}

/**
 * @brief Sets the GUI style to a dark theme.
 *
 * Applies ImGui's built-in dark color scheme. Can be replaced
 * with custom styles if desired.
 */
void cImGUI::setStyle() {
    ImGui::StyleColorsDark();
}

/**
 * @brief Initializes the GLFW and OpenGL3 backends for ImGui.
 *
 * Sets up the OpenGL GLSL version (#version 330) and connects
 * the GLFW backend to ImGui.
 *
 * @param window Pointer to the GLFW window that ImGui will render onto.
 */
void cImGUI::implementBackendGLFWOpenGL(GLFWwindow *window) {
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
}
