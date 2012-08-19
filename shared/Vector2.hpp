#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include "Defines.hpp"
// TODO: Minor cleanings necessary.

template<typename NumberType>
class Vector2
{
    public:
        Vector2();
        Vector2(NumberType x, NumberType y);

        template<typename OtherType>
        int GetManhattanDistance(Vector2<OtherType> const& Target);
        template<typename OtherType>
        float GetDistance(Vector2<OtherType> const& Target);

        template<typename OtherType>
        float GetAngle(Vector2<OtherType> const& Target);

        template<typename OtherType>
        Vector2<NumberType>& operator *(OtherType const& Multiplier) { x *= static_cast<NumberType>(Multiplier); y *= static_cast<NumberType>(Multiplier); return *this;}

    NumberType x;
    NumberType y;
};

template<typename NumberType>
bool operator ==(Vector2<NumberType> const& Left, Vector2<NumberType> const& Right)
{
    return Left.x == Right.x && Left.y == Right.y;
}

template<typename NumberType>
bool operator !=(Vector2<NumberType> const& Left, Vector2<NumberType> const& Right)
{
    return Left.x != Right.x && Left.y != Right.y;
}

template<typename NumberType>
Vector2<NumberType>::Vector2()
{

}

template<typename NumberType>
Vector2<NumberType>::Vector2(NumberType px, NumberType py) : x(px), y(py)
{

}

template<typename NumberType>
template<typename OtherType>
int Vector2<NumberType>::GetManhattanDistance(Vector2<OtherType> const& Target)
{
    return std::abs(x - Target.x) + std::abs(y - Target.y);
}
template<typename NumberType>
template<typename OtherType>
float Vector2<NumberType>::GetDistance(Vector2<OtherType> const& Target)
{
    return std::sqrt((x - Target.x) * (x - Target.x) + (y - Target.y) * (y - Target.y));
}

template<typename NumberType>
template<typename OtherType>
float Vector2<NumberType>::GetAngle(Vector2<OtherType> const& Target)
{
    // Click position should represent center of sprite, NOT the upper-left corner
    Target.x -= TILE_SIZE / 2;
    Target.y -= TILE_SIZE / 2;

    // 'Move' the origin to (0, 0)
    Target.x -= x;
    Target.y -= y;

    // Calculate angle
    return std::atan2(Target.x, Target.y);
}



typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

#endif // VECTOR2_HPP
