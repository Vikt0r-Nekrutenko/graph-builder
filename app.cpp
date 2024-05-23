#include "app.hpp"
#include <chrono>
#include <string>


App::App()
{
    SDL_Init(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

App::~App()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

bool App::onUpdateHandler()
{
    return isContinue;
}

void App::onClickHandler(const SDL_MouseButtonEvent &button)
{

}

void App::onKeyHandler(const SDL_Keysym &keysym)
{
    switch (keysym.sym) {
    case 'q':
        isContinue = false;
        break;
    default:
        break;
    }
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
