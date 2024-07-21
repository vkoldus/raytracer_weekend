//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef OBJECTS_H
#define OBJECTS_H

#include "types.h"
#include "ray.h"

class Material;

class Hittable {
public:
    [[nodiscard]] virtual bool hit(const Ray &r, const Interval &interval, HitInfo &hit) const = 0;

    virtual ~Hittable() = default;
};

class Sphere : public Hittable {
public:
    Point3 center;
    fp_t radius;
    std::shared_ptr<const Material> material;

    Sphere(const Point3 &center, fp_t radius, std::shared_ptr<const Material> mat): center(center), radius(radius),
        material(mat)
    {
    }

    virtual bool hit(const Ray &r, const Interval &ray_interval, HitInfo &hit) const override
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
            // return (-b - sqrt(disc)) / (2 * a);
            auto t = (h - sqrt(disc)) / a;
            if (!ray_interval.surrounds(t))
            {
                t = (h + sqrt(disc)) / a;
            }

            if (!ray_interval.surrounds(t))
            {
                return false;
            }

            hit.p = r.at(t);
            hit.t = t;
            hit.material = material;

            // It's our responsibility to make sure the normal always points against the ray
            Vector3 outward_normal = (hit.p - center) / radius;
            if (outward_normal.dot(r.direction) >= 0)
            {
                hit.front_face = false;
                hit.normal = -outward_normal;
            } else
            {
                hit.front_face = true;
                hit.normal = outward_normal;
            }

            return true;
        } else
        {
            return false;
        }
    }
};

#endif //OBJECTS_H
