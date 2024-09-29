#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "common.hpp"
#include "SDL2/SDL.h"

// #define RAM_SIZE 8192
// #define ROM_SIZE 16384

enum GameBoy_states {
    RUNNING,
    QUIT,
    PAUSED,
};

struct GameBoy {
    GameBoy_states state;
    // std::array<u8, RAM_SIZE> WRAM;
    // std::array<u8, RAM_SIZE> VRAM;
    // std::array<u8, ROM_SIZE> ROM;
};

void keyboard(GameBoy *gb);

#endif    //GAMEBOY_HPP