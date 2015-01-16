//
//  Pixels.cpp
//  softRender
//
//  Created by zpf on 15-1-15.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include "Pixels.h"

#define SATURATE(A) if (A < 0) A=0; else if (A > 1) A =1;

SDL_Surface* surface;

Uint32 currentColor = 0xffffffff;

vec3 Uint2Color(Uint32 color) {
    Uint8 r, g, b;
    SDL_GetRGB(color, surface->format, &r, &g, &b);
    
    return vec3(r, g, b);
}

Uint32 Color2Uint(float r, float g, float b) {
    SATURATE(r); SATURATE(g); SATURATE(b);
    
    return SDL_MapRGB(surface -> format, r * 255, g * 255, b * 255);
}

void SetRasterSurface(SDL_Surface *s) {
    surface = s;
}

void SetRasterColor(float r, float g, float b) {
    currentColor = Color2Uint(r, g, b);
}

void SetRasterColor(Uint32 color) {
    currentColor = color;
}

Uint32 GetPixelUint(int x, int y) {
    int bpp = surface -> format -> BytesPerPixel;
    
    Uint8 *p = (Uint8* )surface->pixels + y * surface -> pitch + x * bpp;
    
    switch (bpp) {
        case 1:
            return *p;
            
        case 2:
            return *(Uint16 *) p;
            
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2]<< 16;
            
        case 4:
            return *(Uint32 *)p;
            
        default:
            return 0;
    }
}

vec3 GetPixel(int x, int y) {
    Uint8 r, g, b;
    SDL_GetRGB(GetPixelUint(x, y), surface -> format, &r, &g, &b);
    
    return vec3(r/ 255.0, g/255.0, b/255.0);
}

void SetPixel(int x, int y, vec3 color) {
    SetPixel(x, y, Color2Uint(color.x, color.y, color.z));
}

void SetPixel(int x, int y) {
    SetPixel(x, y, currentColor);
}

void SetPixel(int x, int y, Uint32 pixel) {
    if (x >= surface -> w || x < 0 || y >= surface->h || y < 0) {
        return;
    }
    
    int bpp = surface -> format -> BytesPerPixel;
    
    Uint8 *p = (Uint8 *) surface ->pixels + y * surface -> pitch + x * bpp;
    
    switch (bpp) {
        case 1:
            *p = pixel;
            break;
            
        case 2:
            *(Uint16 *)p = pixel;
            break;
            
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8)  & 0xff;
                p[2] =  pixel        & 0xff;
            } else {
                p[2] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8)  & 0xff;
                p[0] =  pixel        & 0xff;
            }
            break;
            
        case 4:
            *(Uint32 *)p = pixel;
            break;
            
        default:
            break;
    }
}




























