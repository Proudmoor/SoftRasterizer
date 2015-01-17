//
//  Light.h
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#ifndef __softRender__Light__
#define __softRender__Light__

#include <stdio.h>

#include <string>

#include "Color.h"
#include "glm/glm.hpp"

class Light
{
public:
    enum LightType
    {
        LT_POINT = 0,
        LT_SPOT = 1,
        LT_DIRECTIONAL = 2
    };
    
    Light() :
    Type(LT_POINT),
    Ka(0.0f, 0.0f, 0.0f, 1.0f),
    Kd(1.0f, 1.0f, 1.0f, 1.0f),
    Ks(1.0f, 1.0f, 1.0f, 1.0f),
    Pos(0.0f, 0.0f, 1.0f),
    Dir(0.0f, 0.0f, -1.0f),
    C(1.0),
    L(0.0),
    Q(0.0)
    {
    }
    ~Light() {}
    
    float GetAttenuation(float distance) // only valid for point light
    {
        return 1 / (C + L * distance + Q * distance * distance);
    }
    
    LightType Type;
    
    Color Ka; // Ambient
    Color Kd; // Diffuse
    Color Ks; // Specular
    
    glm::vec3 Pos;	// Position in world space
    glm::vec3 Dir;	// Direction in world space
    
    float Range;	// Cutoff range
    float Falloff;	// Falloff
    float C;		// attenuation constant
    float L;		// attenuation linear
    float Q;		// attenuation quadric
    float Theta;	// Inner angle of spotlight cone
    float Phi;      // Outer angle of spotlight cone
};


class Material
{
public:
    Material() :
    Ka(0.2f, 0.2f, 0.2f, 1.0f),
    Kd(0.8f, 0.8f, 0.8f, 1.0f),
    Ks(0.0f, 0.0f, 0.0f, 1.0f),
    Ksh(0.0f),
    Ke(0.0f, 0.0f, 0.0f, 1.0f)
    {}
    
    ~Material() {}
    
    Color Ka;	// ambient
    Color Kd;	// diffuse
    Color Ks;	// specular
    float Ksh;	// shininess index
    Color Ke;	// emission
};

#endif /* defined(__softRender__Light__) */
