//
//  Pixels.h
//  softRender
//
//  Created by zpf on 15-1-15.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#ifndef __softRender__Pixels__
#define __softRender__Pixels__

#include <stdio.h>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"

typedef glm::vec3 vec3;

extern SDL_Surface * surface;

void SetRasterSurface(SDL_Surface *);

void SetRasterColor(float, float, float);

void SetRasterColor(Uint32);

vec3 Uint2Color(Uint32);

Uint32 Color2Uint(float, float ,float);

vec3 GetPixel(int, int );

void SetPixel(int, int, Uint32);

void SetPixel(int, int, vec3);

void SetPixel (int, int);



#endif /* defined(__softRender__Pixels__) */
