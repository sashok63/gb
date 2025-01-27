#include "ppu.hpp"

PPU::PPU(MemoryBus *bus_ptr, Registers *regs_ptr) : bus(bus_ptr), registers(regs_ptr)
{
    if (!bus || !registers)
    {
        throw runtime_error("Null pointer provided to PPU constructor");
    }

    // Intialization of SDL2
    init();

    // Set vars
    control = &bus->get_memory(0xFF40);
    stat = &bus->get_memory(0xFF41);
    scy = &bus->get_memory(0xFF42);
    scx = &bus->get_memory(0xFF43);
    ly = &bus->get_memory(0xFF44);
    lyc = &bus->get_memory(0xFF45);
    interrupt_flag = &bus->get_memory(0xFF0F);

    // Setup palettes
    bus->palette_BGP = bus->palette;
    fill(bus->palette_sprite.begin(), bus->palette_sprite.end(), bus->palette);

    // Setup tiles
    bus->tiles.fill(array<array<u8, 8>, 8>{});

    // Setup frame buffer
    fill(frame_buffer.begin(), frame_buffer.end(), Colour{255, 255, 255});
};

auto PPU::init() -> void
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }

    // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    window = SDL_CreateWindow("GameBoy Emulator",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH /* * SCALE */, SCREEN_HEIGHT /* * SCALE */,
                              SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    //   SDL_RENDERER_TARGETTEXTURE);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // SDL_SetWindowSize(window, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);

    u8 rend_logic = SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (rend_logic != 0)
    {
        SDL_Log("Failed to set logical size: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_SetWindowResizable(window, SDL_TRUE);

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    bus->write_byte(0xFF41, 0x80);
}

auto PPU::draw_scanline() -> void
{
    if (*ly >= 144)
    {
        return;
    }

    u8 scanline_row[160] = {0};

    u16 map_offset = *control & 0x08 ? 0x1C00 : 0x1800;
    map_offset += (((*ly + *scy) & 0xFF) >> 3) << 5;

    u8 line_offset = *scx >> 3;
    u8 x = *scx & 0x07;
    u8 y = (*ly + *scy) & 0x07;
    u16 pixel_offset = *ly * 160;

    u8 tile = bus->read_byte(map_offset + line_offset + 0x8000);

    // Background
    for (u8 i = 0; i < 160; i++)
    {
        if (tile >= bus->tiles.size())
        {
            break;
        }

        u8 colour = bus->tiles[tile][y][x];
        scanline_row[i] = colour;

        if (pixel_offset < frame_buffer.size() && colour < 4)
        {
            frame_buffer[pixel_offset].r = bus->palette_BGP[colour].r;
            frame_buffer[pixel_offset].g = bus->palette_BGP[colour].g;
            frame_buffer[pixel_offset].b = bus->palette_BGP[colour].b;
        }

        x++;
        if (x == 8)
        {
            x = 0;
            line_offset = (line_offset + 1) & 0x1F;
            tile = bus->read_byte(map_offset + line_offset + 0x8000);
        }

        pixel_offset++;
    }

    // Sprite
    for (u8 i = 0; i < 40; i++)
    {
        Sprite sprite; // Each sprite is 4 bytes long
        sprite.y = bus->read_byte(0xFE00 + i * 4);
        sprite.x = bus->read_byte(0xFE01 + i * 4);
        sprite.tile = bus->read_byte(0xFE02 + i * 4);
        sprite.options.flags = bus->read_byte(0xFE03 + i * 4);

        u8 sprite_y = sprite.y - 16;
        u8 sprite_x = sprite.x - 8;

        if (sprite_y <= *ly && (sprite_y + 8) > *ly)
        {
            Colour *palette = &bus->palette_sprite[sprite.options.bits.palette][0];
            pixel_offset = *ly * 160 + sprite_x;

            u8 tile_row = 0;
            if (sprite.options.bits.vFlip)
            {
                tile_row = 7 - *ly - sprite_y;
            }
            else
            {
                tile_row = *ly - sprite_y;
            }

            for (x = 0; x < 8; x++)
            {
                if (sprite_x + x >= 0 && sprite_x + x < 160 &&
                    (~sprite.options.bits.render_priority || !scanline_row[sprite_x + x]))
                {
                    u8 colour;
                    if (sprite.options.bits.hFlip)
                    {
                        colour = bus->tiles[sprite.tile][tile_row][7 - x];
                    }
                    else
                    {
                        colour = bus->tiles[sprite.tile][tile_row][x];
                    }

                    if (colour && colour < 4)
                    {
                        frame_buffer[pixel_offset].r = palette[colour].r;
                        frame_buffer[pixel_offset].g = palette[colour].g;
                        frame_buffer[pixel_offset].b = palette[colour].b;
                    }
                    pixel_offset++;
                }
            }
        }
    }
}

auto PPU::draw_frame() -> void
{
    if (SDL_SetTextureColorMod(texture, 255, 255, 255) != 0)
    {
        SDL_Log("Failed to set texture color mod: %s", SDL_GetError());
        SDL_Quit();
    }

    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0)
    {
        SDL_Log("Failed to set render draw color: %s", SDL_GetError());
        SDL_Quit();
    }

    if (SDL_RenderClear(renderer) != 0)
    {
        SDL_Log("Failed to render clear: %s", SDL_GetError());
        SDL_Quit();
    }

    if (SDL_SetRenderTarget(renderer, texture) != 0)
    {
        SDL_Log("Failed to set render target: %s", SDL_GetError());
        SDL_Quit();
    }

    if (SDL_UpdateTexture(texture, nullptr, frame_buffer.data(), SCREEN_WIDTH * 3) != 0)
    {
        SDL_Log("Failed to update texture: %s", SDL_GetError());
        SDL_Quit();
    }

    if (SDL_RenderCopy(renderer, texture, nullptr, &texture_rect) != 0)
    {
        SDL_Log("Failed to render copy: %s", SDL_GetError());
        SDL_Quit();
    }

    SDL_RenderPresent(renderer);
}

auto PPU::step(u8 cycle) -> void
{
    // TODO: Delete later
    *ly = 0x8F; // TODO: Delete later
    // TODO: Delete later

    ppu_cycle += cycle;

    // Delete it later
    (*ly)++;

    switch (mode)
    {
    case 0: // H-Blank
        if (ppu_cycle >= 51)
        {
            ppu_cycle -= 51;
            mode = 2;

            (*ly)++;
            compare_ly_lyc();

            // Enter V-Blank if LY reaches 144
            if (*ly == 144)
            {
                mode = 1;
                frame_drawn_flag = true;
                registers->set_interrupt_flag(INTERRUPT_VBANK);
                if (*stat & 0x10) // Bit 4 enables V-Blank interrupt
                {
                    registers->set_interrupt_flag(INTERRUPT_LCD);
                }
            }
            else if (*stat & 0x20) // Bit 5 enables OAM interrupt
            {
                registers->set_interrupt_flag(INTERRUPT_LCD);
            }

            // Update mode in STAT register
            *stat = (*stat & 0xFC) | (mode & 3);
        }
        break;

    case 1: // V-Blank
        if (ppu_cycle >= 114)
        {
            ppu_cycle -= 114;

            (*ly)++;
            compare_ly_lyc();

            if (*ly == 153)
            {
                *ly = 0;
                mode = 2;

                // Update mode in STAT register
                *stat = (*stat & 0xFC) | (mode & 3);

                if (*stat & 0x20) // Bit 5 enables OAM interrupt
                {
                    registers->set_interrupt_flag(INTERRUPT_LCD);
                }
            }
        }
        break;

    case 2: // OAM (Object Attribute Memory)
        if (ppu_cycle >= 20)
        {
            ppu_cycle -= 20;
            mode = 3;

            // Update mode in STAT register
            *stat = (*stat & 0xFC) | (mode & 3);
        }
        break;

    case 3: // V-RAM (Video RAM)
        if (ppu_cycle >= 43)
        {
            ppu_cycle -= 43;
            mode = 0;

            // Render the current scanline
            draw_scanline();

            // Update mode in STAT register
            *stat = (*stat & 0xFC) | (mode & 3);

            if (*stat & 0x08) // Bit 3 enables H-Blank interrupt
            {
                registers->set_interrupt_flag(INTERRUPT_LCD);
            }
        }
        break;

    default:
        throw runtime_error("Unknown mode at ppu step: " + to_string(static_cast<u16>(mode)));
        break;
    }

    // Frame rendering
    if (frame_drawn_flag)
    {
        draw_frame();
        frame_drawn_flag = false;
    }
}

auto PPU::compare_ly_lyc() -> void
{
    *stat = (*stat & ~0x04) | ((*lyc == *ly) ? 0x04 : 0x00);
    if (*lyc == *ly && (*stat & 0x04))
    {
        registers->set_interrupt_flag(INTERRUPT_LCD);
    }
}

auto PPU::quit() -> void
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}