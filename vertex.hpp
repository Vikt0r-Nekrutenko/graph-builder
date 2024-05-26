#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "SDL2/SDL_stdinc.h"
#include <vector>

#define VertexRectSize 8

typedef struct _Edge {
    int x = 0, y = 0, nextVId = -1;
} Edge;

class SDL_Renderer;
class SDL_Texture;

void createThickLine(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);
void drawThickLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int scale, int xoff, int yoff, int thickness);
void deleteThickLine();

class Vertex : private std::vector<Edge>
{
    friend class Graph;

public:

    Vertex(int x, int y);

    void print() const;

    void add(int x, int y, int id);

    void move(int dx, int dy);

    void draw(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const;

    void drawEdges(SDL_Renderer *renderer, int scale, int ox, int oy) const;

    void drawVertex(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const;

private:

    int mX = -1, mY = -1;

    std::vector<Edge>::iterator findEdge(int x, int y);
};

#endif // VERTEX_HPP
