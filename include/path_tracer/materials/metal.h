//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef METAL_H
#define METAL_H

#include "random.h"
#include "path_tracer/ray.h"
#include "material.h"

Vector3 reflect(const Vector3 &v, const Vector3 &n)
{
    return v - 2 * v.dot(n) * n;
}

class Metal : public Material {
    Color albedo;

public:
    Metal(const Color &albedo)
        : albedo(albedo)
    {
    }

    bool scatter(
        const Ray &ray_in, const HitInfo &hit, Color &attenuation, Ray &scattered
    ) const override
    {
        scattered.origin = hit.p;
        scattered.direction = reflect(ray_in.direction, hit.normal);

        // TODO: Why / how does this work exactly?

        attenuation = albedo;

        return true;
    }
};

#endif //METAL_H
