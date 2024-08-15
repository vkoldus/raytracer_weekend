//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef RENDERING_SERVICE_H
#define RENDERING_SERVICE_H

#include <png.h>
#include "images.h"
#include "path_tracer/camera.h"
#include "path_tracer/path_tracer.h"
#include "path_tracer/world.h"
#include "state/app_state.h"

struct RenderingService {
    AppState &app_state;
    std::thread rendering_thread;
    ImageWithTexture image1;
    Camera camera;
    World world;
    bool prev_fuzz;

    RenderingService(AppState &app_state)
        : app_state(app_state),
          rendering_thread(),
          image1(create_image_with_texture(app_state.image_width, app_state.image_height)),
          camera(1.0, Vector3(0, 0, 0), 2.0, (double(app_state.image_width) / app_state.image_height)),
          world(make_world()),
          prev_fuzz(true)
    {
    }

    void render_sync()
    {
        render(&app_state, &world, &camera, image1.buffer);
    }

    void render_async()
    {
        cancel_render();
        rendering_thread = std::thread(render, &app_state, &world, &camera, image1.buffer);
    }

    void cancel_render()
    {
        if (rendering_thread.joinable())
        {
            app_state.cancel_rendering = true;
            rendering_thread.join();
        }
        app_state.cancel_rendering = false;
    }

    void loop_hook()
    {
        if (app_state.rendering_finished)
        {
            upload_texture(image1);
            // Move one of the objects
            if (app_state.move_object)
            {
                std::dynamic_pointer_cast<Sphere>(world[1])->center[0] = sinf((float) ImGui::GetTime());
            }

            // Move the camera
            if (app_state.move_camera)
            {
                camera.center[1] = sinf((float) ImGui::GetTime() / 3) / 3;
            }

            // Modify materials based on config
            if (prev_fuzz != app_state.metal_fuzz)
            {
                std::cout << "Changing fuzz to " << app_state.metal_fuzz << std::endl;
                for (auto object: world)
                {
                    object->material().enable_fuzz(app_state.metal_fuzz);
                }

                prev_fuzz = app_state.metal_fuzz;
            }
        }
    }

    void shutdown()
    {
        cancel_render();
        delete_image_with_texture(image1);
    }

    void save_next_image_to(const std::string &path)
    {
        auto fp = fopen(path.c_str(), "wb");
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info_ptr = png_create_info_struct(png_ptr);

        png_set_IHDR(png_ptr,
                     info_ptr,
                     (png_uint_32) image1.width,
                     (png_uint_32) image1.height,
                     8,
                     PNG_COLOR_TYPE_RGB,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);

        png_byte **row_pointers = (png_byte **) png_malloc(png_ptr, (size_t) image1.height * sizeof(png_byte *));
        for (int y = 0; y < image1.height; y++)
        {
            png_byte *row = (png_byte *) png_malloc(png_ptr, sizeof(uint8_t) * size_t(image1.width) * 24);
            row_pointers[y] = row;
            for (int x = 0; x < image1.width; x++)
            {
                *row++ = image1.buffer[image1.width * y + x] & 0xff;
                *row++ = (image1.buffer[image1.width * y + x] & 0xff00) >> 8;
                *row++ = (image1.buffer[image1.width * y + x] & 0xff0000) >> 16;
            }
        }

        png_init_io(png_ptr, fp);
        png_set_rows(png_ptr, info_ptr, row_pointers);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
        fclose(fp);
    }

    auto gl_texture() const
    {
        return image1.gl_texture;
    }
};

#endif // RENDERING_SERVICE_H
