//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef METAL_H
#define METAL_H

#include "material.hpp"
#include "path_tracer/ray.hpp"
#include "random.hpp"

class Metal : public Material {
    bool _fuzz_enabled = true;

public:
    Metal(const Color &albedo, fp_t fuzz = 0.0)
        : albedo(albedo),
          fuzz(fuzz)
    {
    }

    bool scatter(const Ray &ray_in, const HitInfo &hit, Color &attenuation, Ray &scattered) const override
    {
        Vector3 fuzz_coeff = Vector3::Zero();

        if (_fuzz_enabled)
        {
            fuzz_coeff = fuzz * random_on_unit_sphere();
        }

        scattered.origin = hit.p;
        scattered.direction = reflect(ray_in.direction, hit.normal).normalized() + fuzz_coeff;

        // TODO: Why / how does this work exactly?

        attenuation = albedo;

        if (scattered.direction.dot(hit.normal) < 0)
            // Reflected into the material, so we stop the scattering.
            return false;
        else
            return true;
    }

    virtual void enable_fuzz(bool enable) override
    {
        _fuzz_enabled = enable;
    }

    Color albedo;
    fp_t fuzz;
};

#endif // METAL_H
