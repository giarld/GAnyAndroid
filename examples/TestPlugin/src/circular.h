//
// Created by Gxin on 2023/11/4.
//

#ifndef TEST_PLUGIN_CIRCULAR_H
#define TEST_PLUGIN_CIRCULAR_H

#include <cstddef>
#include <cstdint>


struct Point
{
    float x;
    float y;

    Point()
            : x(0), y(0)
    {}

    Point(float x, float y)
            : x(x), y(y)
    {}
};

class Circular
{
public:
    explicit Circular(float x, float y, float r);

public:
    void setOrigin(const Point &origin);

    Point getOrigin() const;

    void setRadius(float r);

    float getRadius() const;

    double getArea() const;

    double getCircumference() const;

private:
    float mRadius;
    Point mOrigin;
};


#endif //TEST_PLUGIN_CIRCULAR_H
