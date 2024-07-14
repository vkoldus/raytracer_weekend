//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef OBJECTS_H
#define OBJECTS_H
#include "types.h"
#include "ray.h"

class Hittable {
public:
    [[nodiscard]] virtual bool hit(const Ray &r, HitInfo &hit) const = 0;

    virtual ~Hittable() = default;
};

class Sphere : public Hittable {
public:
    Point3 center;
    fp_t radius;

    Sphere(const Point3 &center, fp_t radius): center(center), radius(radius)
    {
    }

    virtual bool hit(const Ray &r, HitInfo &hit) const override
    {
        // Analytical solution of intersection with a sphere
        // auto oc = center - r.origin;
        // auto a = r.direction.dot(r.direction);
        // auto b = -2 * r.direction.dot(oc);
        // auto c = oc.dot(oc) - radius * radius;
        // auto disc = b * b - 4 * a * c;

        // Simplified form of the code above through substituting b = -2h
        // Note that it doesn't seem to speed up. Just nicer form I guess?
        auto oc = center - r.origin;
        auto a = r.direction.dot(r.direction);
        auto h = r.direction.dot(oc);
        auto c = oc.dot(oc) - radius * radius;
        auto disc = h * h - a * c;

        if (disc >= 0)
        {
            // We have a solution. TODO: Pick the one closer to the camera.
            // return (-b - sqrt(disc)) / (2 * a);
            auto t = (h - sqrt(disc)) / a;
            hit.p = r.at(t);
            hit.t = t;
            hit.normal = (hit.p - center) / radius;

            return true;
        } else
        {
            return false;
            // return -1;
        }
    }
};

#endif //OBJECTS_H
