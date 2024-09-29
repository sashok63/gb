#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "common.hpp"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCALE 5

class SDL2 {
public:
    SDL2(const std::string &title);
    ~SDL2();

    bool init();
    void draw();
    void cleanup();

private:
    string title;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif    //WINDOW_HPP