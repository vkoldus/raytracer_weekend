//
// Created by Vaclav Koldus on 20/07/2024.
//

#ifndef ANGLES_H
#define ANGLES_H

#include "consts.h"

constexpr inline double degrees_to_radians(double degrees)
{
    return degrees * Pi / 180.0;
}

#endif