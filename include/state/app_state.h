//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef APP_STATE_H
#define APP_STATE_H

#include <Eigen/Dense>

struct AppState {
    // Window
    Eigen::Vector4f clear_color = {0.45f, 0.55f, 0.60f, 1.00f};
    Eigen::Vector2i window_size = {1280, 720};

    // Rendering config
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = fmax(1, image_width / aspect_ratio);
    bool live_render = false;
    bool move_camera = false;
    bool move_object = false;
    bool antialiasing = true;
    bool metal_fuzz = true;

    // Rendering status
    float progress = 0.0;
    bool cancel_rendering = false;
    bool rendering_finished = false;
};

#endif // APP_STATE_H
