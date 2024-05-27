#include "app.hpp"
// #include "SDL2/SDL_timer.h"
// #include "rasterfont.hpp"
// #include <chrono>
// #include <string>

int WinMain()
{
    return App().run(30);
    // bool isContinue = true;
    // SDL_Event event;
    // size_t countedFrames = 0;
    // float dt = 0.f, elapsedTime = 0.f, _fps = 0;
    // auto t1 = std::chrono::high_resolution_clock::now();

    // SDL_Window *mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    // SDL_Renderer *mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    // RasterFont rf(mRenderer, 255, 255, 255);

    // int scale = 1;

    // while(isContinue) {
    //     SDL_SetRenderDrawColor(mRenderer, 128, 128, 128, 255);
    //     SDL_RenderClear(mRenderer);

    //     draw(mRenderer, rf, 0, 0, "%f.2%s", 24.780, "Hello_");
    //     draw(mRenderer, rf, 0, rf.H, "%s", "_world!");

    //     SDL_RenderPresent(mRenderer);
    //     ++countedFrames;

    //     while(SDL_PollEvent(&event)) {
    //         if(event.type == SDL_QUIT) {
    //             isContinue = false;
    //         } else if(event.type == SDL_MOUSEBUTTONUP) {
    //         } else if(event.type == SDL_KEYDOWN) {
    //             if(event.key.keysym.sym == 'q')
    //                 isContinue = false;
    //             if(event.key.keysym.sym == 'z')
    //                 rf.scalePlus();
    //             if(event.key.keysym.sym == 'x')
    //                 rf.scaleMinus();
    //         } else if(event.type == SDL_MOUSEMOTION) {
    //         }
    //     }

    //     dt = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1).count()) / 100.f;
    //     t1 = std::chrono::high_resolution_clock::now();
    //     elapsedTime += dt;
    //     if(dt < 1.f / float(15)) {
    //         SDL_Delay((1.f / float(15) - dt) * 100.f);
    //     }

    //     if(elapsedTime > 1.f) {
    //         SDL_SetWindowTitle(mWindow, ("FPS: " + std::to_string(int(_fps))).c_str());
    //         _fps = float(countedFrames) * 0.5f + _fps * 0.5f;
    //         elapsedTime -= 1.f;
    //         countedFrames = 0;
    //     }
    // }
    // return 0;
}
