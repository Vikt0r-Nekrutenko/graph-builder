#include "app.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

static SDL_Rect bgSourceRect {0,0, 0,0};

App::App()
{
    SDL_Init(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, 0);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* image = SDL_LoadBMP("minimap.bmp");
    mBackground = SDL_CreateTextureFromSurface(mRenderer, image);
    bgSourceRect.w = image->w;
    bgSourceRect.h = image->h;
    SDL_FreeSurface(image);

    // std::ifstream edgesFile("edges.txt");
    // if(edgesFile.is_open()) {
    //     while(edgesFile.eof() == false) {
    //         int sx = -1, sy = -1, dx = -1, dy = -1, id = -1;
    //         edgesFile >> sx >> sy >> dx >> dy >> id;
    //         if(sx == -1 || sy == -1 || dx == -1 || dy == -1 || id == -1)
    //             continue;
    //         auto vertex = findExistVertex(sx, sy);
    //         if(vertex == mGraph.end())
    //             mGraph.push_back({{{dx, dy, id}}, sx, sy});
    //         else
    //             vertex->edges.push_back({dx, dy, id});
    //     }
    //     edgesFile.close();
    // }
}

App::~App()
{
    SDL_DestroyTexture(mBackground);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();

    // std::ofstream edgesFile("edges.txt");
    // for(const auto &vertex: mGraph)
    //     for(const auto &edge: vertex.edges)
    //         edgesFile << vertex.x << " " << vertex.y << " " << edge.x << " " << edge.y << " " << edge.nextVId << std::endl;
    // edgesFile.close();
}

bool App::onUpdateHandler()
{
    drawBackground(mRenderer, mBackground, mScaleCoeffitient, mXOffset, mYOffset);

    graph.draw(mRenderer, mScaleCoeffitient, mXOffset, mYOffset, 0, 0, 0);

    return isContinue;
}

void App::onClickHandler(const SDL_MouseButtonEvent &button)
{
    int mx = (button.x - mXOffset) / mScaleCoeffitient;
    int my = (button.y - mYOffset) / mScaleCoeffitient;

    switch(button.button) {
    case SDL_BUTTON_LEFT:
        graph.selectv(mx, my);
        if(!graph.isVertexSelected()) {
            mHistory.push(graph);
            graph.addv(mx, my);
            graph.selectv(mx, my);
        }
        break;
    case SDL_BUTTON_RIGHT:
        if(graph.isVertexSelected()) {
            mHistory.push(graph);
            graph.sadd(mx, my);
        }
        graph.selectv(mx, my);

        break;
    }
}

void App::onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion)
{
    int mx = (motion.x - mXOffset) / mScaleCoeffitient;
    int my = (motion.y - mYOffset) / mScaleCoeffitient;

    if(button.button == SDL_BUTTON_LEFT)
        if(graph.isVertexSelected())
            graph.smove(mx, my);
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
        if(!mHistory.empty()) {
            graph = mHistory.top();
            graph.deselectv();
            mHistory.pop();
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
