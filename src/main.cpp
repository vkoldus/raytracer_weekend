#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <thread>

#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl2.h"
#include "imgui.h"

#include "path_tracer/rendering_service.h"
#include "state/app_state.h"
#include "ui/config_window.h"
#include "ui/rendering_window.h"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    AppState app_state;

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(app_state.window_size[0],
                                          app_state.window_size[1],
                                          "Path tracing in one weekend",
                                          nullptr,
                                          nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    io.Fonts->AddFontDefault();

    RenderingService rendering_service(app_state);
    rendering_service.render_async();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        rendering_service.loop_hook(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_D)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Q)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_T)),
                                    ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_G)));

        rendering_window(app_state, rendering_service);
        config_window(app_state, rendering_service, io);

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(app_state.clear_color[0],
                     app_state.clear_color[1],
                     app_state.clear_color[2],
                     app_state.clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    rendering_service.shutdown();
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
