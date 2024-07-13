//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H


#include <chrono>
#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Vector3i;

struct Viewport {
    double height;
    double width;
    Vector3d u;
    Vector3d v;
    Vector3d pixel_delta_u;
    Vector3d pixel_delta_v;

    Viewport(double h, double aspect_ratio)
        : height(h), width(h * aspect_ratio)
    {
        u = {width, 0, 0};
        v = {0, -height, 0};
    }
};

struct Camera {
    double focal_length;
    Vector3d center;
    Viewport viewport;
    Vector3d viewport_top_left;
    Vector3d top_left_pixel;

    Camera(double f, Vector3d position, int viewport_h, double viewport_aspect_ratio)
        : focal_length(f), center(position), viewport(viewport_h, viewport_aspect_ratio)
    {
        viewport_top_left = center - Vector3d(0, 0, focal_length) - viewport.u / 2 - viewport.v / 2;
    }
};

#endif //CAMERA_H
