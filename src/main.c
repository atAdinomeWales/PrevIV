#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "libStdAux.h"

#define IMG_PATH "../example.jpg"
#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   800
#define WINDOW_NAME "Preview Image Viewer"
#define WINDOW_BACKGROUND_INTENSITY 230

struct instance {
    SDL_Window *window;
    SDL_Surface *image;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    bool quit;
} instance;

void instanceInit(struct instance *instance);
void instanceQuit(struct instance *instance);
int  loadImage(struct instance *instance, char* path);
void loadImageToTexture(struct instance *instance, SDL_Surface *image);
void updateRender(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest);

int main(int argc, char*argv[]){
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

    instanceInit(&instance);

    char* imgPath = argvScan(argc, argv);
    if(imgPath){
        loadImage(&instance, imgPath);
        if(!instance.image){
            DEBUG(debug, "Path: %s not found\n", imgPath);
        }
    } else {
        loadImage(&instance, IMG_PATH);
    }
    free(imgPath);
    
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
        while (SDL_PollEvent(&event))
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

void instanceInit(struct instance *instance){
    // Init window
    instance->window = SDL_CreateWindow(
        WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    ASSERT(
        instance->window,
        "Failed to initialize window, error thrown: %s\n",
        SDL_GetError()
    );

    // Init renderer
    instance->renderer = SDL_CreateRenderer(
        instance->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    ASSERT(
        instance->renderer,
        "Failed to initialize renderer, error thrown: %s\n",
        SDL_GetError()
    );
    SDL_SetRenderDrawColor(instance->renderer, WINDOW_BACKGROUND_INTENSITY, WINDOW_BACKGROUND_INTENSITY, WINDOW_BACKGROUND_INTENSITY, 255);
    SDL_RenderClear(instance->renderer);
    SDL_RenderPresent(instance->renderer);

    // Init loop
    instance->quit = false;

    return;
}

void instanceQuit(struct instance *instance){
    SDL_DestroyWindow(instance->window);
    SDL_DestroyRenderer(instance->renderer);
}

int loadImage(struct instance *instance, char* path){
    instance->image = IMG_Load(path);
    ASSERT(
        instance->image,
        "Failed to load image, check path. Error: %s\n",
        IMG_GetError()
    )
    if (instance->image == NULL){
        printf("Could not locate image, likely due to an invalid path");
        return 0;
    }

    return 1;
}

void loadImageToTexture(struct instance *instance, SDL_Surface *image) {
    int windowW, windowH;
    SDL_GetWindowSize(instance->window, &windowW, &windowH);

    // Calculate the aspect ratio of the image and window
    float imageAspect = (float)image->w / image->h;
    float windowAspect = (float)windowW / windowH;

    SDL_Rect destRect;

    if (imageAspect > windowAspect) {
        // Image is wider than the window, scale by width
        destRect.w = windowW;
        destRect.h = (int)(windowW / imageAspect);
    } else {
        // Image is taller than the window, scale by height
        destRect.h = windowH;
        destRect.w = (int)(windowH * imageAspect);
    }

    destRect.x = (windowW - destRect.w) / 2;
    destRect.y = (windowH - destRect.h) / 2;

    instance->texture = SDL_CreateTextureFromSurface(instance->renderer, image);
    ASSERT(instance->texture, "Failed to load image as texture, error thrown: %s\n", SDL_GetError());

    SDL_FreeSurface(image);

    SDL_RenderClear(instance->renderer);
    SDL_RenderCopy(instance->renderer, instance->texture, NULL, &destRect);
    SDL_RenderPresent(instance->renderer);
}

void updateRender(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, source, dest);
    SDL_RenderPresent(renderer);
}