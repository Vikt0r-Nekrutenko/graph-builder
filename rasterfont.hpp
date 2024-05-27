#ifndef RASTERFONT_HPP
#define RASTERFONT_HPP

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_stdinc.h"

#define RF_SYM_sW 10
#define RF_SYM_sH 20
#define RF_SYM_dW 5
#define RF_SYM_dH 10
#define RF_MAP_S 13

class SDL_Texture;
class SDL_Renderer;
class SDL_Surface;

class RasterFont
{
public:

    RasterFont(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

    ~RasterFont();

    void updateColor(Uint8 r, Uint8 g, Uint8 b);

    friend void drawSymbol(SDL_Renderer *renderer, const RasterFont &font, unsigned char sym, int x, int y);
    friend void drawText(SDL_Renderer *renderer, const RasterFont &font, unsigned char sym, int x, int y);

private:

    SDL_Rect mSymbolSrcRect {};

    SDL_Surface *mFontSurface = nullptr;
    SDL_Texture *mFontTexture = nullptr;
    Uint8 mR = 0xff,
          mG = 0xff,
          mB = 0xff;

    void changeColor(Uint8 r, Uint8 g, Uint8 b);
};

void drawSymbol(SDL_Renderer *renderer, const RasterFont &font, unsigned char sym, int x, int y);
void drawText(SDL_Renderer *renderer, const RasterFont &font, const char *text, int x, int y);

#endif // RASTERFONT_HPP
