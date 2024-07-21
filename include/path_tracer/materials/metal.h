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
    fp_t fuzz;

public:
    Metal(const Color &albedo, fp_t fuzz = 0.0)
        : albedo(albedo), fuzz(fuzz)
    {
    }

    bool scatter(
        const Ray &ray_in, const HitInfo &hit, Color &attenuation, Ray &scattered
    ) const override
    {
        scattered.origin = hit.p;
        scattered.direction = reflect(ray_in.direction, hit.normal).normalized() + fuzz * random_on_unit_sphere();

        // TODO: Why / how does this work exactly?

        attenuation = albedo;

        if (scattered.direction.dot(hit.normal) < 0)
            // Reflected into the material, so we stop the scattering.
            return false;
        else
            return true;
    }
};

#endif //METAL_H
