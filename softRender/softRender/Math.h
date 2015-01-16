//
//  Math.h
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#ifndef __softRender__Math__
#define __softRender__Math__

#include <cstdio>
#include <cmath>
#include <cassert>

#define FLOAT_EPSILON 1e-6
#define DTOR		0.017453 // convert degrees to radians, PI / 180
#define RTOD		57.29578 // convert radians to degrees, 180 / PI

//some common math function need in other files

#define ROUND(x) (int)((x)+0.5)

inline bool FloatEqual(float fp1, float fp2)
{
    return std::fabs(fp1 - fp2) < FLOAT_EPSILON;
}

inline float Clampf(float value, float minimum, float maximum)
{
    float ret = std::fmin(std::fmax(minimum, value), maximum);
    return ret;
}

inline float LERPf(float v1, float v2, float t)
{
    return v1 * (1 - t) + v2 * t;
}
#endif /* defined(__softRender__Math__) */
