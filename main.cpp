#include <algorithm>
#include <iostream>
#include "SDL2/SDL_timer.h"
#include "app.hpp"
#include "rasterfont.hpp"
#include <chrono>

// #define SYM_W 10
// #define SYM_H 20
// #define MAP_S 13

// SDL_Texture *txtr;

// SDL_Surface *surf;

// struct {
//     Uint8 r = 255, g = 255, b = 255;
// } textColor;

// void loadFont(SDL_Renderer *renderer)
// {
//     surf = SDL_LoadBMP("raster_font.bmp");
//     for(int y = 0; y < surf->h; ++y) {
//         for(int x = 0; x < surf->w; ++x) {
//             Uint8 *pixel = ((Uint8 *) surf->pixels +
//                             y * surf->pitch +
//                             x * surf->format->BytesPerPixel);
//             Uint8 &a = pixel[3],
//                 &r = pixel[2],
//                 &g = pixel[1],
//                 &b = pixel[0];
//             if(r < 150 || g < 150 || b < 150) {
//                 a = 0;
//             } else {
//                 r = g = b = a = 0xff;
//             }
//         }
//     }
//     txtr = SDL_CreateTextureFromSurface(renderer, surf);
// }

// void changeColor(Uint8 r, Uint8 g, Uint8 b)
// {
//     if(textColor.r == r && textColor.g == g && textColor.b == b)
//         return;
//     for(int _y = 0; _y < surf->h; ++_y) {
//         for(int _x = 0; _x < surf->w; ++_x) {
//             Uint8 *pixel = ((Uint8 *) surf->pixels +
//                             _y * surf->pitch +
//                             _x * surf->format->BytesPerPixel);
//             Uint8 &_a = pixel[3],
//                 &_r = pixel[2],
//                 &_g = pixel[1],
//                 &_b = pixel[0];
//             if(_a == 0xff) {
//                 _r = r; _g = g; _b = b;
//             }
//         }
//     }
//     textColor.r = r; textColor.g = g; textColor.b = b;
//     SDL_UpdateTexture(txtr, NULL, surf->pixels, surf->pitch);
// }

// void drawSym(SDL_Renderer *renderer, unsigned char sym, int x, int y, int r, int g, int b)
// {
//     SDL_Rect sr {
//         SYM_W * (sym % MAP_S),
//         3 + SYM_H * (sym / MAP_S + 1),
//         SYM_W,
//         SYM_H
//     };
//     SDL_Rect dr { x, y, SYM_W / 2, SYM_H / 2 };
//     changeColor(r,g,b);
//     SDL_RenderCopy(renderer, txtr, &sr, &dr);
// }

// void draw(SDL_Renderer *renderer, int x, int y, const char *format, ...)
// {
//     va_list vl;
//     va_start(vl, format);

//     int it = 0;

//     for (const char *ptr = format; *ptr != '\0'; ptr++)
//     {
//         if (*ptr == '%')
//         {
//             ptr++;
//             if (*ptr == 's')
//             {
//                 const char *str = va_arg(vl, const char *);
//                 const char *ptr = str;
//                 while(*ptr != 0) {
//                     drawSym(renderer, *ptr++, x + it, y, textColor.r, textColor.g, textColor.b);
//                     it += 10;
//                 }
//             }
//             else if(*ptr == 'd')
//             {
//                 const int n = va_arg(vl, int);
//                 const std::string ns = std::to_string(n);
//                 const char *ptr = ns.data();
//                 while(*ptr != 0) {
//                     drawSym(renderer, *ptr++, x + it, y, textColor.r, textColor.g, textColor.b);
//                     it += 10;
//                 }
//             }
//             else if(*ptr == 'f')
//             {
//                 const double n = va_arg(vl, double);
//                 std::string ns = std::to_string(n);
//                 if(*++ptr == '.') {
//                     int symAfterPoint = *++ptr - '0' + 1;
//                     auto itr = std::find_if(ns.begin(), ns.end(), [&](const char &s){ return s == '.'; });
//                     ns = std::string(ns.begin(), itr + symAfterPoint);
//                 }
//                 const char *ptr = ns.data();
//                 while(*ptr != 0) {
//                     drawSym(renderer, *ptr++, x + it, y, textColor.r, textColor.g, textColor.b);
//                     it += 10;
//                 }
//             }
//         }
//         else
//         {
//             drawSym(renderer, *ptr, x + it, y, textColor.r, textColor.g, textColor.b);
//             it += 10;
//         }
//     }
//     va_end(vl);
// }

int WinMain()
{
    // return App().run(30);

    bool isContinue = true;
    SDL_Event event;
    size_t countedFrames = 0;
    float dt = 0.f, elapsedTime = 0.f, _fps = 0;
    auto t1 = std::chrono::high_resolution_clock::now();

    SDL_Window *mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    RasterFont rf(mRenderer, 255, 255, 255);

    // loadFont(mRenderer);

    while(isContinue) {
        SDL_SetRenderDrawColor(mRenderer, 128, 128, 128, 255);
        SDL_RenderClear(mRenderer);

        // int x = 0;
        // drawSym(mRenderer, 'H', x,     0, 255, 255, 255);
        // drawSym(mRenderer, 'e', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'l', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'l', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'o', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, '_', x+=10, 0, 255,   0,   0);
        // drawSym(mRenderer, 'W', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'o', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'r', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'l', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, 'd', x+=10, 0, 255, 255, 255);
        // drawSym(mRenderer, '!', x+=10, 0, 255, 255, 255);
        draw(mRenderer, rf, 0, 21, "%f.2%s", 24.780, "Hello_world!");


        SDL_RenderPresent(mRenderer);
        ++countedFrames;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                isContinue = false;
            } else if(event.type == SDL_MOUSEBUTTONUP) {
            } else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == 'q')
                    isContinue = false;
            } else if(event.type == SDL_MOUSEMOTION) {
            }
        }

        dt = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1).count()) / 100.f;
        t1 = std::chrono::high_resolution_clock::now();
        elapsedTime += dt;
        if(dt < 1.f / float(15)) {
            SDL_Delay((1.f / float(15) - dt) * 100.f);
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
