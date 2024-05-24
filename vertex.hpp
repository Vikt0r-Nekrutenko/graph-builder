#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>

#define VertexRectSize 8

typedef struct _Edge {
    int x = 0, y = 0, nextVId = -1;
} Edge;

class SDL_Renderer;

class Vertex : private std::vector<Edge>
{
    friend class Graph;

public:

    Vertex(int x, int y);

    void print() const;

    void adde(int x, int y, int id);

    void move(int dx, int dy);

    void draw(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const;

private:

    int mX = -1, mY = -1;

    std::vector<Edge>::iterator findEdge(int x, int y);
};

#endif // VERTEX_HPP
