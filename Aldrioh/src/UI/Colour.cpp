#include <pch.h>
#include "Colour.h"
#include <Math/Math.h>

glm::vec4 Colour::Random()
{
    return glm::vec4{ Math::Random::normalized(), Math::Random::normalized(), Math::Random::normalized(), 1.0f };
}
