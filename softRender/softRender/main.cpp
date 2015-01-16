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
#include "drawprim.cpp"

#include <sstream>







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







