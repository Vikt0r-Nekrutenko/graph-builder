#ifndef APP_HPP
#define APP_HPP

#include "SDL2/SDL.h"

#include <stack>

#include "graph.hpp"

#define WindowWidth 640
#define WindowHeight 480

typedef struct _Vertex {
    std::vector<Edge> edges;
    int x, y;
} VertexOld;

using GraphOld = std::vector<VertexOld>;

void drawBackground(SDL_Renderer *renderer, SDL_Texture *texture, int scaleCoeffitient, int xOffset, int yOffset);
void drawGraph(SDL_Renderer *renderer, const GraphOld &graph, int scaleCoeffitient, int xOffset, int yOffset);
void drawSelectedVertex(SDL_Renderer *renderer, const VertexOld *selected, int scaleCoeffitient, int xOffset, int yOffset);

class App
{
public:

    App();
    ~App();

    int run(int fps);

private:

    GraphOld mGraph;
    Graph graph;
    std::stack<GraphOld> mHistory;

    VertexOld *mSelected = nullptr;

    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    SDL_Texture *mBackground = nullptr;

    int mScaleCoeffitient = 1, mXOffset = 0, mYOffset = 0;
    bool isContinue = true;

    bool onUpdateHandler();
    void onClickHandler(const SDL_MouseButtonEvent &button);
    void onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion);
    void onKeyHandler(const SDL_Keysym &keysym);

    GraphOld::iterator findExistVertex(int x, int y);
    std::vector<Edge>::iterator findExistEdge(VertexOld *source, VertexOld *destination);

    VertexOld *leftButtonClickHandler(GraphOld::iterator vertexIt, int mouseX, int mouseY);
    VertexOld *leftButtonDragHandler(int mouseX, int mouseY);
    VertexOld *rightButtonClickHandler(GraphOld::iterator vertexIt, int mouseX, int mouseY);
};

#endif // APP_HPP
