//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef WORLD_H
#define WORLD_H

#include "consts.hpp"
#include "path_tracer/materials/dielectric.hpp"
#include "path_tracer/materials/lambertian.hpp"
#include "path_tracer/materials/metal.hpp"
#include "path_tracer/objects.hpp"
#include "types.hpp"


using World = std::vector<std::shared_ptr<Object>>;

// Development scene
World make_dev_world()
{
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    // Air bubble in water
    // auto material_left = std::make_shared<Dielectric>(1 / 1.33);
    // Glass
    auto material_left = std::make_shared<Dielectric>(1.5);
    // Air in glass
    auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.1);

    return {
            std::make_shared<Sphere>(Point3(0, -100.5, -1.0), 100, material_ground),
            std::make_shared<Sphere>(Point3(0, 0, -1.0), 0.5, material_center),
            std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left),
            std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble),
            std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right),

            // FoV testing spheres
            // std::make_shared<Sphere>(Point3(-R, 0, -1), R, std::make_shared<Lambertian>(Color(0, 0, 1))),
            // std::make_shared<Sphere>(Point3(R, 0, -1), R, std::make_shared<Lambertian>(Color(1, 0, 0))),
    };
}

World make_demo_world()
{
    // Demo scene
    World world;

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.emplace_back(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_number();
            Point3 center(a + 0.9 * random_number(), 0.2, b + 0.9 * random_number());

            if ((center - Point3(4, 0.2, 0)).norm() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    Color albedo = (((Color::Random() + Color(1.0, 1.0, 1.0)) / 2.0)
                                            .cwiseProduct((Color::Random() + Color(1.0, 1.0, 1.0)) / 2.0));
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.emplace_back(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95)
                {
                    // metal
                    Color albedo = (Color::Random() + Color(1.0, 1.0, 1.0)) / 4 + Color(0.5, 0.5, 0.5);
                    auto fuzz = random_number() / 2;
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.emplace_back(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.emplace_back(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.emplace_back(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.emplace_back(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.emplace_back(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

#endif // WORLD_H
