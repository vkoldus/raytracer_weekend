//
// Created by Vaclav Koldus on 14/07/2024.
//

#ifndef OBJECTS_H
#define OBJECTS_H

#include "ray.h"
#include "types.h"

class Material;

class Hittable {
public:
    [[nodiscard]] virtual bool hit(const Ray &r, const Interval &interval, HitInfo &hit) const = 0;

    virtual ~Hittable() = default;
};

class Object : public Hittable {
public:
    virtual Material &material() = 0;
};

class Sphere : public Object {
    std::shared_ptr<Material> _material;

public:
    Point3 center;
    fp_t radius;

    Sphere(const Point3 &center, fp_t radius, std::shared_ptr<Material> mat)
        : _material(mat),
          center(center),
          radius(radius)
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
            hit.material = _material;

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

    virtual Material &material() override
    {
        return *_material;
    }
};

#endif // OBJECTS_H
