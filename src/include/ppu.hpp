#ifndef PPU_HPP
#define PPU_HPP

#include "common.hpp"
#include "bus.hpp"
#include <SDL2/SDL.h>

class PPU
{
private:
    u8 tilemap = 0;
    u8 tiledata = 0;

    array<u8, 160 * 144 * 3> frame_buffer_RGB = {};  // RGB24, 160X144, 3 bytes per pixel
    array<u8, 160 * 144 * 4> frame_buffer_RGBA = {}; // RGBA24, 160X144, 4 bytes per pixel
    array<u8, 160 * 256 * 4> bg_map_a = {};

    constexpr static u8 SCREEN_WIDTH = 160;
    constexpr static u8 SCREEN_HEIGHT = 144;
    constexpr static u8 TEXTURE_WIDTH = 160;
    constexpr static u8 TEXTURE_HEIGHT = 144;
    constexpr static u8 PIXEL_SIZE = 4;
    constexpr static u8 SCALE = 3;
    constexpr static array<u8, 12> COLORS = {
        255, 255, 255,
        192, 192, 192,
        96, 96, 96,
        0, 0, 0};

    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;

    MemoryBus *bus = nullptr;

public:
    PPU() = default;

    PPU(MemoryBus *bus_ptr)
        : bus(bus_ptr)
    {
        if (!bus)
        {
            throw runtime_error("Null pointer provided to PPU constructor");
        }

        init();

        setup_tilemap_and_tiledata();
    }

    auto init() -> void;
    auto draw() -> void;
    auto quit() -> void;
    auto setup_tilemap_and_tiledata() -> void;
};

#endif // GPU_HPP