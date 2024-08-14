//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "path_tracer/materials/material.h"
#include "path_tracer/ray.h"
#include "types.h"


class Dielectric : public Material {
    fp_t refraction_index;

public:
    Dielectric(fp_t refraction_index = 0.0)
        : refraction_index(refraction_index)
    {
    }

    bool scatter(const Ray &ray_in, const HitInfo &hit, Color &attenuation, Ray &scattered) const override
    {
        attenuation[0] = 1.0;
        attenuation[1] = 1.0;
        attenuation[2] = 1.0;

        fp_t ref_idx = hit.front_face ? (1.0 / refraction_index) : refraction_index;

        scattered = Ray{hit.p, refract(ray_in.direction.normalized(), hit.normal, ref_idx)};

        return true;
    }
};

#endif // DIELECTRIC_H
