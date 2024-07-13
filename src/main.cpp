#include <iostream>
#include <thread>

#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl2.h"

#include <GLFW/glfw3.h>

#include "images.h"
#include "path_tracer_main.h"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

struct VectFloat4 {
    float x;
    float y;
    float z;
    float w;
};

struct VectInt2 {
    int x;
    int y;
};

VectFloat4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};
VectInt2 window_size = {1280, 720};

ImageWithTexture image1;

float progress = 0.0;
bool cancel_rendering = false;
bool rendering_finished = false;
std::thread rendering_thread;

void on_render_pushed();

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(window_size.x, window_size.y, "Path tracing in one weekend", nullptr,
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    io.Fonts->AddFontDefault();

    image1 = create_image_with_texture(256, 256);

    on_render_pushed();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (rendering_finished)
        {
            upload_texture(image1);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("OpenGL Texture Text");
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        availableSize.y -= 25;
        ImGui::Image((void *) (intptr_t) image1.gl_texture, availableSize);
        ImGui::ProgressBar(progress, ImVec2(ImGui::GetFontSize() * 25, 0.0f));
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();


        //
        {
            //     static float f = 0.0f;
            //     static int counter = 0;
            //
            ImGui::Begin("Config"); // Create a window called "Hello, world!" and append into it.
            //
            //     ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
            //     ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            //     // ImGui::Checkbox("Another Window", &show_another_window);
            //
            //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            //     ImGui::ColorEdit3("clear color", (float *) &clear_color); // Edit 3 floats representing a color
            //
            if (ImGui::Button("Render"))
            {
                on_render_pushed();
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    cancel_rendering = true;
    rendering_thread.join();

    delete_image_with_texture(image1);

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void on_render_pushed()
{
    if (rendering_thread.joinable())
    {
        cancel_rendering = true;
        rendering_thread.join();
    }
    cancel_rendering = false;

    rendering_thread = std::thread(render, &progress, &cancel_rendering, &rendering_finished,
                                   image1.buffer);
}
