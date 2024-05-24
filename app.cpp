#include "app.hpp"
#include <chrono>

#include <iostream>
#include <string>


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
    drawBackground(mScaleCoeffitient, mXOffset, mYOffset);

    mGraph.draw(mRenderer, mScaleCoeffitient, mXOffset, mYOffset, 0, 0, 0);

    return isContinue;
}

void App::onClickHandler(const SDL_MouseButtonEvent &button)
{
    int mx = (button.x - mXOffset) / mScaleCoeffitient;
    int my = (button.y - mYOffset) / mScaleCoeffitient;

    switch(button.button) {
    case SDL_BUTTON_LEFT:
        mGraph.selectVertex(mx, my);
        if(!mGraph.isVertexSelected()) {
            mHistory.push(mGraph);
            mGraph.addNewVertex(mx, my);
            mGraph.selectVertex(mx, my);
        }
        break;
    case SDL_BUTTON_RIGHT:
        if(mGraph.isVertexSelected()) {
            mHistory.push(mGraph);
            mGraph.addNewVertexToSelected(mx, my);
        }
        mGraph.selectVertex(mx, my);

        break;
    }
}

void App::onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion)
{
    int mx = (motion.x - mXOffset) / mScaleCoeffitient;
    int my = (motion.y - mYOffset) / mScaleCoeffitient;

    if(button.button == SDL_BUTTON_LEFT)
        if(mGraph.isVertexSelected())
            mGraph.moveSelected(mx, my);
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
            mGraph = mHistory.top();
            mGraph.deselectVertex();
            mHistory.pop();
        } break;
    }
    std::cout << mXOffset << " " << mYOffset << std::endl;
}

void App::drawBackground(int scaleCoeffitient, int xOffset, int yOffset)
{
    SDL_Rect bgDestinationRect {
        (-(VertexRectSize >> 1))+2 + xOffset,
        (-(VertexRectSize >> 1))+2 + yOffset,
        bgSourceRect.w * scaleCoeffitient,
        bgSourceRect.h * scaleCoeffitient
    };
    SDL_RenderCopy(mRenderer, mBackground, &bgSourceRect, &bgDestinationRect);
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
