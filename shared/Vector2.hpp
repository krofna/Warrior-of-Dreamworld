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
Vector2<NumberType> operator *(Vector2<NumberType> const& Left, NumberType Multiplier)
{
    return Vector2<NumberType>(Left.x * Multiplier, Left.y * Multiplier);
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

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

#endif // VECTOR2_HPP
