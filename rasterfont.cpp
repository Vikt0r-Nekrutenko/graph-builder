#include "rasterfont.hpp"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"

RasterFont::RasterFont(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface *surf = SDL_LoadBMP("raster_font_.bmp");
    for(int y = 0; y < surf->h; ++y) {
        for(int x = 0; x < surf->w; ++x) {
            Uint8 *pixel = ((Uint8 *) surf->pixels +
                            y * surf->pitch +
                            x * surf->format->BytesPerPixel);
            Uint8 &_a = pixel[3],
                  &_r = pixel[2],
                  &_g = pixel[1],
                  &_b = pixel[0];
            if(_a == 0xff) {
                mR = _r = r;
                mG = _g = g;
                mB = _b = b;
            }
        }
    }
    mFontTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

RasterFont::~RasterFont()
{
    SDL_DestroyTexture(mFontTexture);
}
