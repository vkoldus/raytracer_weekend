//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef CONFIG_WINDOW_H
#define CONFIG_WINDOW_H

#include <imgui.h>
#include "path_tracer/rendering_service.hpp"
#include "state/app_state.hpp"

void config_window(AppState &app_state, RenderingService &rendering_service, const ImGuiIO &io)
{
    ImGui::Begin("Config");

    ImGui::Checkbox("Live render", &app_state.live_render);
    ImGui::Checkbox("Move camera", &app_state.move_camera);
    ImGui::Checkbox("Move object", &app_state.move_object);
    ImGui::Checkbox("Anti-aliasing", &app_state.antialiasing);
    ImGui::Checkbox("Metal fuzz", &app_state.metal_fuzz);

    ImGui::SliderAngle("VFoV", &app_state.vfov_rad, 20, 120);

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

    if (ImGui::Button("Save image"))
    {
        rendering_service.save_next_image_to("ray_tracer.png");
    }
    ImGui::Text("%.3f ms/f (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("");
    ImGui::Text("Camera controls in live render mode");
    ImGui::Text("-----------------------------------");
    ImGui::Text("WSAD - move camera");
    ImGui::Text("QE - strafe");
    ImGui::Text("RFTG - Fly, pitch");
    ImGui::End();
}
#endif // CONFIG_WINDOW_H
