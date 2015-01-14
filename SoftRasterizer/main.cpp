//
//  main.cpp
//  SoftRasterizer
//
//  Created by zpf on 15-1-13.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

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
    pixels[y * 640 + x] = 0xff0000ff;
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


int main(int argc, char** argv) {
    bool quit = false;
    bool leftMouseButtonDown = false;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window * window = SDL_CreateWindow("Draw Pixel",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           640, 480,
                                           0);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STATIC,
                                             640,480);
    //initialize memory to ....uint8
    memset(pixels, 255, 640 * 480 * sizeof(Uint32));
    
    int drawline = 0;
    int drawcircle = 0;
    bool ptCount = false; // whether have a point
    while (!quit) {
        SDL_UpdateTexture(texture, NULL, pixels, 640*sizeof(Uint32));
        SDL_WaitEvent(&event);
        int x1, y1;
        int x2, y2;
        
        switch(event.type){
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                case SDLK_1:
                    drawline = 1;
                    drawcircle = 0;
                    break;
                case SDLK_2:
                    drawcircle = 1;
                    drawline = 0;
                    break;
                }
    
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    leftMouseButtonDown = false;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
                    leftMouseButtonDown = true;
                    if (leftMouseButtonDown == true){
                    if(ptCount == 0){
                        SDL_GetMouseState(&x1, &y1);
                        std::stringstream ss;
                        ss << "  X1:" << x2 << "  X1:" << y2;
                        SDL_SetWindowTitle(window, ss.str().c_str());
                        ptCount = true;
                    } else {
                        SDL_GetMouseState(&x2, &y2);
                        
                        std::stringstream ss;
                        ss << "X2:" << x2 << "  X2:" << y2;
                        SDL_SetWindowTitle(window, ss.str().c_str());
                        ptCount = false;
                        double r  = sqrt(double(((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))));
                        if (drawline)  DrawLine(x1, y1, x2, y2);
                        if (drawcircle)  DrawCircle(x1, y1, int (r));
                    }
                }
            }
                
            case SDL_MOUSEMOTION:
                if (leftMouseButtonDown){
                    //int mouseX = event.motion.x;
                    //int mouseY = event.motion.y;
                }
                break;
                
            case SDL_QUIT:
                quit = true;
                break;
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL,NULL);
        SDL_RenderPresent(renderer);
    }
    
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    return 0;
}




















