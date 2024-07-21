//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "random.h"
#include "path_tracer/ray.h"
#include "material.h"

class Lambertian : public Material {
    bool scatter(
        const Ray &ray_in, const HitInfo &hit, Color &attenuation, Ray &scattered
    ) const override
    {
        scattered.origin = hit.p;
        // TODO: Why / how does this work exactly?
        scattered.direction = hit.normal + random_on_unit_sphere();

        attenuation[0] = 0.5;
        attenuation[1] = 0.5;
        attenuation[2] = 0.5;

        return true;
    }
};

#endif //LAMBERTIAN_H
