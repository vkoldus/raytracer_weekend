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


Color ray_color(const std::vector<std::shared_ptr<Hittable> > &objects, const Ray &ray)
{
    HitInfo hit;
    for (auto o: objects)
    {
        if (o->hit(ray, hit))
        {
            return 0.5 * Color(hit.normal[0] + 1, hit.normal[1] + 1, hit.normal[2] + 1);
        }
    }

    // Gradient for background
    double a = 0.5 * (ray.direction.normalized()[1] + 1);
    return lerp(a, Color(1.0, 1.0, 1.0), Color(.5, 0.7, 1.0));
}

void render(float *progress_fraction, bool *cancel, bool *finished, uint32_t *buffer, int image_width, int image_height)
{
    Camera camera(1.0, Vector3(0, 0, 0), 2.0, (double(image_width) / image_height));

    auto pixel_delta_u = camera.viewport.u / image_width;
    auto pixel_delta_v = camera.viewport.v / image_height;

    // Pixel coordinates are defined in their centers
    auto top_left_pixel = camera.viewport_top_left + (pixel_delta_u + pixel_delta_v) / 2;

    // Scene
    auto x = sinf((float) ImGui::GetTime());
    // auto x = 0.0;

    std::vector<std::shared_ptr<Hittable> > objects = {
        std::make_shared<Sphere>(Point3(x, 0, -1), 0.5),
        std::make_shared<Sphere>(Point3(-2.5, -0.5, -2), 0.3),
    };


    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = top_left_pixel + i * pixel_delta_u + j * pixel_delta_v;
            Ray ray{camera.center, pixel_center - camera.center};

            Vector3i color = (ray_color(objects, ray) * 255).cast<int>();
            buffer[j * image_width + i] = 0xff << 24 | color(2) << 16 | color(1) << 8 | color(0);


            if (*cancel)
            {
                std::cout << "Rendering cancelled" << std::endl;
                return;
            }
        }

        *progress_fraction = j / (image_height - 1.0);
        // std::this_thread::sleep_for(10ms);
        // std::this_thread::yield();
    }

    *finished = true;
}


#endif //PATH_TRACER_MAIN_H
