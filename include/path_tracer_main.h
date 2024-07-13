//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef PATH_TRACER_MAIN_H
#define PATH_TRACER_MAIN_H

#include <chrono>
#include <iostream>
#include <Eigen/Dense>

using namespace std::chrono_literals;
using Eigen::Vector3f;
using Eigen::Vector3i;


void render(float *progress_fraction, bool *cancel, bool *finished, uint32_t *buffer, int image_width, int image_height)
{
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            Vector3i color = Vector3f(255.999 * double(i) / (image_width - 1), 255.999 * double(j) / (image_height - 1),
                                      0).cast<int>();
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
