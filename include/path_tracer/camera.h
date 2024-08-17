//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H


#include "angles.h"
#include "types.h"

struct Camera {
    double focal_length;
    double vfov_rad;

    Point3 look_from;
    Point3 look_at;
    Vector3 up;

    Vector3 w, u, v;

    fp_t viewport_height;
    fp_t viewport_width;
    Vector3 viewport_u;
    Vector3 viewport_v;
    Point3 viewport_top_left;
    Point3 top_left_pixel;


    Camera(Vector3 look_from, Vector3 look_at, Vector3 up, float vfov_rad, double viewport_aspect_ratio)
        : focal_length((look_at - look_from).norm()),
          vfov_rad(vfov_rad),
          look_from(look_from),
          look_at(look_at),
          up(up),
          w((look_from - look_at).normalized()),
          u(up.cross(w).normalized()),
          v(w.cross(u)),
          viewport_height(2.0 * std::tan(vfov_rad / 2) * focal_length),
          viewport_width(viewport_height * viewport_aspect_ratio),
          viewport_u(viewport_width * u),
          viewport_v(viewport_height * -v)
    {
        viewport_top_left = look_from - w * focal_length - viewport_u / 2 - viewport_v / 2;
    }

    Point3 center()
    {
        return look_from;
    }
};

#endif // CAMERA_H
