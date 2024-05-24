#include "vertex.hpp"
#include "SDL2/SDL_render.h"

#include <algorithm>
#include <iostream>


Vertex::Vertex(int x, int y) : mX{x}, mY{y} {}

void Vertex::print() const
{
    std::cout << "Vertex [" << mX << ":" << mY << "]. Size: " << size() << std::endl;
    for(const auto &edge: *this){
        std::cout << "\t" << mX << ":" << mY << " - " << edge.x << ":" << edge.y << " | " << edge.nextVId << std::endl;
    }
}

void Vertex::add(int x, int y, int id)
{
    auto edge = findEdge(x, y);
    if(edge != end())
        std::cout << "Edge does exist!" << std::endl;
    else
        push_back({x, y, id});
}

void Vertex::move(int dx, int dy)
{
    mX = dx;
    mY = dy;
}

void Vertex::draw(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect rect {
        mX * scale + ox - (VertexRectSize >> 1),
        mY * scale + oy - (VertexRectSize >> 1),
        VertexRectSize,
        VertexRectSize
    };
    SDL_RenderDrawRect(renderer, &rect);
    for(const auto &edge: *this) {
        SDL_RenderDrawLine(renderer,
                           mX * scale + ox,
                           mY * scale + oy,
                           edge.x * scale + ox,
                           edge.y * scale + oy);
    }
}

std::vector<Edge>::iterator Vertex::findEdge(int x, int y)
{
    return std::find_if(begin(), end(), [&](const Edge &e){ return e.x == x && e.y == y; });
}
