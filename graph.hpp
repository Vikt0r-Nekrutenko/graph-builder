#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "vertex.hpp"

class Graph : private std::vector<Vertex>
{
public:

    Graph();

    ~Graph();

    void addNewVertex(int x, int y);

    void addNewEdge(int sx, int sy, int dx, int dy);

    void moveVertex(int sx, int sy, int dx, int dy);

    void print() const;

    void draw(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const;

    void selectVertex(int x, int y);

    void deselectVertex();

    void addNewVertexToSelected(int dx, int dy);

    void moveSelected(int dx, int dy);

    inline bool isVertexSelected() const { return mSelected != nullptr; }

private:

    Vertex *mSelected = nullptr;

    std::vector<Vertex>::iterator findVertex(int x, int y);
};

#endif // GRAPH_HPP
