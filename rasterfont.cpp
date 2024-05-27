#include "rasterfont.hpp"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include <algorithm>
#include <cstring>
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

int drawText(SDL_Renderer *renderer, const RasterFont &font, const char *text, int x, int y)
{
    int it = 0;
    const char *ptr = text;
    while(*ptr != 0) {
        drawSymbol(renderer, font, *ptr++, x + it, y);
        it += RF_SYM_dW;
    }
    return it;
}

int drawNumber(SDL_Renderer *renderer, const RasterFont &font, int n, int x, int y)
{
    return drawText(renderer, font, std::to_string(n).c_str(), x, y);
}

int drawNumber(SDL_Renderer *renderer, const RasterFont &font, double n, int x, int y)
{
    return drawText(renderer, font, std::to_string(n).c_str(), x, y);
}

int draw(SDL_Renderer *renderer, const RasterFont &font, int x, int y, const char *format, ...)
{
    va_list vl;
    va_start(vl, format);

    int it = 0;

    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%') {
            ptr++;
            if (*ptr == 's') {
                const char *str = va_arg(vl, const char *);
                it += drawText(renderer, font, str, x + it, y);
            } else if(*ptr == 'd') {
                const int n = va_arg(vl, int);
                it += drawNumber(renderer, font, n, x + it, y);
            } else if(*ptr == 'f') {
                const double n = va_arg(vl, double);
                std::string ns = std::to_string(n);
                if(*++ptr == '.') {
                    int symAfterPoint = *++ptr - '0' + 1;
                    auto itr = std::find_if(ns.begin(), ns.end(), [&](const char &s){ return s == '.'; });
                    ns = std::string(ns.begin(), itr + symAfterPoint);
                }
                it += drawText(renderer, font, ns.c_str(), x + it, y);
            }
        } else {
            drawSymbol(renderer, font, *ptr, x + it, y);
            it += RF_SYM_dW;
        }
    }
    va_end(vl);
    return it;
}
