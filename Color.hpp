//
// Created by Flo on 07/04/2023.
//

#pragma once

#include "Matrix.hpp"

template<typename T>
struct Color
{
    Color(const T& r_ = 0, const T& g_ = 0, const T& b_ = 0, const T& a_ = 0): r(r_), g(g_), b(b_), a(a_) {}
    Color(const Color& color): r(color.r), g(color.g), b(color.b), a(color.a) {}

    T r = 0;
    T g = 0;
    T b = 0;
    T a = 0;
};

template<typename T>
struct vertex_struct_color
{
    Point2d<T> p;
    Color<T> c;
};
