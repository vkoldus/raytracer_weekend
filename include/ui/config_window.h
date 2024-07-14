//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef CONFIG_WINDOW_H
#define CONFIG_WINDOW_H

#include <imgui.h>
#include "state/app_state.h"
#include "path_tracer/rendering_service.h"

void config_window(AppState &app_state, RenderingService &rendering_service, const ImGuiIO &io)
{
    ImGui::Begin("Config");
    //
    //     ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
    ImGui::Checkbox("Live render", &app_state.live_render);
    ImGui::Checkbox("Move camera", &app_state.move_camera);
    ImGui::Checkbox("Move object", &app_state.move_object);
    ImGui::Checkbox("Anti-aliasing", &app_state.antialiasing);
    //     // ImGui::Checkbox("Another Window", &show_another_window);
    //
    //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
    //     ImGui::ColorEdit3("clear color", (float *) &clear_color); // Edit 3 floats representing a color
    //

    if (app_state.live_render)
    {
        rendering_service.render_sync();
    } else
    {
        if (ImGui::Button("Render"))
        {
            rendering_service.render_async();
        }
    }

    ImGui::Text("%.3f ms/f (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
#endif //CONFIG_WINDOW_H
