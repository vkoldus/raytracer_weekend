//
// Created by Vaclav Koldus on 20/07/2024.
//

#ifndef GAMMA_H
#define GAMMA_H

#include <cmath>
#include "types.hpp"

inline Color linear_to_gamma(const Color &color)
{
    return color.cwiseSqrt();
}

#endif // GAMMA_H
