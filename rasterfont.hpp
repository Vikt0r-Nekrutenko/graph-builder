#ifndef RASTERFONT_HPP
#define RASTERFONT_HPP

#include "SDL2/SDL_stdinc.h"

class SDL_Texture;
class SDL_Renderer;
class SDL_Surface;

class RasterFont
{
public:

    RasterFont(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

    ~RasterFont();

    void updateColor(Uint8 r, Uint8 g, Uint8 b);

private:

    SDL_Surface *mFontSurface = nullptr;
    SDL_Texture *mFontTexture = nullptr;
    Uint8 mR = 0xff,
          mG = 0xff,
          mB = 0xff;

    void changeColor(Uint8 r, Uint8 g, Uint8 b);
};

#endif // RASTERFONT_HPP
