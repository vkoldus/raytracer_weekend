//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef RENDERING_SERVICE_H
#define RENDERING_SERVICE_H

#include "images.h"
#include "state/app_state.h"
#include "path_tracer/path_tracer.h"
#include "path_tracer/camera.h"

struct RenderingService {
    ImageWithTexture image1;
    std::thread rendering_thread;
    AppState &app_state;
    World world;
    Camera camera;

    RenderingService(AppState &app_state): app_state(app_state),
                                           rendering_thread(),
                                           image1(create_image_with_texture(
                                               app_state.image_width, app_state.image_height)),
                                           camera(1.0, Vector3(0, 0, 0), 2.0,
                                                  (double(app_state.image_width) / app_state.image_height))


    {
        world = {
            std::make_shared<Sphere>(Point3(0, 0, -1), 0.5),
            std::make_shared<Sphere>(Point3(0, -100.5, -1), 100),
        };
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
                std::dynamic_pointer_cast<Sphere>(world[0])->center[0] = sinf((float) ImGui::GetTime());
            }

            // Move the camera
            if (app_state.move_camera)
            {
                camera.center[1] = sinf((float) ImGui::GetTime() / 3) / 3;
            }
        }
    }

    void shutdown()
    {
        cancel_render();
        delete_image_with_texture(image1);
    }

    auto gl_texture() const
    {
        return image1.gl_texture;
    }
};


#endif //RENDERING_SERVICE_H
