#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <cmath>
#include <iostream>
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

class Graph : private std::vector<Vertex>
{
public:

    void addv(int x, int y);

    void adde(int sx, int sy, int dx, int dy);

    void movev(int sx, int sy, int dx, int dy);

    void print() const;

    void draw(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const;

private:

    Vertex *mSelected = nullptr;

    std::vector<Vertex>::iterator findVertex(int x, int y);
};

#endif // GRAPH_HPP
