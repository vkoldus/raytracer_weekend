//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef APP_STATE_H
#define APP_STATE_H

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

struct AppState {
    // Window
    VectFloat4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};
    VectInt2 window_size = {1280, 720};

    // Rendering config
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = fmax(1, image_width / aspect_ratio);
    bool live_render = false;

    // Rendering status
    float progress = 0.0;
    bool cancel_rendering = false;
    bool rendering_finished = false;
};

#endif //APP_STATE_H
