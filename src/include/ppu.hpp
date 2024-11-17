#ifndef PPU_HPP
#define PPU_HPP

#include "common.hpp"
#include "bus.hpp"
#include <SDL2/SDL.h>

class PPU
{
private:
    u16 ppu_cycle = 0;
    u8 mode = 2;
    bool frame_drawn_flag = 0;

    array<u8, 160 * 144> frame_buffer = {};       // 160X144
    array<u8, 160 * 144 * 4> frame_buffer_a = {}; // 160X144, 4 bytes per pixel, RGB24A
    // array<u8, 256 * 256 * 4> bg_map_a = {};       // 256X256, 4 bytes per pixel, RGB24A
    // array<u8, 256 * 256 * 4> win_map_a = {};      // 256X256, 4 bytes per pixel, RGB24A
    // array<u8, 256 * 256 * 4> sprite_map_a = {};   // 256X256, 4 bytes per pixel, RGB24A

    constexpr static u8 SCREEN_WIDTH = 160;
    constexpr static u8 SCREEN_HEIGHT = 144;
    constexpr static u8 PIXEL_SIZE = 4;
    constexpr static u8 SCALE = 3;
    constexpr static array<u8, 12> COLORS = {
        0xFF, 0xFF, 0xFF,
        0xAA, 0xAA, 0xAA,
        0x55, 0x55, 0x55,
        0x00, 0x00, 0x00};

    SDL_Rect texture_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

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
    }

    auto get_ppu_cycle() const -> u8 { return ppu_cycle; }

    auto init() -> void;
    auto calc_bg(u8 row) -> void;
    auto calc_window(u8 row) -> void;
    auto calc_sprite(u8 row) -> void;
    auto draw_frame() -> void;
    auto step(u8 cycle) -> void;
    auto quit() -> void;
};

#endif // GPU_HPP