#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "libStdAux.h"
#include "libSdlInstance.h"

#define IMG_PATH "../default.jpg"
#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   800
#define WINDOW_NAME "Preview Image Viewer"
#define WINDOW_BACKGROUND_INTENSITY 230


int main(int argc, char*argv[]){
    sdlInst instance;
    bool debug;
    if(argvFind(argc, argv, "-d") > 0){
        debug = !DEFAULT_DEBUG_STATUS;
    } else {
        debug = DEFAULT_DEBUG_STATUS;
    }

    ASSERT(
        !(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)), 
        "Failed to initialize, error thrown: %s\n", 
        SDL_GetError()
    );

    ASSERT(
        IMG_Init(IMG_INIT_JPG),
        "Failed to initialize image, error thrown: %s\n",
        IMG_GetError()
    );

    instanceInit(&instance, WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BACKGROUND_INTENSITY);

    instance.path = argvScan(argc, argv);
    if(instance.path){
        loadImage(&instance, instance.path);
        if(!instance.image){
            DEBUG(debug, "Path: %s not found\n", instance.path);
        }
    } else {
        loadImage(&instance, IMG_PATH);
    }
    free(instance.path);
    instance.path = NULL;
    
    DEBUG(debug, "Image width: %d, height: %d\n", instance.image->w, instance.image->h);
    int imgH, imgW;
    imgH = instance.image->h;
    imgW = instance.image->w;
    loadImageToTexture(&instance, instance.image);


    SDL_Event event;
    bool drag = false;
    float zoom = 1.0;
    int mouseX = 0;
    int mouseY = 0;
    int offX = 0;
    int offY = 0;

    while(!(instance.quit)){
        if (SDL_WaitEvent(&event))
        {
            switch (event.type){
                case SDL_QUIT:
                    instance.quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        drag = true;
                        mouseX = event.button.x;
                        mouseY = event.button.y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (drag){
                        drag = false;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y < 0 && zoom > 0.15){
                        zoom -= 0.15f * zoom;
                        SDL_Rect destRect;
                        destRect.h = imgH * zoom;
                        destRect.w = imgW * zoom;
                        destRect.x = WINDOW_WIDTH / 2 - destRect.w / 2 + offX;
                        destRect.y = WINDOW_HEIGHT / 2 - destRect.h / 2 + offY;
                        updateRender(instance.renderer, instance.texture, NULL, &destRect);
                    } else if (event.wheel.y > 0){
                        zoom += 0.15f * zoom;
                        SDL_Rect destRect;
                        destRect.h = imgH * zoom;
                        destRect.w = imgW * zoom;
                        destRect.x = WINDOW_WIDTH / 2 - destRect.w / 2 + offX;
                        destRect.y = WINDOW_HEIGHT / 2 - destRect.h / 2 + offY;
                        updateRender(instance.renderer, instance.texture, NULL, &destRect);
                    }
                    break;
            }

            if (drag){
                int newX, newY;
                SDL_GetMouseState(&newX, &newY);
                offX = -mouseX + newX;
                offY = -mouseY + newY;
                SDL_Rect destRect;
                destRect.h = imgH * zoom;
                destRect.w = imgW * zoom;
                destRect.x = WINDOW_WIDTH / 2 - destRect.w / 2 + offX;
                destRect.y = WINDOW_HEIGHT / 2 - destRect.h / 2 + offY;
                updateRender(instance.renderer, instance.texture, NULL, &destRect);
            }
        }
    }

    instanceQuit(&instance);
    SDL_Quit();

    return 0;
}