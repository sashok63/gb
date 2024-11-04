#include "ppu.hpp"

auto PPU::init() -> void
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
    }

    window = SDL_CreateWindow("GameBoy Emulator",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
    }

    SDL_SetWindowSize(window, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                TEXTURE_WIDTH, TEXTURE_HEIGHT);

    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    SDL_SetWindowResizable(window, SDL_TRUE);
}

auto PPU::draw() -> void
{
    // Draw Backbground
    for (u8 row = 0; row < 144; row++)
    {
        for (u8 col = 0; col < 160; col++)
        {
            u32 yoff_a = row * 256 * PIXEL_SIZE;
            u32 xoff_a = row * PIXEL_SIZE;

            u8 tile_id = bus->get_memory(tilemap + (row / 8) * 32 + (col / 8));
            u8 tile_line = bus->get_memory(tiledata + (tile_id * 16) + (row % 8) * 2);
            u8 color_value = (tile_line >> (7 - (col % 8))) & 1;

            u8 color_from_palette = (bus->get_memory(0xFF47) >> (color_value * 2)) & 0x03;

            bg_map_a[yoff_a + xoff_a] = COLORS[color_from_palette * 3];
            bg_map_a[yoff_a + xoff_a + 1] = COLORS[color_from_palette * 3 + 1];
            bg_map_a[yoff_a + xoff_a + 2] = COLORS[color_from_palette * 3 + 2];
            bg_map_a[yoff_a + xoff_a + 3] = 0xFF;
        }
    }

    // Draw Frame
    for (u8 row = 0; row < 144; row++)
    {
        for (u8 col = 0; col < 160; col++)
        {
            u32 yoff_a = row * 256 * PIXEL_SIZE;
            u32 xoff_a = row * PIXEL_SIZE;

            u8 frame_buffer_id_x = (row * SCREEN_WIDTH * PIXEL_SIZE) + (col * PIXEL_SIZE);

            frame_buffer_RGBA[frame_buffer_id_x] = bg_map_a[yoff_a + xoff_a];
            frame_buffer_RGBA[frame_buffer_id_x + 1] = bg_map_a[yoff_a + xoff_a + 1];
            frame_buffer_RGBA[frame_buffer_id_x + 2] = bg_map_a[yoff_a + xoff_a + 2];
            frame_buffer_RGBA[frame_buffer_id_x + 3] = bg_map_a[yoff_a + xoff_a + 3];
        }
    }

    SDL_UpdateTexture(texture, nullptr, frame_buffer_RGBA.data(), SCREEN_WIDTH * PIXEL_SIZE);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

auto PPU::quit() -> void
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

auto PPU::setup_tilemap_and_tiledata() -> void
{
    tilemap = (((bus->get_memory(0xFF40) >> 3) & 1) == 1) ? 0x9C00 : 0x9800;
    tiledata = (((bus->get_memory(0xFF40) >> 4) & 1) == 1) ? 0x8000 : 0x8800;
}