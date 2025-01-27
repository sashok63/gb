#ifndef PPU_HPP
#define PPU_HPP

#include "common.hpp"
#include "registers.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

struct Sprite
{
    u8 y;
    u8 x;
    u8 tile;
    union
    {
        struct Flags
        {
            u8 palette : 1;
            u8 hFlip : 1;
            u8 vFlip : 1;
            u8 render_priority : 1;
            u8 unused : 4;
        } bits;
        u8 flags;
    } options;
};

class PPU
{
private:
    constexpr static u8 SCREEN_WIDTH = 160;
    constexpr static u8 SCREEN_HEIGHT = 144;

    array<Colour, 160 * 144> frame_buffer;    // 160X144

    u16 ppu_cycle = 0;
    u8 mode = 0;
    u8 *control = 0;
    u8 *stat = 0;
    u8 *ly = 0; // Scanline
    u8 *lyc = 0;
    u8 *scy = 0; // Scroll Y
    u8 *scx = 0; // Scroll X
    u8 *interrupt_flag = 0;

    SDL_Rect texture_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;

    MemoryBus *bus = nullptr;
    Registers *registers = nullptr;

public:
    PPU(MemoryBus *bus_ptr, Registers *regs_ptr);

    bool frame_drawn_flag = 0;

    auto get_ppu_cycle() const -> u8 { return ppu_cycle; }

    auto init() -> void;
    auto draw_scanline() -> void;
    auto draw_frame() -> void;
    auto step(u8 cycle) -> void;
    auto compare_ly_lyc() -> void;
    auto quit() -> void;
};

#endif // GPU_HPP