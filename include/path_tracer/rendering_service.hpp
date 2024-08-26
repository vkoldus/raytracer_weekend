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
    std::array<std::thread, 6> rendering_threads;
    std::vector<std::function<void(void)>> rendering_jobs;
    std::mutex rendering_jobs_mutex;
    bool should_shutdown;
    std::chrono::time_point<std::chrono::high_resolution_clock> render_start;

    RenderingService(AppState &app_state)
        : app_state(app_state),
          rendering_thread(),
          image1(create_image_with_texture(app_state.image_width, app_state.image_height)),
          camera(make_camera(app_state)),
          world(make_dev_world()),
          //   world(make_demo_world()),
          prev_fuzz(true),
          rendering_threads(),
          rendering_jobs(),
          should_shutdown(false),
          render_start()
    {
        for (size_t i = 0; i < rendering_threads.size(); i++) {
            rendering_threads[i] = std::thread(std::bind(&RenderingService::rendering_thread_loop, this));
        }
    }

    void rendering_thread_loop() {
        while (!should_shutdown) {
            std::function<void(void)> job = nullptr;
            {
                std::unique_lock<std::mutex> l(rendering_jobs_mutex);
                
                if (rendering_jobs.size() > 0) {
                    job = rendering_jobs.back();
                    rendering_jobs.pop_back();
                }
            }
            if (job != nullptr) {
                job();
            } else {
                // TODO: An event would be better...
                std::this_thread::yield();
                // std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    }

    static Camera make_camera(AppState &app_state)
    {
        return Camera(app_state.initial_lookfrom,
                      app_state.initial_lookat,
                      app_state.initial_vup,
                      app_state.focus_distance.value,
                      app_state.vfov_rad.value,
                      app_state.defocus_angle_rad.value,
                      (double(app_state.image_width) / app_state.image_height));
    }

    void render_sync()
    {
        render(&app_state, &world, &camera, image1.buffer, 0, (size_t) app_state.image_height);
    }

    void render_async()
    {
        cancel_render();

        app_state.progress = 0;
        render_start = std::chrono::high_resolution_clock::now();

        auto thread_count = int(rendering_threads.size());
        std::unique_lock<std::mutex> l(rendering_jobs_mutex);
        for (auto i = 0; i < thread_count; i++)
        {
            auto batch_size = int(app_state.image_height / int(thread_count));
            auto batch_start = i * batch_size;
            auto batch_end = i == thread_count-1 ? app_state.image_height + 1 : (i + 1) * batch_size;

            rendering_jobs.emplace_back(
                std::bind(&render, &app_state, &world, &camera, image1.buffer, batch_start, batch_end)
            );
        }
    }

    void cancel_render()
    {
        app_state.cancel_rendering = false;
    }

    void loop_hook(bool is_w_down, bool is_s_down, bool is_a_down, bool is_d_down, bool is_q_down, bool is_e_down,
                   bool is_r_down, bool is_f_down, bool is_t_down, bool is_g_down)
    {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        using std::chrono::high_resolution_clock;

        upload_texture(image1);

        if (app_state.progress == app_state.image_height)
        {
            if (render_start != high_resolution_clock::time_point()) {
                std::cout << "Rendering took " << duration_cast<milliseconds>(high_resolution_clock::now() - render_start) << "\n";
                render_start = high_resolution_clock::time_point();
            }

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
        should_shutdown = true;
        for (auto &t : rendering_threads) {
            t.join();
        }
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
