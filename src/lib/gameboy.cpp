#include "gameboy.hpp"

void keyboard(GameBoy *gb)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    gb->state = QUIT;
                    break;

                case SDL_QUIT:
                    gb->state = QUIT;
                    break;

                case SDLK_SPACE:
                    if (gb->state == PAUSED) gb->state = RUNNING;
                    else gb->state = PAUSED;
                    break;

                default:
                    break;
            }
        }
    }
}