//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef RENDERING_WINDOW_H
#define RENDERING_WINDOW_H

#include <imgui.h>
#include "path_tracer/rendering_service.h"
#include "state/app_state.h"

void rendering_window(AppState &app_state, const RenderingService &rendering_service)
{
    // No borders
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Path tracer output");

    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    availableSize.y = (float) (availableSize.x / app_state.aspect_ratio);

    ImGui::Image((void *) (intptr_t) rendering_service.gl_texture(), availableSize);
    ImGui::ProgressBar(app_state.progress, ImVec2(ImGui::GetFontSize() * 25, 0.0f));

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

#endif // RENDERING_WINDOW_H
