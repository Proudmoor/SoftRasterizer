//
//  main.cpp
//  softRender
//
//  Created by zpf on 15-1-14.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

//global variables
SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;


bool init(){
    bool success =true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL cannot initialize! Error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("Texture", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, 480, 640, 0);
        if (gWindow == NULL) {
            std::cout << "Window cannot be created! Error: "<< SDL_GetError() << std::endl;
            success = false;
        } else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia(){
    bool success = true;
    
    return success;
}

void close(){
    SDL_FreeSurface(gHelloWorld);
    
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    SDL_Quit();
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
