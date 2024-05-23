#include "app.hpp"
#include <chrono>
#include <iostream>
#include <string>

#define VertexRectSize 8

App::App()
{
    SDL_Init(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, 0);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    mGraph.push_back({{{20,20,1}}, 10,10});
    mGraph.push_back({{{10,10,0}}, 30,30});
    mSelected = &mGraph.back();
}

App::~App()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

bool App::onUpdateHandler()
{
    drawGraph(mRenderer, mGraph, mScaleCoeffitient, mXOffset, mYOffset);

    if(mSelected != nullptr) {
        drawSelectedVertex(mRenderer, mSelected, mScaleCoeffitient, mXOffset, mYOffset);
    }
    return isContinue;
}

void App::onClickHandler(const SDL_MouseButtonEvent &button)
{

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
            else if(event.type == SDL_KEYUP)
                onKeyHandler(event.key.keysym);
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

void drawGraph(SDL_Renderer *renderer, const Graph &graph, int scaleCoeffitient, int xOffset, int yOffset)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
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
