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
    int x = 0, y = 0, nextVId = 0;
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

    int run(int fps);

private:

    Graph mGraph;
    Vertex *mSelected = nullptr;

    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    SDL_Texture *mBackground = nullptr;

    int mScaleCoeffitient = 1, mXOffset = 0, mYOffset = 0;
    bool isContinue = true;

    bool onUpdateHandler();
    void onClickHandler(const SDL_MouseButtonEvent &button);
    void onKeyHandler(const SDL_Keysym &keysym);

    Graph::iterator findExistVertex(int x, int y);
    std::vector<Edge>::iterator findExistEdge(Vertex *source, Vertex *destination);

    Vertex *leftButtonClickHandler(Graph::iterator vertexIt, int mouseX, int mouseY);
    Vertex *rightButtonClickHandler(Graph::iterator vertexIt, int mouseX, int mouseY);
};

#endif // APP_HPP
