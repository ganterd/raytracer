#pragma once

#include <iostream>

#include <rt/math/float4.hpp>

std::ostream& operator<<(std::ostream& os, const float4& v)
{
    return os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
}
