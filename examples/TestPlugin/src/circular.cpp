//
// Created by Gxin on 2023/11/4.
//

#include "circular.h"

#define PI  3.14159265358979323846264338327950288

Circular::Circular(float x, float y, float r)
    : mRadius(r), mOrigin({x, y})
{
}

void Circular::setOrigin(const Point &origin)
{
    mOrigin = origin;
}

Point Circular::getOrigin() const
{
    return mOrigin;
}

void Circular::setRadius(float r)
{
    mRadius = r;
}

float Circular::getRadius() const
{
    return mRadius;
}

double Circular::getArea() const
{
    return mRadius * mRadius * PI;
}

double Circular::getCircumference() const
{
    return 2.0 * PI * mRadius;
}
