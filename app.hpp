#ifndef APP_HPP
#define APP_HPP

#include "SDL2/SDL.h"

#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <vector>

#define WindowWidth 640
#define WindowHeight 480

typedef struct _Edge {
    int x, y, nextVId;
} Edge;

typedef struct _Vertex {
    std::vector<Edge> edges;
    int x, y;
} Vertex;

using Graph = std::vector<Vertex>;

void drawGraph(SDL_Renderer *renderer, const Graph &graph, int scaleCoeffitient, int xOffset, int yOffset);
void drawSelectedVertex(SDL_Renderer *renderer, const Vertex *selected, int scaleCoeffitient, int xOffset, int yOffset);

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

    Graph mGraph;
    Vertex *mSelected = nullptr;

    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;

    int mScaleCoeffitient = 1, mXOffset = 0, mYOffset = 0;
    bool isContinue = true;
};

#endif // APP_HPP
