#ifndef LIB_SDL_INSTANCE_H
#define LIB_SDL_INSTANCE_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Surface *image;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    char* path;
    bool quit;
} sdlInst;

void instanceInit(sdlInst *instance, char* name, unsigned int w, unsigned int h, Uint8 intensity);
void instanceQuit(sdlInst *instance);
int  loadImage(sdlInst *instance, char* path);
void loadImageToTexture(sdlInst *instance, SDL_Surface *image);
void updateRender(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest);

#endif//LIB_SDL_INSTANCE_H