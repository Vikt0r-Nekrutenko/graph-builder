#include "rasterfont.hpp"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"

RasterFont::RasterFont(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b)
{
    mFontSurface = SDL_LoadBMP("raster_font_.bmp");
    changeColor(r, g, b);
    mFontTexture = SDL_CreateTextureFromSurface(renderer, mFontSurface);
}

RasterFont::~RasterFont()
{
    SDL_FreeSurface(mFontSurface);
    SDL_DestroyTexture(mFontTexture);
}

void RasterFont::updateColor(Uint8 r, Uint8 g, Uint8 b)
{
    changeColor(r, g, b);
    SDL_UpdateTexture(mFontTexture, NULL, mFontSurface->pixels, mFontSurface->pitch);
}

void RasterFont::changeColor(Uint8 r, Uint8 g, Uint8 b)
{
    for(int y = 0; y < mFontSurface->h; ++y) {
        for(int x = 0; x < mFontSurface->w; ++x) {
            Uint8 *pixel = ((Uint8 *) mFontSurface->pixels +
                            y * mFontSurface->pitch +
                            x * mFontSurface->format->BytesPerPixel);
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
}
