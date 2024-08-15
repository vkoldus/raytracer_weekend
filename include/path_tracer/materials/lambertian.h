//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <random>

#include "material.h"
#include "path_tracer/ray.h"
#include "random.h"

class Lambertian : public Material {
    Color albedo;

public:
    Lambertian(const Color &albedo)
        : albedo(albedo)
    {
    }

    bool scatter(const Ray &, const HitInfo &hit, Color &attenuation, Ray &scattered) const override
    {
        scattered.origin = hit.p;

        // TODO: Why / how does this work exactly?
        scattered.direction = hit.normal + random_on_unit_sphere();
        if (scattered.direction.cwiseAbs().minCoeff() < 1e-8)
            scattered.direction = hit.normal;

        attenuation = albedo;

        return true;
    }
};

#endif // LAMBERTIAN_H
