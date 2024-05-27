#ifndef RASTERFONT_HPP
#define RASTERFONT_HPP

class SDL_Texture;
class SDL_Renderer;

class RasterFont
{
public:

    RasterFont(SDL_Renderer *renderer);

    ~RasterFont();

private:

    SDL_Texture *mFontTexture = nullptr;
};

#endif // RASTERFONT_HPP
