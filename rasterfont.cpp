#include "rasterfont.hpp"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"

RasterFont::RasterFont(SDL_Renderer *renderer)
{
    SDL_Surface *surf = SDL_LoadBMP("raster_font.bmp");
    for(int y = 0; y < surf->h; ++y) {
        for(int x = 0; x < surf->w; ++x) {
            Uint8 *pixel = ((Uint8 *) surf->pixels +
                            y * surf->pitch +
                            x * surf->format->BytesPerPixel);
            Uint8 &a = pixel[3],
                  &r = pixel[2],
                  &g = pixel[1],
                  &b = pixel[0];
            if(r < 150 || g < 150 || b < 150) {
                a = 0;
            } else {
                r = g = b = a = 0xff;
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
