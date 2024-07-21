//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include "path_tracer/materials/lambertian.h"
#include "path_tracer/materials/metal.h"


World make_world()
{
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2));

    return {
        std::make_shared<Sphere>(Point3(0, -100.5, -1.0), 100, material_ground),
        std::make_shared<Sphere>(Point3(0, 0, -1.2), 0.5, material_center),
        std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left),
        std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right),
    };
}

#endif //WORLD_H
