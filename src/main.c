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


int main(int argc, char*argv[]){
    sdlInst instance;
    bool debug;
    if(argvFind(argc, argv, "-d") > 0){
        debug = !DEFAULT_DEBUG_STATUS;
    } else {
        debug = DEFAULT_DEBUG_STATUS;
    }

    instance.path = argvScan(argc, argv);

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

    if(instance.path){
        loadImage(&instance, instance.path, true);
        if(!instance.image){
            DEBUG(debug, "Path: %s not found\n", instance.path);
        }
    } else {
        loadImage(&instance, IMG_PATH, false);
    }
    instance.path = NULL;
    
    DEBUG(debug, "Image width: %d, height: %d\n", instance.imgW, instance.imgH);

    loadImageToTexture(&instance, instance.image, false);
    SDL_RenderPresent(instance.renderer);


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
                        SDL_GetMouseState(&mouseX, &mouseY);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    drag = false;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    break;
                case SDL_MOUSEWHEEL:
                    zoomRecalc(-event.wheel.y, &zoom, &offX, &offY, &instance);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_c:
                            onMouseRecalc(&instance, &zoom, &offX, &offY);
                            break;
                        case SDLK_m:
                            offX = 0;
                            offY = 0;
                            zoom = 1;
                            recalcRender(&instance, instance.imgW, instance.imgH, 0, 0, 1);
                            break;
                        case SDLK_p:
                            zoomRecalc(1,  &zoom, &offX, &offY, &instance);
                            break;
                        case SDLK_l:
                            zoomRecalc(-1, &zoom, &offX, &offY, &instance);
                            break;
                    }
                    break;
            }

            moveRecalc(drag, &instance, &offX, &offY, &mouseX, &mouseY, &zoom);
        }
    }

    instanceQuit(&instance);
    SDL_Quit();

    return 0;
}