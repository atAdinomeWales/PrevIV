#ifndef LIB_SDL_INSTANCE_H
#define LIB_SDL_INSTANCE_H

#include <SDL2/SDL.h>

#define CHK_W_SIZE 0

typedef struct {
    SDL_Window *window;
    float windowAspect;
    SDL_Surface *image;
    float imageAspect;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    char* path;
    bool quit;
} sdlInst;

void instanceInit(sdlInst *instance, char* name, unsigned int w, unsigned int h, Uint8 intensity);
void instanceQuit(sdlInst *instance);
int  loadImage(sdlInst *instance, char* path);
void loadImageToTexture(sdlInst *instance, SDL_Surface *image, bool destroyImage);
void updateRender(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest);
void recalcRender(sdlInst* instance, uint16_t imgW, uint16_t imgH, int winW, int winH, int offX, int offY, float zoom);

#endif//LIB_SDL_INSTANCE_H