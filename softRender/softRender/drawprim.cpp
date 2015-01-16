//
//  drawline.cpp
//  softRender
//
//  Created by zpf on 15-1-15.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include "Pixels.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <iostream>

#include <sstream>

Uint32 * pixels = new Uint32[640*480];
void SetPixel(int x, int y) {
    // pixel easyliy out of range especially draw circle, so
    if(x > 640)  x = x -640;
    if(y > 480)  y = y -480;
    if (y < 0)   y = 480 - y;
    if (x < 0)   x = 640 - x;
    pixels[y * 640 + x] = 0; // you can change color here
    //    Uint8 *colors = (Uint8*)&pixels;
    //    pixels[y * 640 +x] = colors[0];
}
void DrawCirclePoint(int xc, int yc, int x, int y) {
    SetPixel(xc + x, yc + y);
    SetPixel(xc - x, yc - y);
    SetPixel(xc + x, yc - y);
    SetPixel(xc - x, yc + y);
    SetPixel(xc + y, yc + x);
    SetPixel(xc - y, yc - x);
    SetPixel(xc + y, yc - x);
    SetPixel(xc - y, yc + x);
}

void DrawCircle(int xc, int yc, int r) {
    int x = 0; int y = r;
    int p = 1 - r;
    
    SetPixel(xc, yc +r);
    
    while ( x < y) {
        
        x ++;
        
        if ( p < 0) {
            p += 2 * x +1;
        } else {
            y--;
            p += 2* (x - y) + 1;
        }
        DrawCirclePoint(xc, yc, x, y);
    }
}

void DrawLine(int x0, int y0,  int xEnd,  int yEnd) {
    const bool steep = (fabs(yEnd - y0) > fabs(xEnd - x0)); //whether the slope greater than 1
    
    if (steep) {
        std::swap(x0, y0);
        std::swap(xEnd, yEnd);
    }
    
    if (x0 > xEnd){
        std::swap(x0, xEnd);
        std::swap(y0, yEnd);
    }
    
    const int dx = xEnd - x0; // xEnd always greater than x0
    const int dy = fabs(yEnd - y0);
    float error = dx /2.0f;
    const int ystep = (y0 < yEnd) ? 1: -1;
    int y = y0;
    
    const int maxX = xEnd;
    //draw point
    if (x0 == xEnd & y0 == yEnd) SetPixel(x0,y0);
    for (int x = x0; x < maxX; x++){
        if (steep){
            SetPixel(y, x);
        } else {
            SetPixel(x, y);
        }
        
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}




















