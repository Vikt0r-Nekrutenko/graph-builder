#include "vertex.hpp"
#include "SDL2/SDL_render.h"

#include <algorithm>
#include <iostream>
#include <cmath>

SDL_Texture *createThickLine(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b)
{
    // auto t1 = std::chrono::high_resolution_clock::now();
    SDL_Surface* lineSurface = SDL_CreateRGBSurface(0, 1, 1, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    Uint32 color = SDL_MapRGB(lineSurface->format, r, g, b);

    for (int x = 0; x < lineSurface->w; ++x) {
        ((Uint32*)lineSurface->pixels)[x] = color;
    }

    // std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t1).count() << std::endl;
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, lineSurface);
    SDL_FreeSurface(lineSurface);
    return texture;
}

void drawThickLine(SDL_Renderer *renderer, SDL_Texture *line, int x1, int y1, int x2, int y2, int scale, int xoff, int yoff, int thickness)
{
    SDL_Rect dr {x1 * scale + xoff,
                 y1 * scale + yoff,
                 int(std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2)) * scale),
                 thickness};
    SDL_Point c{0,0};
    int _x = (x2 * scale + xoff);
    int _y = (y2 * scale + yoff);
    float angle = (std::atan2<float>(-(dr.y - _y), -(dr.x - _x))) * (180.f / 3.14f);
    SDL_RenderCopyEx(renderer, line, NULL, &dr, angle, &c, SDL_RendererFlip::SDL_FLIP_NONE);
}


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

void Vertex::draw(SDL_Renderer *renderer, SDL_Texture *line, int scale, int ox, int oy, int r, int g, int b) const
{
    SDL_Rect rect {
        mX * scale + ox - (VertexRectSize >> 1),
        mY * scale + oy - (VertexRectSize >> 1),
        VertexRectSize,
        VertexRectSize
    };
    for(const auto &edge: *this) {
        drawThickLine(renderer, line, mX, mY, edge.x, edge.y, scale, ox, oy, 3);
    }
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
}

void Vertex::drawEdges(SDL_Renderer *renderer, SDL_Texture *line, int scale, int ox, int oy) const
{
    for(const auto &edge: *this)
        drawThickLine(renderer, line, mX, mY, edge.x, edge.y, scale, ox, oy, 3);
}

void Vertex::drawVertex(SDL_Renderer *renderer, int scale, int ox, int oy, int r, int g, int b) const
{
    SDL_Rect rect {
        mX * scale + ox - (VertexRectSize >> 1),
        mY * scale + oy - (VertexRectSize >> 1),
        VertexRectSize,
        VertexRectSize
    };
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
}

std::vector<Edge>::iterator Vertex::findEdge(int x, int y)
{
    return std::find_if(begin(), end(), [&](const Edge &e){ return e.x == x && e.y == y; });
}
