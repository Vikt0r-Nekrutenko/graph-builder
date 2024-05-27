#include "rasterfont.hpp"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include <string>

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
    if(mR == r && mG == g && mB == b)
        return;

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

void drawSymbol(SDL_Renderer *renderer, const RasterFont &font, unsigned char sym, int x, int y)
{
    SDL_Rect srcRect {
            RF_SYM_sW * (sym % RF_MAP_S),
        3 + RF_SYM_sH * (sym / RF_MAP_S + 1),
        RF_SYM_sW,
        RF_SYM_sH
    };
    SDL_Rect destRect {
        x,
        y,
        RF_SYM_dW,
        RF_SYM_dH
    };
    SDL_RenderCopy(renderer, font.mFontTexture, &srcRect, &destRect);
}

void drawText(SDL_Renderer *renderer, const RasterFont &font, const char *text, int x, int y)
{
    int it = 0;
    const char *ptr = text;
    while(*ptr != 0) {
        drawSymbol(renderer, font, *ptr++, x + it, y);
        it += RF_SYM_dW;
    }
}

void drawNumber(SDL_Renderer *renderer, const RasterFont &font, int n, int x, int y)
{
    drawText(renderer, font, std::to_string(n).c_str(), x, y);
}

void drawNumber(SDL_Renderer *renderer, const RasterFont &font, double n, int x, int y)
{
    drawText(renderer, font, std::to_string(n).c_str(), x, y);
}
