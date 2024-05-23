#include "graph.hpp"
#include "SDL2/SDL_render.h"


Vertex::Vertex(int x, int y) : mX{x}, mY{y} {}

void Vertex::print() const
{
    std::cout << "Vertex [" << mX << ":" << mY << "]. Size: " << size() << std::endl;
    for(const auto &edge: *this){
        std::cout << "\t" << mX << ":" << mY << " - " << edge.x << ":" << edge.y << " | " << edge.nextVId << std::endl;
    }
}

void Vertex::adde(int x, int y, int id)
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

void Graph::addv(int x, int y)
{
    auto vertex = findVertex(x, y);
    if(vertex == end()) {
        push_back({x, y});
        mSelected = &back();
    } else {
        std::cout << "Vertex: " << x << ":" << y << " does exist!" << std::endl;
        mSelected = &*vertex;
    }
}

void Graph::adde(int sx, int sy, int dx, int dy)
{
    auto svertex = findVertex(sx, sy);
    auto dvertex = findVertex(dx, dy);
    if(svertex == end() || dvertex == end() || svertex == dvertex)
        std::cout << "Vertex doesn't exist!" << std::endl;
    else {
        svertex->adde(dx, dy, std::distance(begin(), dvertex));
        dvertex->adde(sx, sy, std::distance(begin(), svertex));
    }
}

void Graph::movev(int sx, int sy, int dx, int dy)
{
    auto vertex = findVertex(sx, sy);
    if(vertex == end())
        std::cout << "Vertex doesn't exist!" << std::endl;
    else {
        for(auto &v: *this) {
            auto edge = v.findEdge(sx, sy);
            if(edge != v.end()) {
                edge->x = dx;
                edge->y = dy;
            }
        }
        vertex->move(dx, dy);
    }
}

void Graph::print() const
{
    std::cout << "Graph size: " << size() << std::endl;
    for(const auto &vertex: *this)
        vertex.print();
}

void Graph::draw(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(const auto &vertex: *this)
        vertex.draw(renderer, scale, ox, oy, r, g, b);
    if(mSelected != nullptr) {
        mSelected->draw(renderer, scale, ox, oy, 255, g, b);
    }
}

std::vector<Vertex>::iterator Graph::findVertex(int x, int y)
{
    return std::find_if(begin(), end(), [&](const Vertex &v){
        return std::sqrt(std::pow(v.mX - x, 2.f) +
                         std::pow(v.mY - y, 2.f)) < (VertexRectSize >> 1) + 1.f;
    });
}
