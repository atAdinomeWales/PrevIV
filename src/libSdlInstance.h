#ifndef LIB_SDL_INSTANCE_H
#define LIB_SDL_INSTANCE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define ZOOM_FACTOR 0.10f
#define MIN_ZOOM_RESOLUTION 200

typedef struct {
    SDL_Window *window;
    float windowAspect;
    SDL_Surface *image;
    float imageAspect;
    int imgW;
    int imgH;
    int bigRes;
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
void recalcRender(sdlInst* instance, uint16_t imgW, uint16_t imgH, int offX, int offY, float zoom);
void zoomRecalc(float input, int refResolution, float *currZoom, int *offX, int *offY,sdlInst *instance);
void onMouseRecalc(sdlInst* instance, float *zoom, int *offX, int *offY);
void moveRecalc(bool drag, sdlInst* instance, int *offX, int *offY, int *prevMouseX, int *prevMouseY, float *currZoom);

#endif//LIB_SDL_INSTANCE_H