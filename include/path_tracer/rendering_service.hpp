//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef RENDERING_SERVICE_H
#define RENDERING_SERVICE_H

#include <future>

#include <png.h>
#include "images.hpp"
#include "path_tracer/camera.hpp"
#include "path_tracer/path_tracer.hpp"
#include "path_tracer/world.hpp"
#include "state/app_state.hpp"

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
          camera(make_camera(app_state)),
          world(make_world()),
          prev_fuzz(true)
    {
    }

    static Camera make_camera(AppState &app_state)
    {
        return Camera(Vector3(-2, 2, 1),
                      Vector3(0, 0, -1),
                      Vector3(0, 1, 0),
                      app_state.focus_distance.value,
                      app_state.vfov_rad.value,
                      app_state.defocus_angle_rad.value,
                      (double(app_state.image_width) / app_state.image_height));
    }

    void render_sync()
    {
        render(&app_state, &world, &camera, image1.buffer, 0, (size_t) app_state.image_height);
    }

    void _render_async()
    {
        // This is not faster. Probably need a thread pool?
        // auto threads = 4;
        // std::vector<std::thread> tasks;

        // for (auto i = 0; i < threads; i++)
        // {
        //     auto batch_size = int(app_state.image_height / threads);
        //     auto batch_start = i * batch_size;
        //     auto batch_end = (i + 1) * batch_size;

        //     std::cout << batch_start << " - " << batch_end << std::endl;

        //     tasks.push_back(std::thread(render, &app_state, &world, &camera, image1.buffer, batch_start, batch_end));
        // }


        // for (auto &t: tasks)
        // {
        //     t.join();
        // }
    }

    void render_async()
    {
        cancel_render();

        rendering_thread = std::thread(render, &app_state, &world, &camera, image1.buffer, 0, app_state.image_height);
        // _render_async();
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

    void loop_hook(bool is_w_down, bool is_s_down, bool is_a_down, bool is_d_down, bool is_q_down, bool is_e_down,
                   bool is_r_down, bool is_f_down, bool is_t_down, bool is_g_down)
    {
        if (app_state.rendering_finished)
        {
            upload_texture(image1);
            // Move one of the objects
            if (app_state.move_object)
            {
                std::dynamic_pointer_cast<Sphere>(world[1])->center[0] = sinf((float) ImGui::GetTime());
            }

            // Change the camera parameters
            if (app_state.vfov_rad.has_changed || app_state.focus_distance.has_changed ||
                app_state.defocus_angle_rad.has_changed)
            {
                camera = Camera(camera.look_from,
                                camera.look_at,
                                camera.up,
                                app_state.focus_distance.value,
                                app_state.vfov_rad.value,
                                app_state.defocus_angle_rad.value,
                                app_state.aspect_ratio);
                app_state.vfov_rad.reset();
                app_state.focus_distance.reset();
                app_state.defocus_angle_rad.reset();
            }

            // Move the camera
            if (app_state.move_camera)
            {
                camera.look_from[1] = sinf((float) ImGui::GetTime() / 3) / 3;
            } else
            {
                const fp_t speed = 0.1;
                const fp_t turn_speed_deg = 5;

                if (is_w_down)
                {
                    camera.forward(speed);
                }

                if (is_s_down)
                {
                    camera.forward(-speed);
                }

                if (is_a_down)
                {
                    camera.turn(turn_speed_deg);
                }
                if (is_d_down)
                {
                    camera.turn(-turn_speed_deg);
                }
                if (is_q_down)
                {
                    camera.strafe(-speed);
                }
                if (is_e_down)
                {
                    camera.strafe(speed);
                }
                if (is_r_down)
                {
                    camera.fly_vertical(speed);
                }
                if (is_f_down)
                {
                    camera.fly_vertical(-speed);
                }
                if (is_t_down)
                {
                    camera.look_vertical(turn_speed_deg);
                }
                if (is_g_down)
                {
                    camera.look_vertical(-turn_speed_deg);
                }
            }

            // Modify materials based on config
            if (prev_fuzz != app_state.metal_fuzz)
            {
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
