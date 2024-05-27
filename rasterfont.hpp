#ifndef RASTERFONT_HPP
#define RASTERFONT_HPP

#include "SDL2/SDL_stdinc.h"

class SDL_Texture;
class SDL_Renderer;

class RasterFont
{
public:

    RasterFont(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

    ~RasterFont();

private:

    SDL_Texture *mFontTexture = nullptr;
    Uint8 mR = 0xff,
          mG = 0xff,
          mB = 0xff;
};

#endif // RASTERFONT_HPP
