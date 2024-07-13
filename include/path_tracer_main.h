//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef PATH_TRACER_MAIN_H
#define PATH_TRACER_MAIN_H

#include <thread>
#include <chrono>
#include <iostream>
#include <Eigen/Dense>

using namespace std::chrono_literals;
using Eigen::Vector3f;
using Eigen::Vector3i;

void render(float *progress_fraction, bool *cancel, bool *finished, uint32_t *buffer)
{
    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 256; i++)
        {
            Vector3i color = Vector3f(255.999 * double(i) / (256 - 1), 255.999 * double(j) / (256 - 1), 0).cast<
                int>();
            buffer[j * 256 + i] = 0xff << 24 | color(2) << 16 | color(1) << 8 | color(0);

            if (*cancel)
            {
                std::cout << "Rendering cancelled" << std::endl;
                return;
            }
        }

        *progress_fraction = j / (256.0 - 1);
        // std::this_thread::sleep_for(1ms);
        // std::this_thread::yield();
    }

    *finished = true;
}


#endif //PATH_TRACER_MAIN_H
