#include "app.hpp"
#include "SDL2/SDL.h"

#include <chrono>
#include <iostream>
#include <ostream>
#include <string>


static SDL_Rect __bgSourceRect {0,0, 0,0};

App::App()
    : mGraph{&mScaleCoeffitient}
{
    SDL_Init(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    mRegularEdgeTexture = createThickLine(mRenderer, 0, 225, 225);
    mSelectedEdgeTexture = createThickLine(mRenderer, 255, 0, 0);

    SDL_Surface* image = SDL_LoadBMP("minimap.bmp");

    for(int i = 0; i < image->h; ++i) {
        for(int j = 0; j < image->w; ++j) {
            Uint8 *pixel = ((Uint8 *) image->pixels +
                                               i * image->pitch +
                                               j * image->format->BytesPerPixel);
            Uint8 &r = pixel[2],
                  &g = pixel[1],
                  &b = pixel[0];
            if(r < 150 ||
               g < 150 ||
               b < 150)
            {
                r = g = b = 0;
            } else {
                r = g = b = 128;
            }
        }
    }

    mBackground = SDL_CreateTextureFromSurface(mRenderer, image);
    __bgSourceRect.w = image->w;
    __bgSourceRect.h = image->h;
    SDL_FreeSurface(image);
}

App::~App()
{
    SDL_DestroyTexture(mRegularEdgeTexture);
    SDL_DestroyTexture(mSelectedEdgeTexture);
    SDL_DestroyTexture(mBackground);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

bool App::onUpdateHandler()
{
    drawBackground(mScaleCoeffitient, mXOffset, mYOffset);

    mGraph.draw(mRenderer, mRegularEdgeTexture, mSelectedEdgeTexture, mScaleCoeffitient, mXOffset, mYOffset, 255, 75, 39);

    return isContinue;
}

void App::onClickHandler(const SDL_MouseButtonEvent &button)
{
    int mx = (button.x - mXOffset) / mScaleCoeffitient;
    int my = (button.y - mYOffset) / mScaleCoeffitient;

    switch(button.button) {
    case SDL_BUTTON_LEFT:
        switch (mEditMode) {
        case EditMode::None:
            mGraph.selectVertex(mx, my);
            break;
        case EditMode::AddVertex:
            // if(!mGraph.isVertexSelected()) {
                mHistory.push(mGraph);
                mGraph.addNewVertex(mx, my);
                mGraph.selectVertex(mx, my);
            // }
            break;
        case EditMode::AddEdge:
            if(mGraph.isVertexSelected()) {
                mHistory.push(mGraph);
                // mGraph.addNewVertexToSelected(mx, my);
                // mGraph.selectVertex(mx, my);
                mGraph.addNewEdgeToSelected(mx, my);
            }
            break;
        }
        break;
    case SDL_BUTTON_RIGHT:
        // if(mGraph.isVertexSelected()) {
        //     mHistory.push(mGraph);
        //     mGraph.addNewVertexToSelected(mx, my);
        // }
        // mGraph.selectVertex(mx, my);


        break;
    }
}

void App::onDragHandler(const SDL_MouseButtonEvent &button, const SDL_MouseMotionEvent &motion)
{
    int mx = (motion.x - mXOffset) / mScaleCoeffitient;
    int my = (motion.y - mYOffset) / mScaleCoeffitient;

    if(button.button == SDL_BUTTON_LEFT && mGraph.isVertexSelected())
        mGraph.moveSelected(mx, my);
}

void App::onKeyHandler(const SDL_Keysym &keysym)
{
    switch (keysym.scancode) {
    case SDL_SCANCODE_ESCAPE: isContinue = false; break;
    case SDL_SCANCODE_W: mYOffset += 5; break;
    case SDL_SCANCODE_S: mYOffset -= 5; break;
    case SDL_SCANCODE_A: mXOffset += 5; break;
    case SDL_SCANCODE_D: mXOffset -= 5; break;
    case SDL_SCANCODE_X: mScaleCoeffitient -= mScaleCoeffitient > 1 ? 1 : 0 ; break;
    case SDL_SCANCODE_Z:
        if(keysym.mod == KMOD_LCTRL) {
            if(!mHistory.empty()) {
                mGraph = mHistory.top();
                mGraph.deselectVertex();
                mHistory.pop();
            }
        } else {
            ++mScaleCoeffitient;
        }
        break;
    case SDL_SCANCODE_UNKNOWN:
    case SDL_SCANCODE_B:
    case SDL_SCANCODE_C:
    case SDL_SCANCODE_E:
        mEditMode = mEditMode == EditMode::AddEdge ? EditMode::None : EditMode::AddEdge;
        break;
    case SDL_SCANCODE_F:
    case SDL_SCANCODE_G:
    case SDL_SCANCODE_H:
    case SDL_SCANCODE_I:
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_L:
    case SDL_SCANCODE_M:
    case SDL_SCANCODE_N:
    case SDL_SCANCODE_O:
    case SDL_SCANCODE_P:
    case SDL_SCANCODE_Q:
    case SDL_SCANCODE_R:
    case SDL_SCANCODE_T:
    case SDL_SCANCODE_U:
    case SDL_SCANCODE_V:
        mEditMode = mEditMode == EditMode::AddVertex ? EditMode::None : EditMode::AddVertex;
        break;
    case SDL_SCANCODE_Y:
    case SDL_SCANCODE_1:
    case SDL_SCANCODE_2:
    case SDL_SCANCODE_3:
    case SDL_SCANCODE_4:
    case SDL_SCANCODE_5:
    case SDL_SCANCODE_6:
    case SDL_SCANCODE_7:
    case SDL_SCANCODE_8:
    case SDL_SCANCODE_9:
    case SDL_SCANCODE_0:
    case SDL_SCANCODE_RETURN:
    case SDL_SCANCODE_BACKSPACE:
    case SDL_SCANCODE_TAB:
    case SDL_SCANCODE_SPACE:
    case SDL_SCANCODE_MINUS:
    case SDL_SCANCODE_EQUALS:
    case SDL_SCANCODE_LEFTBRACKET:
    case SDL_SCANCODE_RIGHTBRACKET:
    case SDL_SCANCODE_BACKSLASH:
    case SDL_SCANCODE_NONUSHASH:
    case SDL_SCANCODE_SEMICOLON:
    case SDL_SCANCODE_APOSTROPHE:
    case SDL_SCANCODE_GRAVE:
    case SDL_SCANCODE_COMMA:
    case SDL_SCANCODE_PERIOD:
    case SDL_SCANCODE_SLASH:
    case SDL_SCANCODE_CAPSLOCK:
    case SDL_SCANCODE_F1:
    case SDL_SCANCODE_F2:
    case SDL_SCANCODE_F3:
    case SDL_SCANCODE_F4:
    case SDL_SCANCODE_F5:
    case SDL_SCANCODE_F6:
    case SDL_SCANCODE_F7:
    case SDL_SCANCODE_F8:
    case SDL_SCANCODE_F9:
    case SDL_SCANCODE_F10:
    case SDL_SCANCODE_F11:
    case SDL_SCANCODE_F12:
    case SDL_SCANCODE_PRINTSCREEN:
    case SDL_SCANCODE_SCROLLLOCK:
    case SDL_SCANCODE_PAUSE:
    case SDL_SCANCODE_INSERT:
    case SDL_SCANCODE_HOME:
    case SDL_SCANCODE_PAGEUP:
    case SDL_SCANCODE_DELETE:
    case SDL_SCANCODE_END:
    case SDL_SCANCODE_PAGEDOWN:
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_NUMLOCKCLEAR:
    case SDL_SCANCODE_KP_DIVIDE:
    case SDL_SCANCODE_KP_MULTIPLY:
    case SDL_SCANCODE_KP_MINUS:
    case SDL_SCANCODE_KP_PLUS:
    case SDL_SCANCODE_KP_ENTER:
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_KP_6:
    case SDL_SCANCODE_KP_7:
    case SDL_SCANCODE_KP_8:
    case SDL_SCANCODE_KP_9:
    case SDL_SCANCODE_KP_0:
    case SDL_SCANCODE_KP_PERIOD:
    case SDL_SCANCODE_NONUSBACKSLASH:
    case SDL_SCANCODE_APPLICATION:
    case SDL_SCANCODE_POWER:
    case SDL_SCANCODE_KP_EQUALS:
    case SDL_SCANCODE_F13:
    case SDL_SCANCODE_F14:
    case SDL_SCANCODE_F15:
    case SDL_SCANCODE_F16:
    case SDL_SCANCODE_F17:
    case SDL_SCANCODE_F18:
    case SDL_SCANCODE_F19:
    case SDL_SCANCODE_F20:
    case SDL_SCANCODE_F21:
    case SDL_SCANCODE_F22:
    case SDL_SCANCODE_F23:
    case SDL_SCANCODE_F24:
    case SDL_SCANCODE_EXECUTE:
    case SDL_SCANCODE_HELP:
    case SDL_SCANCODE_MENU:
    case SDL_SCANCODE_SELECT:
    case SDL_SCANCODE_STOP:
    case SDL_SCANCODE_AGAIN:
    case SDL_SCANCODE_UNDO:
    case SDL_SCANCODE_CUT:
    case SDL_SCANCODE_COPY:
    case SDL_SCANCODE_PASTE:
    case SDL_SCANCODE_FIND:
    case SDL_SCANCODE_MUTE:
    case SDL_SCANCODE_VOLUMEUP:
    case SDL_SCANCODE_VOLUMEDOWN:
    case SDL_SCANCODE_KP_COMMA:
    case SDL_SCANCODE_KP_EQUALSAS400:
    case SDL_SCANCODE_INTERNATIONAL1:
    case SDL_SCANCODE_INTERNATIONAL2:
    case SDL_SCANCODE_INTERNATIONAL3:
    case SDL_SCANCODE_INTERNATIONAL4:
    case SDL_SCANCODE_INTERNATIONAL5:
    case SDL_SCANCODE_INTERNATIONAL6:
    case SDL_SCANCODE_INTERNATIONAL7:
    case SDL_SCANCODE_INTERNATIONAL8:
    case SDL_SCANCODE_INTERNATIONAL9:
    case SDL_SCANCODE_LANG1:
    case SDL_SCANCODE_LANG2:
    case SDL_SCANCODE_LANG3:
    case SDL_SCANCODE_LANG4:
    case SDL_SCANCODE_LANG5:
    case SDL_SCANCODE_LANG6:
    case SDL_SCANCODE_LANG7:
    case SDL_SCANCODE_LANG8:
    case SDL_SCANCODE_LANG9:
    case SDL_SCANCODE_ALTERASE:
    case SDL_SCANCODE_SYSREQ:
    case SDL_SCANCODE_CANCEL:
    case SDL_SCANCODE_CLEAR:
    case SDL_SCANCODE_PRIOR:
    case SDL_SCANCODE_RETURN2:
    case SDL_SCANCODE_SEPARATOR:
    case SDL_SCANCODE_OUT:
    case SDL_SCANCODE_OPER:
    case SDL_SCANCODE_CLEARAGAIN:
    case SDL_SCANCODE_CRSEL:
    case SDL_SCANCODE_EXSEL:
    case SDL_SCANCODE_KP_00:
    case SDL_SCANCODE_KP_000:
    case SDL_SCANCODE_THOUSANDSSEPARATOR:
    case SDL_SCANCODE_DECIMALSEPARATOR:
    case SDL_SCANCODE_CURRENCYUNIT:
    case SDL_SCANCODE_CURRENCYSUBUNIT:
    case SDL_SCANCODE_KP_LEFTPAREN:
    case SDL_SCANCODE_KP_RIGHTPAREN:
    case SDL_SCANCODE_KP_LEFTBRACE:
    case SDL_SCANCODE_KP_RIGHTBRACE:
    case SDL_SCANCODE_KP_TAB:
    case SDL_SCANCODE_KP_BACKSPACE:
    case SDL_SCANCODE_KP_A:
    case SDL_SCANCODE_KP_B:
    case SDL_SCANCODE_KP_C:
    case SDL_SCANCODE_KP_D:
    case SDL_SCANCODE_KP_E:
    case SDL_SCANCODE_KP_F:
    case SDL_SCANCODE_KP_XOR:
    case SDL_SCANCODE_KP_POWER:
    case SDL_SCANCODE_KP_PERCENT:
    case SDL_SCANCODE_KP_LESS:
    case SDL_SCANCODE_KP_GREATER:
    case SDL_SCANCODE_KP_AMPERSAND:
    case SDL_SCANCODE_KP_DBLAMPERSAND:
    case SDL_SCANCODE_KP_VERTICALBAR:
    case SDL_SCANCODE_KP_DBLVERTICALBAR:
    case SDL_SCANCODE_KP_COLON:
    case SDL_SCANCODE_KP_HASH:
    case SDL_SCANCODE_KP_SPACE:
    case SDL_SCANCODE_KP_AT:
    case SDL_SCANCODE_KP_EXCLAM:
    case SDL_SCANCODE_KP_MEMSTORE:
    case SDL_SCANCODE_KP_MEMRECALL:
    case SDL_SCANCODE_KP_MEMCLEAR:
    case SDL_SCANCODE_KP_MEMADD:
    case SDL_SCANCODE_KP_MEMSUBTRACT:
    case SDL_SCANCODE_KP_MEMMULTIPLY:
    case SDL_SCANCODE_KP_MEMDIVIDE:
    case SDL_SCANCODE_KP_PLUSMINUS:
    case SDL_SCANCODE_KP_CLEAR:
    case SDL_SCANCODE_KP_CLEARENTRY:
    case SDL_SCANCODE_KP_BINARY:
    case SDL_SCANCODE_KP_OCTAL:
    case SDL_SCANCODE_KP_DECIMAL:
    case SDL_SCANCODE_KP_HEXADECIMAL:
    case SDL_SCANCODE_LCTRL:
    case SDL_SCANCODE_LSHIFT:
    case SDL_SCANCODE_LALT:
    case SDL_SCANCODE_LGUI:
    case SDL_SCANCODE_RCTRL:
    case SDL_SCANCODE_RSHIFT:
    case SDL_SCANCODE_RALT:
    case SDL_SCANCODE_RGUI:
    case SDL_SCANCODE_MODE:
    case SDL_SCANCODE_AUDIONEXT:
    case SDL_SCANCODE_AUDIOPREV:
    case SDL_SCANCODE_AUDIOSTOP:
    case SDL_SCANCODE_AUDIOPLAY:
    case SDL_SCANCODE_AUDIOMUTE:
    case SDL_SCANCODE_MEDIASELECT:
    case SDL_SCANCODE_WWW:
    case SDL_SCANCODE_MAIL:
    case SDL_SCANCODE_CALCULATOR:
    case SDL_SCANCODE_COMPUTER:
    case SDL_SCANCODE_AC_SEARCH:
    case SDL_SCANCODE_AC_HOME:
    case SDL_SCANCODE_AC_BACK:
    case SDL_SCANCODE_AC_FORWARD:
    case SDL_SCANCODE_AC_STOP:
    case SDL_SCANCODE_AC_REFRESH:
    case SDL_SCANCODE_AC_BOOKMARKS:
    case SDL_SCANCODE_BRIGHTNESSDOWN:
    case SDL_SCANCODE_BRIGHTNESSUP:
    case SDL_SCANCODE_DISPLAYSWITCH:
    case SDL_SCANCODE_KBDILLUMTOGGLE:
    case SDL_SCANCODE_KBDILLUMDOWN:
    case SDL_SCANCODE_KBDILLUMUP:
    case SDL_SCANCODE_EJECT:
    case SDL_SCANCODE_SLEEP:
    case SDL_SCANCODE_APP1:
    case SDL_SCANCODE_APP2:
    case SDL_SCANCODE_AUDIOREWIND:
    case SDL_SCANCODE_AUDIOFASTFORWARD:
    case SDL_SCANCODE_SOFTLEFT:
    case SDL_SCANCODE_SOFTRIGHT:
    case SDL_SCANCODE_CALL:
    case SDL_SCANCODE_ENDCALL:
    case SDL_NUM_SCANCODES:
        break;
    }
}

void App::drawBackground(int scaleCoeffitient, int xOffset, int yOffset)
{
    SDL_Rect bgDestinationRect {
        (-(VertexRectSize >> 1))+2 + xOffset,
        (-(VertexRectSize >> 1))+2 + yOffset,
        __bgSourceRect.w * scaleCoeffitient,
        __bgSourceRect.h * scaleCoeffitient
    };
    SDL_RenderCopy(mRenderer, mBackground, &__bgSourceRect, &bgDestinationRect);
}

int App::run(int fps)
{
    SDL_Event event;
    size_t countedFrames = 0;
    float dt = 0.f, elapsedTime = 0.f, _fps = 0;
    auto t1 = std::chrono::high_resolution_clock::now();

    while(isContinue) {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mRenderer);
        onUpdateHandler();
        SDL_RenderPresent(mRenderer);
        ++countedFrames;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                isContinue = false;
            else if(event.type == SDL_MOUSEBUTTONUP)
                onClickHandler(event.button);
            else if(event.type == SDL_KEYDOWN) {
                onKeyHandler(event.key.keysym);
            }
            else if(event.type == SDL_MOUSEMOTION) {
                onDragHandler(event.button, event.motion);
            }
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
