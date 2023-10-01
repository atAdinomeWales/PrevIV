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
#define WINDOW_BACKGROUND_INTENSITY 50
#define ZOOM_FACTOR 0.10f
#define MIN_ZOOM_RESOLUTION 200


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
    loadImageToTexture(&instance, instance.image, false);
    SDL_RenderPresent(instance.renderer);


    SDL_Event event;
    bool drag = false;
    float zoom = 1.0;
    int mouseX = 0;
    int mouseY = 0;
    int offX = 0;
    int offY = 0;
    int bigRes;
    if (imgW > imgH){
        bigRes = imgW;
    } else {
        bigRes = imgH;
    }

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
                        SDL_GetMouseState(&mouseX, &mouseY);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (drag){
                        drag = false;
                        SDL_GetMouseState(&mouseX, &mouseY);
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y < 0){
                        float newZoom = zoom - ( ZOOM_FACTOR * zoom );
                        if( (bigRes * newZoom) > MIN_ZOOM_RESOLUTION){
                            zoom = newZoom;
                        }
                    } else if (event.wheel.y > 0){
                        zoom += ZOOM_FACTOR * zoom;
                    }
                    break;
            }

            if (drag){
                int newX, newY;
                SDL_GetMouseState(&newX, &newY);
                offX += newX - mouseX;
                offY += newY - mouseY;
                mouseX = newX;
                mouseY = newY;
            }

            recalcRender(&instance, imgW, imgH, WINDOW_WIDTH, WINDOW_HEIGHT, offX, offY, zoom);
        }
    }

    instanceQuit(&instance);
    SDL_Quit();

    return 0;
}