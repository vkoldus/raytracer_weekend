//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef PATH_TRACER_MAIN_H
#define PATH_TRACER_MAIN_H

#include <chrono>
#include <iostream>
#include <memory>


#include "types.h"
#include "interpolation.h"
#include "camera.h"
#include "objects.h"

using namespace std::chrono_literals;
using World = std::vector<std::shared_ptr<Hittable> >;

Color ray_color(const std::vector<std::shared_ptr<Hittable> > &objects, const Ray &ray)
{
    HitInfo hit;
    Interval ray_interval = {0, +Infinity};

    for (auto o: objects)
    {
        if (o->hit(ray, ray_interval, hit))
        {
            ray_interval.max = hit.t;
        }
    }

    if (ray_interval.max < Infinity)
    {
        return 0.5 * Color(hit.normal[0] + 1, hit.normal[1] + 1, hit.normal[2] + 1);
    } else
    {
        // Gradient for background
        double a = 0.5 * (ray.direction.normalized()[1] + 1);
        return lerp(a, Color(1.0, 1.0, 1.0), Color(.5, 0.7, 1.0));
    }
}

void render(AppState *app_state, const World *world, Camera *camera, uint32_t *buffer)
{
    auto pixel_delta_u = camera->viewport.u / app_state->image_width;
    auto pixel_delta_v = camera->viewport.v / app_state->image_height;

    // Pixel coordinates are defined in their centers
    auto top_left_pixel = camera->viewport_top_left + (pixel_delta_u + pixel_delta_v) / 2;


    for (int j = 0; j < app_state->image_height; j++)
    {
        for (int i = 0; i < app_state->image_width; i++)
        {
            auto pixel_center = top_left_pixel + i * pixel_delta_u + j * pixel_delta_v;
            Ray ray{camera->center, pixel_center - camera->center};

            Vector3i color = (ray_color(*world, ray) * 255).cast<int>();
            buffer[j * app_state->image_width + i] = 0xff << 24 | color(2) << 16 | color(1) << 8 | color(0);


            if (app_state->cancel_rendering)
            {
                std::cout << "Rendering cancelled" << std::endl;
                return;
            }
        }

        app_state->progress = j / (app_state->image_height - 1.0);
        // std::this_thread::sleep_for(10ms);
        // std::this_thread::yield();
    }

    app_state->rendering_finished = true;
}


#endif //PATH_TRACER_MAIN_H
