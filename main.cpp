#include <iostream>
#include <chrono>

#include "SDL2/SDL.h"

int WinMain()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    bool isContinue = true;
    size_t countedFrames = 0;
    float dt = 0.f, elapsedTime = 0.f, fps = 0.f;
    auto t1 = std::chrono::high_resolution_clock::now();

    while(isContinue) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == 'q'))
                isContinue = false;
        }
        SDL_RenderPresent(renderer);
        ++countedFrames;

        dt = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1).count()) / 100.f;
        t1 = std::chrono::high_resolution_clock::now();
        elapsedTime += dt;
        if(dt < 1.f / 15.f) {
            SDL_Delay((1.f / 15.f - dt) * 100.f);
        }

        if(elapsedTime > 1.f) {
            SDL_SetWindowTitle(window, ("FPS: " + std::to_string(int(fps))).c_str());
            fps = float(countedFrames) * 0.5f + fps * 0.5f;
            elapsedTime -= 1.f;
            countedFrames = 0;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
