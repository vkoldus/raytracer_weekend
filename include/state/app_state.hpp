//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef APP_STATE_H
#define APP_STATE_H

#include <Eigen/Dense>
#include "consts.hpp"

template<typename T>
struct StateValue {
    StateValue<T>(T value)
        : value(value),
          has_changed(true)
    {
    }

    void reset()
    {
        has_changed = false;
    }


    T value;
    bool has_changed;
};

struct AppState {
    // Window
    Eigen::Vector4f clear_color = {0.45f, 0.55f, 0.60f, 1.00f};
    Eigen::Vector2i window_size = {1280, 720};

    // Rendering config
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = std::max(1, int(image_width / aspect_ratio));
    bool live_render = false;
    bool move_camera = false;
    bool move_object = false;
    bool antialiasing = true;
    bool metal_fuzz = true;
    StateValue<float> focus_distance{float(3.4)};
    StateValue<float> vfov_rad{float(20 * Pi / 180.0)};
    StateValue<float> defocus_angle_rad{float(10 * Pi / 180.0)};

    // Rendering status
    float progress = 0.0;
    bool cancel_rendering = false;
    bool rendering_finished = false;
};

#endif // APP_STATE_H
