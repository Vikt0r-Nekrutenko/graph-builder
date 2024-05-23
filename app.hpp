#ifndef APP_HPP
#define APP_HPP

#include "SDL2/SDL.h"

class App
{
public:

    App();
    ~App();

    bool onUpdateHandler();
    void onClickHandler(const SDL_MouseButtonEvent &button);
    void onKeyHandler(const SDL_Keysym &keysym);
    int run(int fps);

private:

    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;

    bool isContinue = true;
};

#endif // APP_HPP
