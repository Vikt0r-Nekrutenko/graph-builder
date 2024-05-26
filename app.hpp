#ifndef APP_HPP
#define APP_HPP

#include <stack>

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "graph.hpp"

#define WindowWidth 640
#define WindowHeight 480

enum class EditMode {
    None,
    AddVertex,
    AddEdge,
};

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
    SDL_Texture *mRegularEdgeTexture = nullptr;
    SDL_Texture *mSelectedEdgeTexture = nullptr;

    int mScaleCoeffitient = 1, mXOffset = 0, mYOffset = 0;
    EditMode mEditMode = EditMode::None;
    bool isContinue = true;

    bool onUpdateHandler();
    void onClickHandler(const SDL_MouseButtonEvent &button);
    void onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion);
    void onKeyHandler(const SDL_Keysym &keysym);
    void drawBackground(int scaleCoeffitient, int xOffset, int yOffset);
};

#endif // APP_HPP
