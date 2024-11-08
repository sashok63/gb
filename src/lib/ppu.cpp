#include "ppu.hpp"

auto PPU::init() -> void
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    // if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
    //                                 0,
    //                                 &window, &renderer) != 0)
    // {
    //     SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    // }

    window = SDL_CreateWindow("GameBoy Emulator",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE,
                              SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // SDL_SetWindowSize(window, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);

    if (SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT) != 0)
    {
        SDL_Log("Failed to set logical size: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    SDL_SetWindowResizable(window, SDL_TRUE);

    texture = SDL_CreateTexture(renderer,
                                  SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                                  SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bus->write_byte(0xFF41, 0x80);
}

auto PPU::calc_bg(u8 row) -> void
{
    u8 scy = bus->read_byte(0xFF42);
    u8 scx = bus->read_byte(0xFF43);

    bool use_alt_tilemap = (bus->get_memory(0xFF40) >> 3) & 1;
    bool use_signed_tiledata = !((bus->get_memory(0xFF40) >> 4) & 1);

    u16 tilemap = use_alt_tilemap ? 0x9C00 : 0x9800;
    u16 tiledata = use_signed_tiledata ? 0x8800 : 0x8000;

    u8 palette = bus->read_byte(0xFF47);

    for (u16 j = 0; j < 256; j++)
    {
        u8 offY = row + scy;
        u8 offX = j + scx;
        u16 tile_addr = tilemap + (offY / 8) * 32 + (offX / 8);

        u8 tile_num = bus->read_byte(tile_addr);

        u16 tile_offset = use_signed_tiledata ? 0x800 + static_cast<i8>(tile_num) * 0x10 : tile_num * 0x10;
        u16 line_offset = (offY % 8) * 2;
        u8 byte1 = bus->read_byte(tiledata + tile_offset + line_offset);
        u8 byte2 = bus->read_byte(tiledata + tile_offset + line_offset + 1);
        u8 color_val = ((byte2 >> (7 - (offX % 8))) & 0x1) * 2 + ((byte1 >> (7 - (offX % 8))) & 0x1);

        u8 color_from_pal = (palette >> (2 * color_val)) & 0x3;
        u8 color = COLORS[color_from_pal];

        bg_map_a[(row * 256 + j) * 4 + 0] = color & 0xFF;
        bg_map_a[(row * 256 + j) * 4 + 1] = (color >> 8) & 0xFF;
        bg_map_a[(row * 256 + j) * 4 + 2] = (color >> 16) & 0xFF;
        bg_map_a[(row * 256 + j) * 4 + 3] = 0xFF;
    }
}

auto PPU::calc_window(u8 row) -> void
{
    bool use_alt_tilemap = (bus->get_memory(0xFF40) >> 3) & 1;
    bool use_signed_tiledata = !((bus->get_memory(0xFF40) >> 4) & 1);

    u16 tilemap = use_alt_tilemap ? 0x9C00 : 0x9800;
    u16 tiledata = use_signed_tiledata ? 0x8800 : 0x8000;

    u8 lcdc = bus->read_byte(0xFF40);
    u8 wy = bus->read_byte(0xFF4A);
    u8 wx = bus->read_byte(0xFF4B) - 7;

    if ((lcdc >> 5) & 0x1)
    {
        for (u16 j = wx; j < 256 && j < wx + 160; j++)
        {
            if (row < wy || row >= wy + 144)
            {
                continue;
            }

            u8 offY = row - wy;
            u8 offX = j - wx;
            u16 tile_addr = tilemap + (offY / 8) * 32 + (offX / 8);

            u8 tile_num = bus->read_byte(tile_addr);

            u16 tile_offset = use_signed_tiledata ? 0x800 + static_cast<i8>(tile_num) * 0x10 : tile_num * 0x10;
            u16 line_offset = (offY % 8) * 2;
            u8 byte1 = bus->read_byte(tiledata + tile_offset + line_offset);
            u8 byte2 = bus->read_byte(tiledata + tile_offset + line_offset + 1);
            u8 color_val = ((byte2 >> (7 - (offX % 8))) & 0x1) * 2 + ((byte1 >> (7 - (offX % 8))) & 0x1);

            u8 color_from_pal = (bus->read_byte(0xFF47) >> (2 * color_val)) & 0x3;
            u8 color = COLORS[color_from_pal];

            win_map_a[(row * 256 + j) * 4 + 0] = color & 0xFF;
            win_map_a[(row * 256 + j) * 4 + 1] = (color >> 8) & 0xFF;
            win_map_a[(row * 256 + j) * 4 + 2] = (color >> 16) & 0xFF;
            win_map_a[(row * 256 + j) * 4 + 3] = 0xFF;
        }
    }
}

auto PPU::calc_sprite(u8 row) -> void
{
    u16 oam = 0xFE00;
    u16 oam_end = 0xFE9F;
    u8 sprite_height = (bus->read_byte(0xFF40) & 0x4) ? 16 : 8;

    if ((bus->read_byte(0xFF40) & 0x2) == 0)
        return;

    for (u16 i = oam; i <= oam_end; i += 4)
    {
        u8 y = bus->read_byte(i) - 16;
        u8 x = bus->read_byte(i + 1) - 8;
        u8 tile_num = bus->read_byte(i + 2);
        u8 flags = bus->read_byte(i + 3);

        if (row < y || row >= y + sprite_height)
        {
            continue;
        }

        bool flip_x = flags & 0x20;
        bool flip_y = flags & 0x40;
        u16 tile_addr = 0x8000 + tile_num * 0x10;

        for (u8 u = 0; u < sprite_height; u++)
        {
            u8 line = flip_y ? (sprite_height - 1 - u) : u;
            u8 byte1 = bus->read_byte(tile_addr + line * 2);
            u8 byte2 = bus->read_byte(tile_addr + line * 2 + 1);

            for (u8 v = 0; v < 8; v++)
            {
                u8 bit = flip_x ? v : (7 - v);
                u8 color_val = ((byte2 >> bit) & 0x1) * 2 + ((byte1 >> bit) & 0x1);

                if (color_val == 0)
                {
                    continue;
                }

                u16 color_addr = (flags & 0x10) ? 0xFF49 : 0xFF48;
                u8 color_from_pal = (bus->read_byte(color_addr) >> (2 * color_val)) & 0x3;
                u8 color = COLORS[color_from_pal];

                u16 frame_pos = ((y + u) * 256 + (x + v)) * 4;
                sprite_map_a[frame_pos + 0] = color & 0xFF;
                sprite_map_a[frame_pos + 1] = (color >> 8) & 0xFF;
                sprite_map_a[frame_pos + 2] = (color >> 16) & 0xFF;
                sprite_map_a[frame_pos + 3] = 0xFF;
            }
        }
    }
}

auto PPU::draw_frame() -> void
{
    for (u8 row = 0; row < 144; row++)
    {
        for (u8 col = 0; col < 160; col++)
        {
            u16 bg_index = (row * 256 + col) * 4;
            u16 frame_index = (row * 160 + col) * 4;

            frame_buffer_a[frame_index + 0] = bg_map_a[bg_index + 0];
            frame_buffer_a[frame_index + 1] = bg_map_a[bg_index + 1];
            frame_buffer_a[frame_index + 2] = bg_map_a[bg_index + 2];
            frame_buffer_a[frame_index + 3] = bg_map_a[bg_index + 3];

            if (win_map_a[bg_index + 3] != 0x00)
            {
                frame_buffer_a[frame_index + 0] = win_map_a[bg_index + 0];
                frame_buffer_a[frame_index + 1] = win_map_a[bg_index + 1];
                frame_buffer_a[frame_index + 2] = win_map_a[bg_index + 2];
                frame_buffer_a[frame_index + 3] = win_map_a[bg_index + 3];
            }

            if (sprite_map_a[bg_index + 3] != 0x00)
            {
                frame_buffer_a[frame_index + 0] = sprite_map_a[bg_index + 0];
                frame_buffer_a[frame_index + 1] = sprite_map_a[bg_index + 1];
                frame_buffer_a[frame_index + 2] = sprite_map_a[bg_index + 2];
                frame_buffer_a[frame_index + 3] = sprite_map_a[bg_index + 3];
            }
        }
    }

    if (SDL_UpdateTexture(texture, nullptr, frame_buffer_a.data(), 160 * sizeof(u8) * 4) != 0)
    {
        SDL_Log("Failed to update texture: %s", SDL_GetError());
        SDL_Quit();
    }
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

auto PPU::step(u8 cycle) -> void
{
    ppu_cycle += cycle;
    u8 stat = bus->read_byte(0xFF41);
    u8 ly = bus->read_byte(0xFF44);
    u8 lyc = bus->read_byte(0xFF45);
    u8 interrupt_flag = bus->read_byte(0xFF0F);

    // Mode transition handling
    if (ppu_cycle < 81) // Mode 2 - OAM mode
    {
        stat = (stat & 0xFC) | 0x2;
        bus->write_byte(0xFF41, stat);
        state = 2;
        if ((stat >> 5) & 0x1 && last_state != 2)
        {
            interrupt_flag |= 0x2;
            bus->write_byte(0xFF0F, interrupt_flag);
        }
        last_state = 2;
    }
    else if (ppu_cycle < 253) // Mode 3 - Drawing
    {
        stat = (stat & 0xFC) | 0x3;
        bus->write_byte(0xFF41, stat);
        state = 3;
    }
    else if (ppu_cycle < 457) // Mode 0 - H-Blank
    {
        stat = (stat & 0xFC) | 0x0;
        bus->write_byte(0xFF41, stat);
        state = 0;
        if ((stat >> 3) & 0x1 && last_state != 0)
        {
            interrupt_flag |= 0x2;
            bus->write_byte(0xFF0F, interrupt_flag);
        }
        last_state = 0;
    }

    // V-Blank mode check
    if (ly >= 144)
    {
        stat = (stat & 0xFC) | 0x1;
        bus->write_byte(0xFF41, stat);
        state = 1;
        if ((stat >> 4) & 0x1 && last_state != 1)
        {
            interrupt_flag |= 0x2;
            bus->write_byte(0xFF0F, interrupt_flag);
        }
        last_state = 1;
    }

    // Line rendering
    if (ppu_cycle >= 252 && ly < 144 && !line_calc_flag)
    {
        calc_bg(ly);
        calc_window(ly);
        calc_sprite(ly);
        line_calc_flag = true;
    }

    // Frame rendering
    if (!frame_drawn_flag && ly == 144 && (bus->read_byte(0xFF40) >> 7))
    {
        interrupt_flag |= 0x1; // V-Blank interrupt
        bus->write_byte(0xFF0F, interrupt_flag);
        draw_frame();
        frame_drawn_flag = true;
        fill(begin(bg_map_a), end(bg_map_a), 0);
        fill(begin(win_map_a), end(win_map_a), 0);
        fill(begin(sprite_map_a), end(sprite_map_a), 0);
    }

    // Increment LY register and reset line flag
    if (ppu_cycle >= 456)
    {
        ppu_cycle -= 456;
        ly++;
        bus->write_byte(0xFF44, ly);
        line_calc_flag = false;
    }

    // LY/LYC Coincidence
    if (ly == lyc)
    {
        if ((stat >> 6) & 0x1 && ((stat >> 2) & 0x1) == 0)
        {
            interrupt_flag |= 0x2; // Coincidence interrupt
            bus->write_byte(0xFF0F, interrupt_flag);
            stat |= 0x4;
            bus->write_byte(0xFF41, stat);
        }
    }
    else
    {
        stat &= ~0x4;
        bus->write_byte(0xFF41, stat);
    }

    // End of frame
    if (ly > 153)
    {
        ly = 0;
        bus->write_byte(0xFF44, ly);
        frame_drawn_flag = false;
    }
}

auto PPU::quit() -> void
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}