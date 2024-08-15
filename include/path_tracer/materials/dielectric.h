//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "path_tracer/materials/material.h"
#include "path_tracer/ray.h"
#include "random.h"
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

        auto unit_direction = ray_in.direction.normalized();
        double cos_theta = std::fmin((-unit_direction.dot(hit.normal)), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ref_idx * sin_theta > 1.0;

        Vector3 direction;
        ;
        if (cannot_refract || reflectance(cos_theta, ref_idx) > random_number())
        {
            direction = reflect(unit_direction, hit.normal);

        } else
        {
            direction = refract(unit_direction, hit.normal, ref_idx);
        }

        scattered = Ray{hit.p, direction};

        return true;
    }

    static double reflectance(double cosine, double refraction_index)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;

        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif // DIELECTRIC_H
