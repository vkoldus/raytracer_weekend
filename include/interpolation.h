//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

template<typename T>
T lerp(double a, T start, T end)
{
    return (1 - a) * start + a * end;
}

#endif //INTERPOLATION_H
