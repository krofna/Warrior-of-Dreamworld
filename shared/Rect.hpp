#ifndef RECT_HPP_INCLUDED
#define RECT_HPP_INCLUDED

#include "Vector2.hpp"

// TODO: Need more cleaning and some work...

template<typename NumberType>
struct Rect
{
    Rect() {}
    Rect(NumberType rectLeft, NumberType rectTop, NumberType rectWidth, NumberType rectHeight);
    Rect(Vector2<NumberType> pos, Vector2<NumberType> size);

    bool intersects(Rect<NumberType> const& Target) { Rect<NumberType> intersection; return intersects(Target, intersection); }
    bool intersects(Rect<NumberType> const& Target, Rect<NumberType>& Intersection);

    bool contains(NumberType x, NumberType y) { return (x >= left) && (x < left + width) && y >= top && (y < top + height); }
    bool contains(Vector2<NumberType> const& Pos) { return contains(Pos.x, Pos.y); }

    NumberType top;
    NumberType left;
    NumberType height;
    NumberType width;
};

template<typename NumberType>
Rect<NumberType>::Rect(NumberType rectLeft, NumberType rectTop, NumberType rectWidth, NumberType rectHeight) : top(rectTop), left(rectLeft), height(rectHeight), width(rectWidth)
{

}
template<typename NumberType>
Rect<NumberType>::Rect(Vector2<NumberType> pos, Vector2<NumberType> size) : top(pos.y), left(pos.x), height(size.y), width(size.x)
{

}

template<typename NumberType>
bool Rect<NumberType>::intersects(Rect<NumberType> const& Target, Rect<NumberType>& Intersection)
{
    // Compute the intersection boundaries
    NumberType interLeft = std::max(left, Target.left);
    NumberType interTop = std::max(top, Target.top);
    NumberType interRight = std::min(left + width, Target.left + Target.width);
    NumberType interBottom = std::min(top + height, Target.top + Target.height);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom))
    {
        Intersection = Rect<NumberType>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    }
    else
    {
        Intersection = Rect<NumberType>(0, 0, 0, 0);
        return false;
    }
}

typedef Rect<float> FloatRect;

#endif // RECT_HPP_INCLUDED
