//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef PATH_TRACER_MAIN_H
#define PATH_TRACER_MAIN_H

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void render(float *progress_fraction, bool *cancel, bool *finished, uint32_t *buffer)
{
    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 256; i++)
        {
            auto r = double(i) / (256 - 1);
            auto g = double(j) / (256 - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            buffer[j * 256 + i] = 0xff << 24 | ib << 16 | ig << 8 | ir;

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
