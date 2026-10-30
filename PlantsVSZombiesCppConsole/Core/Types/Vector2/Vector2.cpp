#include "Vector2.h"
#include <cmath>

double Vector2::Length() const
{
    return std::sqrt(x * x + y * y);
}

double Vector2::DistanceTo(const Vector2& otherVec) const
{
    return (*this - otherVec).Length();
}
