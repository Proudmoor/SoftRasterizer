//
//  Color.cpp
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include "Color.h"

#define SATURATE(A) if (A < 0) A=0; else if (A > 1) A =1;

Uint32 Color::Color2Uint()  const{
   
    Uint32 R = (Uint32) (r * 255.0f);
    Uint32 G = (Uint32) (g * 255.0f);
    Uint32 B = (Uint32) (b * 255.0f);
    Uint32 A = (Uint32) (a * 255.0f);
    
    return (A << 24) | (R << 16) | (G << 8) | B;
}

Color& Color::operator+=(const Color& c)
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

Color& Color::operator-=(const Color& c)
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

Color& Color::operator*=(const Color& c)
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

Color& Color::operator*=(float f)
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

Color& Color::operator/=(float f)
{
    assert(f != 0);
    f = 1/f;
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

Color Color::operator+() const
{
    return *this;
}

Color Color::operator-() const
{
    return Color(r, g, b, a);
}

void Color::Clamp()
{
    r = Clampf(r, 0.0f, 1.0f);
    g = Clampf(g, 0.0f, 1.0f);
    b = Clampf(b, 0.0f, 1.0f);
}

bool operator==(const Color& lhs, const Color& rhs)
{
    if ( FloatEqual(lhs.r, rhs.r) &&
        FloatEqual(lhs.g, rhs.g) &&
        FloatEqual(lhs.b, rhs.b) &&
        FloatEqual(lhs.a, rhs.a) )
    {
        return true;
    }
    return false;
}

bool operator!=(const Color& lhs, const Color& rhs)
{
    return !(lhs == rhs);
}

//-------------------------------------------------------------------
Color operator + (const Color& lhs, const Color& rhs)
{
    Color ret(lhs);
    ret += rhs;
    return ret;
}

Color operator-(const Color& lhs, const Color& rhs)
{
    Color ret(lhs);
    ret -= rhs;
    return ret;
}

Color operator * (float f, const Color& c)
{
    Color ret(c);
    ret *= f;
    return ret;
}

Color operator * (const Color& c, float f)
{
    Color ret(c);
    ret *= f;
    return ret;
}

Color operator * (const Color& lhs, const Color& rhs)
{
    Color ret(lhs);
    ret *= rhs;
    return ret;
}

Color operator/(const Color& c, float f)
{
    Color ret(c);
    ret /= f;
    return ret;
}

Color LERP(const Color& c1, const Color& c2, float t)
{
    return Color(LERPf(c1.r, c2.r, t),
                 LERPf(c1.g, c2.g, t),
                 LERPf(c1.b, c2.b, t),
                 LERPf(c1.a, c2.a, t));
}