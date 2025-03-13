#include "utils.h"

#include <cmath>

bool eq_zero(float value)
{
    return abs(value) <= ZERO;
}

bool gt_zero(float value)
{
    return value > ZERO;
}