#include "Math.h"

namespace math
{
    float GetAngle(sf::Vector2f Origin, sf::Vector2i Target)
    {
        // 'Move' the origin to (0, 0)
        Target.x -= Origin.x;
        Target.y -= Origin.y;

        // Calculateangle
        return std::atan2((float)Target.x, (float)Target.y;
    }
};