//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef PATH_TRACER_MAIN_H
#define PATH_TRACER_MAIN_H

#include <chrono>
#include <iostream>
#include <memory>

#include "camera.hpp"
#include "gamma.hpp"
#include "interpolation.hpp"
#include "materials/material.hpp"
#include "objects.hpp"
#include "state/app_state.hpp"
#include "types.hpp"
#include "world.hpp"

using namespace std::chrono_literals;

std::vector<Vector2> aa_sampling_offsets;


void reinitialize_aa_if_needed(const AppState &app_state)
{
    if (app_state.live_render && aa_sampling_offsets.size() != app_state.aa_samples_per_pixel_low_q)
    {
        aa_sampling_offsets.clear();
        for (unsigned int i = 0; i < app_state.aa_samples_per_pixel_low_q; i++)
        {
            aa_sampling_offsets.push_back(Vector2::Random() / 2);
        }
    }

    if (!app_state.live_render && aa_sampling_offsets.size() != app_state.aa_samples_per_pixel)
    {
        aa_sampling_offsets.clear();
        for (unsigned int i = 0; i < app_state.aa_samples_per_pixel; i++)
        {
            aa_sampling_offsets.push_back(Vector2::Random() / 2);
        }
    }
}

Vector3 sample_from_defocus_disk(Camera &camera)
{
    if (camera.defocus_angle_rad > 0)
    {
        auto p = random_in_unit_disk();
        return camera.center() + (p[0] * camera.defocus_disk_u) + (p[1] * camera.defocus_disk_v);
    } else
    {
        return camera.center();
    }
}

Color ray_color(const World &objects, const Ray &ray, int depth)
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
        Ray scattered;
        Color color_attenuation;
        if (hit.material->scatter(ray, hit, color_attenuation, scattered))
        {
            return color_attenuation.cwiseProduct(ray_color(objects, scattered, depth - 1));
        } else
        {
            return {0, 0, 0};
        }

        // return 0.5 * ray_color(objects, Ray{hit.p, hit.normal + random_on_unit_sphere()}, depth - 1);
        // return 0.5 * Color(hit.normal[0] + 1, hit.normal[1] + 1, hit.normal[2] + 1);
    }

    // Gradient for background
    double a = 0.5 * (ray.direction.normalized()[1] + 1);
    return lerp(a, Color(1.0, 1.0, 1.0), Color(.5, 0.7, 1.0));
}

void render(AppState *app_state, const World *world, Camera *camera, uint32_t *buffer, size_t line_start,
            size_t line_end)
{
    reinitialize_aa_if_needed(*app_state);
    int max_depth = 50;

    auto pixel_delta_u = camera->viewport_u / app_state->image_width;
    auto pixel_delta_v = camera->viewport_v / app_state->image_height;

    // Pixel coordinates are defined in their centers
    auto top_left_pixel = camera->viewport_top_left + (pixel_delta_u + pixel_delta_v) / 2;

    Vector3i color;
    for (size_t j = line_start; j < line_end; j++)
    {
        for (size_t i = 0; i < size_t(app_state->image_width); i++)
        {
            auto pixel_center = top_left_pixel + i * pixel_delta_u + j * pixel_delta_v;

            if (app_state->antialiasing)
            {
                color = {0, 0, 0};

                for (size_t k = 0; k < aa_sampling_offsets.size(); k++)
                {
                    auto ray_origin = sample_from_defocus_disk(*camera);
                    Ray ray{ray_origin,
                            (pixel_center + Vector3d{pixel_delta_u[0] * aa_sampling_offsets[k][0],
                                                     pixel_delta_v[1] * aa_sampling_offsets[k][1],
                                                     0.0}) -
                                    ray_origin};
                    color += (linear_to_gamma(ray_color(*world, ray, max_depth)) * 255).cast<int>();
                }
                color = (color / aa_sampling_offsets.size()).cast<int>();
            } else
            {
                Ray ray{camera->center(), pixel_center - camera->center()};
                color = (linear_to_gamma(ray_color(*world, ray, max_depth)) * 255).cast<int>();
            }

            buffer[j * (size_t) app_state->image_width + i] =
                    (uint32_t) ((uint8_t) 0xff << 24 | (uint8_t) color(2) << 16 | (uint8_t) color(1) << 8 |
                                (uint8_t) color(0));

            if (app_state->cancel_rendering)
            {
                std::cout << "Rendering cancelled" << std::endl;
                return;
            }
        }

        app_state->progress = float(j / (app_state->image_height - 1.0));
        // std::this_thread::sleep_for(10ms);
        // std::this_thread::yield();
    }

    app_state->rendering_finished = true;
}


#endif // PATH_TRACER_MAIN_H
