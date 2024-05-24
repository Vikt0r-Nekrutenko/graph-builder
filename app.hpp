#ifndef APP_HPP
#define APP_HPP

#include "SDL2/SDL.h"

#include <stack>

#include "graph.hpp"

#define WindowWidth 640
#define WindowHeight 480

class App
{
public:

    App();
    ~App();

    int run(int fps);

private:

    Graph mGraph;
    std::stack<Graph> mHistory;

    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    SDL_Texture *mBackground = nullptr;

    int mScaleCoeffitient = 1, mXOffset = 0, mYOffset = 0;
    bool isContinue = true;

    bool onUpdateHandler();
    void onClickHandler(const SDL_MouseButtonEvent &button);
    void onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion);
    void onKeyHandler(const SDL_Keysym &keysym);
    void drawBackground(int scaleCoeffitient, int xOffset, int yOffset);
};

#endif // APP_HPP
