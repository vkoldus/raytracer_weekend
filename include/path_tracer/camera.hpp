//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H


#include "angles.hpp"
#include "types.hpp"

struct Camera {
    double focus_distance;
    double vfov_rad;
    double defocus_angle_rad;

    Point3 look_from;
    Point3 look_at;
    Vector3 up;

    Vector3 w, u, v;
    Vector3 defocus_disk_u, defocus_disk_v;

    fp_t viewport_height;
    fp_t viewport_width;
    Vector3 viewport_u;
    Vector3 viewport_v;
    Point3 viewport_top_left;
    Point3 top_left_pixel;

    Camera(Vector3 look_from, Vector3 look_at, Vector3 up, double focus_distance, double vfov_rad,
           double defocus_angle_rad, double viewport_aspect_ratio)
        : focus_distance(focus_distance),
          vfov_rad(vfov_rad),
          defocus_angle_rad(defocus_angle_rad),
          look_from(look_from),
          look_at(look_at),
          up(up),
          viewport_height(2.0 * std::tan(vfov_rad / 2) * focus_distance),
          viewport_width(viewport_height * viewport_aspect_ratio)
    {
        update_basis();
        update_viewport();
        update_top_left();
    }

    void update_basis()
    {
        w = (look_from - look_at).normalized();
        u = up.cross(w).normalized();
        v = w.cross(u);

        auto defocus_radius = focus_distance * std::tan(defocus_angle_rad / 2);
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    void update_viewport()
    {
        viewport_u = viewport_width * u;
        viewport_v = viewport_height * -v;
    }

    void update_top_left()
    {
        viewport_top_left = look_from - (w * focus_distance) - viewport_u / 2 - viewport_v / 2;
    }


    Point3 center()
    {
        return look_from;
    }

    void forward(fp_t step)
    {
        look_from -= step * w;
        look_at -= step * w;
        update_top_left();
    }

    void strafe(fp_t step)
    {
        look_from += step * u;
        look_at += step * u;
        update_top_left();
    }

    void fly_vertical(fp_t step)
    {
        look_from += step * v;
        look_at += step * v;
        update_top_left();
    }


    void turn(fp_t angle_deg)
    {
        // TODO: These are expensive and complicated. Could be simplified by just remembering pitch/yaw angles and
        // calculating look_at from them.
        auto angle_rad = degrees_to_radians(angle_deg);
        auto vxw = v.cross(w);
        auto rotated_w = (v * (v.dot(w))) + (cos(angle_rad) * (vxw)).cross(v) + sin(angle_rad) * (vxw);
        look_at = look_from - rotated_w;
        update_basis();
        update_viewport();
        update_top_left();
    }


    void look_vertical(fp_t angle_deg)
    {
        auto angle_rad = degrees_to_radians(angle_deg);
        auto uxw = u.cross(w);
        auto rotated_w = (u * (u.dot(w))) + (cos(angle_rad) * (uxw)).cross(u) + sin(angle_rad) * (uxw);
        look_at = look_from - rotated_w;
        update_basis();
        update_viewport();
        update_top_left();
    }
};

#endif // CAMERA_H
