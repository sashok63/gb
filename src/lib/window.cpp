#include "window.hpp"

SDL2::SDL2(const std::string &title)
    : title(title), window(nullptr), renderer(nullptr) {}

SDL2::~SDL2()
{
    cleanup();
}

bool SDL2::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return false;
    }

    // if (TTF_Init() == -1)
    // {
    //     std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
    //     SDL_Quit();
    //     return 1;
    // }

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE,
                              SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;   
    }

    return true;
}

void SDL2::draw()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Rect fillRect = { 200, 200, 400, 400 };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);        
    SDL_RenderFillRect(renderer, &fillRect);

    SDL_RenderPresent(renderer);
}

void SDL2::cleanup()
{
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    // TTF_Quit();
    SDL_Quit();
}