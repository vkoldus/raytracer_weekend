//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef RENDERING_SERVICE_H
#define RENDERING_SERVICE_H

#include "images.h"
#include "state/app_state.h"

struct RenderingService {
    ImageWithTexture image1;
    std::thread rendering_thread;
    AppState &app_state;

    RenderingService(AppState &app_state): app_state(app_state),
                                           rendering_thread(),
                                           image1(create_image_with_texture(
                                               app_state.image_width, app_state.image_height))


    {
    }

    void render_sync()
    {
        render(&app_state.progress, &app_state.cancel_rendering, &app_state.rendering_finished, image1.buffer,
               app_state.image_width, app_state.image_height);
    }

    void render_async()
    {
        cancel_render();

        rendering_thread = std::thread(render, &app_state.progress, &app_state.cancel_rendering,
                                       &app_state.rendering_finished,
                                       image1.buffer, app_state.image_width, app_state.image_height);
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
