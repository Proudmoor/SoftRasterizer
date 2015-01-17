//
//  main.cpp
//  SoftRasterizer
//
//  Created by zpf on 15-1-13.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include <SDL2/SDL.h>
#include <iostream>
#include "drawprim.cpp"
#include <stdlib.h>

#include <sstream>



SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture*  backBuffer;
SDL_Surface*  gSurface;

Uint32 *colorBuffer;
float * depthBuffer;

int height = 480; int width = 640;

bool Init(bool fullscreen) {
    if (!SDL_Init(SDL_INIT_EVERYTHING)) {
        return false;
    }
    
    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }
    
    gWindow = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width, height, windowFlags);
    if (gWindow == nullptr)
        return false;
    SDL_Surface *surface = SDL_GetWindowSurface(gWindow);
    if (surface == nullptr)
        return false;
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
    if (gRenderer == nullptr)
        return false;
    backBuffer = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (backBuffer == nullptr)
        return false;
    depthBuffer =  (float *) malloc (width * height * sizeof(float));
    
    SDL_ShowCursor(false);
    SDL_SetWindowGrab(gWindow, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    return true;
    
}

void Close() {
    free(depthBuffer);
    
    SDL_DestroyTexture(backBuffer);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    
    SDL_Quit();
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
                            if (drawline)   DrawLine(x1, y1, x2, y2);
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







