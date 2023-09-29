#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "libSdlInstance.h"
#include "libStdAux.h"

#define Uint8 unsigned char


void instanceInit(sdlInst *instance, char* name, unsigned int w, unsigned int h, Uint8 intensity){
    // Init window
    instance->window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
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
    SDL_SetRenderDrawColor(instance->renderer, intensity, intensity, intensity, 255);
    SDL_RenderClear(instance->renderer);
    SDL_RenderPresent(instance->renderer);

    // Init loop
    instance->quit = false;

    return;
}

void instanceQuit(sdlInst *instance){
    SDL_DestroyWindow(instance->window);
    SDL_DestroyRenderer(instance->renderer);
}

int loadImage(sdlInst *instance, char* path){
    instance->image = IMG_Load(path);
    ASSERT(
        instance->image,
        "Failed to load image, check path. Error: %s\n",
        IMG_GetError()
    );
    if (instance->image == NULL){
        printf("Could not locate image, likely due to an invalid path");
        return 0;
    }

    return 1;
}

void loadImageToTexture(sdlInst *instance, SDL_Surface *image) {
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