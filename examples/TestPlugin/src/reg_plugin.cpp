//
// Created by Gxin on 2023/11/4.
//

#include "reg_plugin.h"

#include <gx/gany.h>

#include "circular.h"

#include <sstream>


using namespace gx;


REGISTER_GANY_MODULE(TestPlugin)
{
    Class<Point>("Cpp", "Point", "Struct Point.")
            .construct<>()
            .construct<float, float>()
            .readWrite("x", &Point::x)
            .readWrite("y", &Point::y)
            .func(MetaFunction::ToString, [](const Point &self) {
                std::stringstream ss;
                ss << "(" << self.x << ", " << self.y << ")";
                return ss.str();
            });

    Class<Circular>("Cpp", "Circular", "Class Circular.")
            .construct<float, float, float>()
            .func("setOrigin", &Circular::setOrigin)
            .func("getOrigin", &Circular::getOrigin)
            .func("setRadius", &Circular::setRadius)
            .func("getRadius", &Circular::getRadius)
            .func("getArea", &Circular::getArea)
            .func("getCircumference", &Circular::getCircumference)
            .property("origin", &Circular::getOrigin, &Circular::setOrigin)
            .property("radius", &Circular::getRadius, &Circular::setRadius);
}
