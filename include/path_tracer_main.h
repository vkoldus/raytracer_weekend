//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef PATH_TRACER_MAIN_H
#define PATH_TRACER_MAIN_H

#include <chrono>
#include <iostream>
#include <Eigen/Dense>

#include "interpolation.h"
#include "camera.h"

using namespace std::chrono_literals;
using Eigen::Vector3d;
using Eigen::Vector3i;


using Color = Vector3d;

Color ray_color(const Vector3d &ray_direction)
{
    double a = 0.5 * (ray_direction.normalized()[1] + 1);
    return lerp(a, Color(1.0, 1.0, 1.0), Color(.5, 0.7, 1.0));
}

void render(float *progress_fraction, bool *cancel, bool *finished, uint32_t *buffer, int image_width, int image_height)
{
    Camera camera(1.0, Vector3d(0, 0, 0), 2.0, (double(image_width) / image_height));

    auto pixel_delta_u = camera.viewport.u / image_width;
    auto pixel_delta_v = camera.viewport.v / image_height;

    // Pixel coordinates are defined in their centers
    auto top_left_pixel = camera.viewport_top_left + (pixel_delta_u + pixel_delta_v) / 2;

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = top_left_pixel + i * pixel_delta_u + j * pixel_delta_v;
            auto ray_direction = pixel_center - camera.center;

            Vector3i color = (ray_color(ray_direction) * 255).cast<int>();
            buffer[j * image_width + i] = 0xff << 24 | color(2) << 16 | color(1) << 8 | color(0);


            if (*cancel)
            {
                std::cout << "Rendering cancelled" << std::endl;
                return;
            }
        }

        *progress_fraction = j / (image_height - 1);
        // std::this_thread::sleep_for(1ms);
        // std::this_thread::yield();
    }

    *finished = true;
}


#endif //PATH_TRACER_MAIN_H
