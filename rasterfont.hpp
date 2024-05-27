#ifndef RASTERFONT_HPP
#define RASTERFONT_HPP

#include "SDL2/SDL_stdinc.h"

class SDL_Texture;
class SDL_Renderer;
class SDL_Surface;

class RasterFont
{
public:

    static constexpr int W = 5;
    static constexpr int H = 10;

    RasterFont(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

    ~RasterFont();

    void updateColor(Uint8 r, Uint8 g, Uint8 b);

    void scalePlus();

    void scaleMinus();

    friend void drawSymbol(SDL_Renderer *renderer, const RasterFont &font, unsigned char sym, int x, int y);
    friend int drawText(SDL_Renderer *renderer, const RasterFont &font, const char *text, int x, int y);
    friend int drawNumber(SDL_Renderer *renderer, const RasterFont &font, int n, int x, int y);
    friend int drawNumber(SDL_Renderer *renderer, const RasterFont &font, double n, int x, int y);
    friend int draw(SDL_Renderer *renderer, const RasterFont &font, int x, int y, const char *format, ...);

private:

    SDL_Surface *mFontSurface = nullptr;
    SDL_Texture *mFontTexture = nullptr;
    int mFontW, mFontH, mScaleMultiplier = 1;
    Uint8 mR = 0xff,
          mG = 0xff,
          mB = 0xff;

    void changeColor(Uint8 r, Uint8 g, Uint8 b);
};

void drawSymbol(SDL_Renderer *renderer, const RasterFont &font, unsigned char sym, int x, int y);
int drawText(SDL_Renderer *renderer, const RasterFont &font, const char *text, int x, int y);
int drawNumber(SDL_Renderer *renderer, const RasterFont &font, int n, int x, int y);
int drawNumber(SDL_Renderer *renderer, const RasterFont &font, double n, int x, int y);
int draw(SDL_Renderer *renderer, const RasterFont &font, int x, int y, const char *format, ...);

#endif // RASTERFONT_HPP
