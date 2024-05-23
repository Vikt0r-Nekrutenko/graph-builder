#include "app.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>

#define VertexRectSize 8

static SDL_Rect bgSourceRect {0,0, 0,0};

App::App()
{
    SDL_Init(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, 0);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    mGraph.push_back({{{87,55,1}}, 79,43});
    mGraph.push_back({{{79,43,0}}, 87,55});

    SDL_Surface* image = SDL_LoadBMP("minimap.bmp");
    mBackground = SDL_CreateTextureFromSurface(mRenderer, image);
    bgSourceRect.w = image->w;
    bgSourceRect.h = image->h;
    SDL_FreeSurface(image);
}

App::~App()
{
    SDL_DestroyTexture(mBackground);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

bool App::onUpdateHandler()
{
    drawBackground(mRenderer, mBackground, mScaleCoeffitient, mXOffset, mYOffset);
    drawGraph(mRenderer, mGraph, mScaleCoeffitient, mXOffset, mYOffset);

    if(mSelected != nullptr) {
        drawSelectedVertex(mRenderer, mSelected, mScaleCoeffitient, mXOffset, mYOffset);
    }
    return isContinue;
}

void App::onClickHandler(const SDL_MouseButtonEvent &button)
{
    auto vertexIt = findExistVertex(button.x, button.y);

    switch(button.button) {
    case SDL_BUTTON_LEFT:
        mSelected = leftButtonClickHandler(vertexIt, button.x, button.y);
        break;
    case SDL_BUTTON_RIGHT:
        mSelected = rightButtonClickHandler(vertexIt, button.x, button.y);
        break;
    }

    if(mSelected != nullptr) {
        std::cout << mGraph.size() << " " << mSelected->edges.size() << std::endl;
        for(const auto &e: mSelected->edges) {
            std::cout << "\t" << mSelected->x << ":" << mSelected->y << "-" << e.x << ":" << e.y << "|" << e.nextVId << std::endl;
        }
    }
}

void App::onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion)
{
    if(button.button == SDL_BUTTON_LEFT)
        mSelected = leftButtonDragHandler(motion.x, motion.y);
}

void App::onKeyHandler(const SDL_Keysym &keysym)
{
    switch (keysym.sym) {
    case 'q': isContinue = false; break;
    case 'w': mYOffset += 5; break;
    case 's': mYOffset -= 5; break;
    case 'a': mXOffset += 5; break;
    case 'd': mXOffset -= 5; break;
    case 'z': ++mScaleCoeffitient; break;
    case 'x': --mScaleCoeffitient; break;
    case 'c':
        if(mHistory.size() > 1) {
            mSelected = nullptr;
            mHistory.pop();
            mGraph = mHistory.top();
        } break;
    }
    std::cout << mXOffset << " " << mYOffset << std::endl;
}

int App::run(int fps)
{
    SDL_Event event;
    size_t countedFrames = 0;
    float dt = 0.f, elapsedTime = 0.f, _fps = 0;
    auto t1 = std::chrono::high_resolution_clock::now();

    while(true) {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mRenderer);
        if(onUpdateHandler() == false)
            break;
        SDL_RenderPresent(mRenderer);
        ++countedFrames;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                break;
            else if(event.type == SDL_MOUSEBUTTONUP)
                onClickHandler(event.button);
            else if(event.type == SDL_KEYDOWN)
                onKeyHandler(event.key.keysym);
            else if(event.type == SDL_MOUSEMOTION)
                onDragHandler(event.button, event.motion);
        }

        dt = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1).count()) / 100.f;
        t1 = std::chrono::high_resolution_clock::now();
        elapsedTime += dt;
        if(dt < 1.f / float(fps>>1)) {
            SDL_Delay((1.f / float(fps>>1) - dt) * 100.f);
        }

        if(elapsedTime > 1.f) {
            SDL_SetWindowTitle(mWindow, ("FPS: " + std::to_string(int(_fps))).c_str());
            _fps = float(countedFrames) * 0.5f + _fps * 0.5f;
            elapsedTime -= 1.f;
            countedFrames = 0;
        }
    }
    return 0;
}

Graph::iterator App::findExistVertex(int x, int y)
{
    return std::find_if(mGraph.begin(), mGraph.end(), [&](const Vertex &v){
        return std::sqrt(std::pow(v.x * mScaleCoeffitient + mXOffset - x, 2.f) +
                         std::pow(v.y * mScaleCoeffitient + mYOffset - y, 2.f)) < (VertexRectSize >> 1) + 1.f;
    });
}

std::vector<Edge>::iterator App::findExistEdge(Vertex *source, Vertex *destination)
{
    return std::find_if(source->edges.begin(), source->edges.end(), [&](const Edge &e){
        return e.x == destination->x && e.y == destination->y;
    });
}

Vertex *App::leftButtonClickHandler(Graph::iterator vertexIt, int mouseX, int mouseY)
{
    if(vertexIt != mGraph.end()) {
        return &*vertexIt;
    } else if(vertexIt == mGraph.end() && mSelected != nullptr) {
        auto selectedIt = findExistVertex(mSelected->x, mSelected->y);
        Vertex newVertex {
                         {{mSelected->x, mSelected->y, int(std::distance(mGraph.begin(), selectedIt))}},
                         (mouseX - mXOffset) / mScaleCoeffitient,
                         (mouseY - mYOffset) / mScaleCoeffitient};
        mSelected->edges.push_back({newVertex.x, newVertex.y, int(mGraph.size())});
        mGraph.push_back(newVertex);
        mHistory.push(mGraph);
        return &mGraph.back();
    }
    return nullptr;
}

Vertex *App::leftButtonDragHandler(int mouseX, int mouseY)
{
    if(mSelected != nullptr) {
        int _x = mSelected->x;
        int _y = mSelected->y;
        mSelected->x = (mouseX - mXOffset) / mScaleCoeffitient;
        mSelected->y = (mouseY - mYOffset) / mScaleCoeffitient;
        for(auto &edge: mSelected->edges) {
            auto destVrtx = &mGraph[edge.nextVId];
            for(auto &destEdge: destVrtx->edges) {
                if(destEdge.x == _x && destEdge.y == _y) {
                    destEdge.x = (mouseX - mXOffset) / mScaleCoeffitient;
                    destEdge.y = (mouseY - mYOffset) / mScaleCoeffitient;
                }
            }
        }
    }
    return mSelected;
}

Vertex *App::rightButtonClickHandler(Graph::iterator vertexIt, int mouseX, int mouseY)
{
    if(mSelected != nullptr) {
        if(vertexIt != mGraph.end() && mSelected != &*vertexIt) {
            auto edgeIt = findExistEdge(&*vertexIt, mSelected);

            if(edgeIt == vertexIt->edges.end())
                vertexIt->edges.push_back({mSelected->x,
                                           mSelected->y,
                                           int(std::distance(mGraph.begin(), findExistVertex(mSelected->x, mSelected->y)))
                });

            edgeIt = findExistEdge(mSelected, &*vertexIt);

            if(edgeIt == mSelected->edges.end())
                mSelected->edges.push_back({vertexIt->x, vertexIt->y, int(std::distance(mGraph.begin(), vertexIt))});
            mHistory.push(mGraph);
            return mSelected;
        }
    }
    return nullptr;
}

void drawGraph(SDL_Renderer *renderer, const Graph &graph, int scaleCoeffitient, int xOffset, int yOffset)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(const auto &vertex: graph) {
        SDL_Rect rect { vertex.x * scaleCoeffitient + xOffset - (VertexRectSize >> 1),
                        vertex.y * scaleCoeffitient + yOffset - (VertexRectSize >> 1),
            VertexRectSize,
            VertexRectSize
        };
        SDL_RenderDrawRect(renderer, &rect);
        for(const auto &edge: vertex.edges) {
            SDL_RenderDrawLine(renderer,
                               vertex.x * scaleCoeffitient + xOffset,
                               vertex.y * scaleCoeffitient + yOffset,
                               edge.x * scaleCoeffitient + xOffset,
                               edge.y * scaleCoeffitient + yOffset);
        }
    }
}

void drawSelectedVertex(SDL_Renderer *renderer, const Vertex *selected, int scaleCoeffitient, int xOffset, int yOffset)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect {
        selected->x * scaleCoeffitient + xOffset - (VertexRectSize >> 1),
        selected->y * scaleCoeffitient + yOffset - (VertexRectSize >> 1),
        VertexRectSize,
        VertexRectSize
    };
    SDL_RenderDrawRect(renderer, &rect);
    for(const auto &edge: selected->edges) {
        SDL_RenderDrawLine(renderer,
                           selected->x * scaleCoeffitient + xOffset,
                           selected->y * scaleCoeffitient + yOffset,
                           edge.x * scaleCoeffitient + xOffset,
                           edge.y * scaleCoeffitient + yOffset);
    }
}

void drawBackground(SDL_Renderer *renderer, SDL_Texture *texture, int scaleCoeffitient, int xOffset, int yOffset)
{
    SDL_Rect bgDestinationRect {
        (-(VertexRectSize >> 1))+2 + xOffset,
        (-(VertexRectSize >> 1))+2 + yOffset,
        bgSourceRect.w * scaleCoeffitient,
        bgSourceRect.h * scaleCoeffitient
    };
    SDL_RenderCopy(renderer, texture, &bgSourceRect, &bgDestinationRect);
}
