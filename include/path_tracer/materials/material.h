//
// Created by Vaclav Koldus on 20/07/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "types.h"

class Ray;
class HitInfo;

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &r_in, const HitInfo &rec, Color &attenuation, Ray &scattered) const
    {
        return false;
    }

    virtual void enable_fuzz(bool enable)
    {
    }
};

#endif // MATERIAL_H
