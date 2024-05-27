#include "graph.hpp"
#include "SDL2/SDL_render.h"

#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>


Graph::Graph(int *scale)
    : mScalePtr{scale}
{
    std::ifstream edgesFile("edges.txt");
    struct tmp { int sx, sy, dx, dy, id; };
    std::vector<tmp> edges;
    if(edgesFile.is_open()) {
        while(edgesFile.eof() == false) {
            int sx = -1, sy = -1, dx = -1, dy = -1, id = -1;
            edgesFile >> sx >> sy >> dx >> dy >> id;
            if(sx == -1 || sy == -1 || dx == -1 || dy == -1 || id == -1){
                continue;
            }
            edges.push_back({sx, sy, dx, dy, id});
            auto vertex = std::find_if(begin(), end(), [&](const Vertex &v){ return v.mX == sx && v.mY == sy; });
            if(vertex == end())
                push_back({sx, sy});
        }
        edgesFile.close();
    }

    for(const auto &edge: edges) {
        auto svertex = std::find_if(begin(), end(), [&](const Vertex &v){ return v.mX == edge.sx && v.mY == edge.sy; });
        auto dvertex = std::find_if(begin(), end(), [&](const Vertex &v){ return v.mX == edge.dx && v.mY == edge.dy; });
        svertex->push_back({edge.dx, edge.dy, int(std::distance(begin(), dvertex))});
    }
}

Graph::~Graph()
{
    std::ofstream edgesFile("edges.txt");
    for(const auto &vertex: *this)
        for(const auto &edge: vertex)
            edgesFile << vertex.mX << " " << vertex.mY << " " << edge.x << " " << edge.y << " " << edge.nextVId << std::endl;
    edgesFile.close();
}

void Graph::addNewVertex(int x, int y)
{
    auto vertex = findVertex(x, y);
    if(vertex == end()) {
        push_back({x, y});
    } else {
        std::cout << "Vertex: " << x << ":" << y << " does exist!" << std::endl;
    }
}

void Graph::addNewEdge(int sx, int sy, int dx, int dy)
{
    auto svertex = findVertex(sx, sy);
    auto dvertex = findVertex(dx, dy);
    if(svertex == end() || dvertex == end() || svertex == dvertex)
        std::cout << "Vertex doesn't exist!" << std::endl;
    else {
        svertex->add(dvertex->mX, dvertex->mY, std::distance(begin(), dvertex));
        dvertex->add(svertex->mX, svertex->mY, std::distance(begin(), svertex));
    }
}

void Graph::moveVertex(int sx, int sy, int dx, int dy)
{
    auto vertex = findVertex(sx, sy);
    if(vertex == end())
        std::cout << "Vertex doesn't exist!" << std::endl;
    else {
        for(auto &v: *this) {
            for(auto &edge: v) {
                if(edge.x == sx && edge.y == sy) {
                    edge.x = dx;
                    edge.y = dy;
                }
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

void Graph::draw(SDL_Renderer *renderer, SDL_Texture *edge, SDL_Texture *selected, int scale, int ox, int oy, int r, int g, int b) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(const auto &vertex: *this)
        vertex.drawEdges(renderer, edge, scale, ox, oy);
    for(const auto &vertex: *this)
        vertex.drawVertex(renderer, scale, ox, oy, r, g, b);
    if(mSelected != nullptr)
        mSelected->draw(renderer, selected, scale, ox, oy, 255, 0, 0);
}

void Graph::selectVertex(int x, int y)
{
    auto vertex = findVertex(x, y);
    if(vertex != end()) {
        mSelected = &*vertex;
        mSelected->print();
    }
    else
        mSelected = nullptr;
}

void Graph::deselectVertex()
{
    mSelected = nullptr;
}

void Graph::addNewVertexToSelected(int dx, int dy)
{
    auto vertex = findVertex(dx, dy);
    if(vertex != end())
        addNewEdge(mSelected->mX, mSelected->mY, dx, dy);
    else {
        addNewVertex(dx, dy);
        addNewEdge(mSelected->mX, mSelected->mY, dx, dy);
    }
}

void Graph::addNewEdgeToSelected(int dx, int dy)
{
    addNewEdge(mSelected->mX, mSelected->mY, dx, dy);
}

void Graph::moveSelected(int dx, int dy)
{
    for(auto &v: *this) {
        for(auto &edge: v) {
            if(edge.x == mSelected->mX && edge.y == mSelected->mY) {
                edge.x = dx;
                edge.y = dy;
            }
        }
    }
    mSelected->move(dx, dy);
}

std::vector<Vertex>::iterator Graph::findVertex(int x, int y)
{
    return std::find_if(begin(), end(), [&](const Vertex &v){
        return std::sqrt(std::pow(v.mX - x, 2.f) +
                         std::pow(v.mY - y, 2.f)) < float(VertexRectSize) / float(*mScalePtr);
    });
}
