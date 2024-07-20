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
#include "random.h"
#include "gamma.h"

using namespace std::chrono_literals;
using World = std::vector<std::shared_ptr<Hittable> >;

std::vector<Vector2> aa_sampling_offsets;

auto hi_q_aa_samples = 100;

void reinitialize_aa_if_needed(const AppState &app_state)
{
    if (app_state.live_render && aa_sampling_offsets.size() != 4)
    {
        aa_sampling_offsets.clear();
        for (int i = 0; i < 4; i++)
        {
            aa_sampling_offsets.push_back(Vector2::Random() / 2);
        }
    }

    if (!app_state.live_render && aa_sampling_offsets.size() != hi_q_aa_samples)
    {
        aa_sampling_offsets.clear();
        for (int i = 0; i < hi_q_aa_samples; i++)
        {
            aa_sampling_offsets.push_back(Vector2::Random() / 2);
        }
    }
}


Color ray_color(const std::vector<std::shared_ptr<Hittable> > &objects, const Ray &ray, int depth)
{
    HitInfo hit;
    Interval ray_interval = {0.001, +Infinity};

    if (depth <= 0)
    {
        return Color(0.0, 0.0, 0.0);
    }

    for (auto o: objects)
    {
        if (o->hit(ray, ray_interval, hit))
        {
            ray_interval.max = hit.t;
        }
    }

    if (ray_interval.max < Infinity)
    {
        return 0.5 * ray_color(objects, Ray{hit.p, hit.normal + random_on_unit_sphere()}, depth - 1);
        // return 0.5 * Color(hit.normal[0] + 1, hit.normal[1] + 1, hit.normal[2] + 1);
    } else
    {
        // Gradient for background
        double a = 0.5 * (ray.direction.normalized()[1] + 1);
        return lerp(a, Color(1.0, 1.0, 1.0), Color(.5, 0.7, 1.0));
    }
}


void render(AppState *app_state, const World *world, Camera *camera, uint32_t *buffer)
{
    reinitialize_aa_if_needed(*app_state);
    int max_depth = 50;


    auto pixel_delta_u = camera->viewport.u / app_state->image_width;
    auto pixel_delta_v = camera->viewport.v / app_state->image_height;

    // Pixel coordinates are defined in their centers
    auto top_left_pixel = camera->viewport_top_left + (pixel_delta_u + pixel_delta_v) / 2;

    Vector3i color;
    for (int j = 0; j < app_state->image_height; j++)
    {
        for (int i = 0; i < app_state->image_width; i++)
        {
            auto pixel_center = top_left_pixel + i * pixel_delta_u + j * pixel_delta_v;

            if (app_state->antialiasing)
            {
                color = {0, 0, 0};

                for (int i = 0; i < aa_sampling_offsets.size(); i++)
                {
                    Ray ray{
                        camera->center,
                        (pixel_center + Vector3d{
                             pixel_delta_u[0] * aa_sampling_offsets[i][0], pixel_delta_v[1] * aa_sampling_offsets[i][1],
                             0.0
                         }) - camera->center
                    };
                    color += (linear_to_gamma(ray_color(*world, ray, max_depth)) * 255).cast<int>();
                }
                color = (color / aa_sampling_offsets.size()).cast<int>();
            } else
            {
                Ray ray{camera->center, pixel_center - camera->center};
                color = (linear_to_gamma(ray_color(*world, ray, max_depth)) * 255).cast<int>();
            }

            color = color;

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
