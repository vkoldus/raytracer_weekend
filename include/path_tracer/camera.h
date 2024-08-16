//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H


#include "angles.h"
#include "types.h"

struct Viewport {
    fp_t height;
    fp_t width;
    Vector3 u;
    Vector3 v;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;

    Viewport(fp_t h, fp_t aspect_ratio)
        : height(h),
          width(h * aspect_ratio)
    {
        u = {width, 0, 0};
        v = {0, -height, 0};
    }
};

struct Camera {
    double focal_length;
    Point3 center;
    Viewport viewport;
    Point3 viewport_top_left;
    Point3 top_left_pixel;
    double vfov_rad;

    Camera(double f, Vector3 position, float vfov_rad, double viewport_aspect_ratio)
        : focal_length(f),
          center(position),
          viewport(2.0 * std::tan(vfov_rad / 2) * focal_length, viewport_aspect_ratio),
          vfov_rad(vfov_rad)
    {
        viewport_top_left = center - Vector3(0, 0, focal_length) - viewport.u / 2 - viewport.v / 2;
    }
};

#endif // CAMERA_H
