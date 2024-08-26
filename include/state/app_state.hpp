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


template<typename T>
struct SyncedPrimitive {
    T value;
    std::mutex _mutex;

    SyncedPrimitive(T&& value): value(value) {
    }

    void operator=(T new_value) {
        std::unique_lock<std::mutex> l(_mutex);
        value = new_value;
    }

    bool operator==(T lval) {
        return value == lval;
    }

    void increase() {
        std::unique_lock<std::mutex> l(_mutex);
        this->value++;
    }
};

struct AppState {
    // Window
    Eigen::Vector4f clear_color = {0.45f, 0.55f, 0.60f, 1.00f};
    Eigen::Vector2i window_size = {1280, 720};

    // Dev config
    const int image_width = 400;
    const unsigned int aa_samples_per_pixel = 50;
    const unsigned int aa_samples_per_pixel_low_q = 4;

    StateValue<float> focus_distance{float(3.4)};
    StateValue<float> vfov_rad{float(20 * Pi / 180.0)};
    StateValue<float> defocus_angle_rad{float(10 * Pi / 180.0)};

    const Vector3 initial_lookfrom = Point3(-2, 2, 1);
    const Vector3 initial_lookat = Point3(0, 0, -1);
    const Vector3 initial_vup = Vector3(0, 1, 0);

    // Demo config
    // const int image_width = 1200;
    // const unsigned int aa_samples_per_pixel = 500;

    // StateValue<float> focus_distance{float(10)};
    // StateValue<float> vfov_rad{float(20 * Pi / 180.0)};
    // StateValue<float> defocus_angle_rad{float(0.6 * Pi / 180.0)};

    // const Vector3 initial_lookfrom = Point3(13, 2, 3);
    // const Vector3 initial_lookat = Point3(0, 0, 0);
    // const Vector3 initial_vup = Vector3(0, 1, 0);

    // Rendering config
    double aspect_ratio = 16.0 / 9.0;
    int image_height = std::max(1, int(image_width / aspect_ratio));
    bool live_render = false;
    bool move_camera = false;
    bool move_object = false;
    bool antialiasing = true;
    bool metal_fuzz = true;

    // Rendering status
    SyncedPrimitive<int> progress = 0;
    bool cancel_rendering = false;
};

#endif // APP_STATE_H
