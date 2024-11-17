#include "ppu.hpp"
#include <assert.h>

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
                                SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
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
    frame_buffer.fill(0xFF);
}

auto PPU::calc_bg(u8 row) -> void
{
    // Validate row range
    if (row >= 144)
    {
        throw runtime_error("Invalidate row range at ppu calc_bg: " + to_string(static_cast<u16>(row)));
        return; // Ignore rows outside visible range
    }

    u8 scy = bus->read_byte(0xFF42);
    u8 scx = bus->read_byte(0xFF43);

    bool use_alt_tilemap = (bus->get_memory(0xFF40) >> 3) & 1;
    bool use_signed_tiledata = !((bus->get_memory(0xFF40) >> 4) & 1);

    u16 tilemap = use_alt_tilemap ? 0x9C00 : 0x9800;
    u16 tiledata = use_signed_tiledata ? 0x8800 : 0x8000;

    u8 palette = bus->read_byte(0xFF47);

    // Starting x-scroll offset within the tile
    u8 initial_x_offset = scx & 7;

    // Row and column offsets for the first visible tile
    u8 offY = row + scy;
    u8 tile_row = (offY / 8) * 32; // Tilemap row offset

    // Render the visible 160 pixels
    for (u8 visible_x = 0; visible_x < 160;)
    {
        // Get the tile address and number
        u8 offX = scx + visible_x;                       // Horizontal offset into the map
        u16 tile_addr = tilemap + tile_row + (offX / 8); // Tile index in the tilemap
        u8 tile_num = bus->read_byte(tile_addr);

        // Calculate the tile data address (signed or unsigned)
        u16 tile_offset = use_signed_tiledata
                              ? 0x800 + static_cast<i8>(tile_num) * 0x10
                              : tile_num * 0x10;

        // Get the pixel line within the tile
        u16 line_offset = (offY % 8) * 2;
        u8 byte1 = bus->read_byte(tiledata + tile_offset + line_offset);
        u8 byte2 = bus->read_byte(tiledata + tile_offset + line_offset + 1);

        // Render up to 8 pixels from this tile
        for (u8 tile_pixel = initial_x_offset; tile_pixel < 8 && visible_x < 160; ++tile_pixel, ++visible_x)
        {
            // Extract the pixel's color value (2 bits)
            u8 color_val = ((byte2 >> (7 - tile_pixel)) & 0x1) * 2 + ((byte1 >> (7 - tile_pixel)) & 0x1);

            // Map the color value to the palette
            u8 color_from_pal = (palette >> (2 * color_val)) & 0x3;

            // Write the pixel to the framebuffer
            frame_buffer[row * 160 + visible_x] = color_from_pal;
        }

        // Reset initial_x_offset after the first tile
        initial_x_offset = 0;
    }
}

auto PPU::calc_window(u8 row) -> void
{
    // Validate row range
    if (row >= 144)
    {
        throw runtime_error("Invalidate row range at ppu calc_window: " + to_string(static_cast<u16>(row)));
        return; // Ignore rows outside visible range
    }

    // Read control registers
    u8 lcdc = bus->read_byte(0xFF40);
    u8 wy = bus->read_byte(0xFF4A);     // Window Y position
    u8 wx = bus->read_byte(0xFF4B) - 7; // Window X position

    // Exit early if the window is disabled or the row is outside the window
    if (!((lcdc >> 5) & 0x1) || row < wy)
    {
        return;
    }

    // Determine tilemap and tiledata base addresses
    bool use_alt_tilemap = (bus->get_memory(0xFF40) >> 3) & 1;
    bool use_signed_tiledata = !((bus->get_memory(0xFF40) >> 4) & 1);

    u16 tilemap = use_alt_tilemap ? 0x9C00 : 0x9800;
    u16 tiledata = use_signed_tiledata ? 0x8800 : 0x8000;

    // Calculate window's vertical offset
    u8 offY = row - wy;
    u16 tile_row = (offY / 8) * 32; // Offset into the tilemap for this row

    for (u8 visible_x = 0; visible_x < 160; ++visible_x)
    {
        // Horizontal offset into the window
        i16 offX = wx + visible_x;
        if (offX < 0 || offX >= 160)
        {
            continue; // Skip pixels that are outside the visible window
        }

        // Calculate tile address and fetch tile number
        u16 tile_addr = tilemap + tile_row + (offX / 8);
        u8 tile_num = bus->read_byte(tile_addr);

        // Calculate tile data address
        u16 tile_offset = use_signed_tiledata
                              ? 0x800 + static_cast<i8>(tile_num) * 0x10
                              : tile_num * 0x10;

        // Fetch tile line bytes
        u16 line_offset = (offY % 8) * 2;
        u8 byte1 = bus->read_byte(tiledata + tile_offset + line_offset);
        u8 byte2 = bus->read_byte(tiledata + tile_offset + line_offset + 1);

        // Extract pixel color
        u8 color_val = ((byte2 >> (7 - (offX % 8))) & 0x1) * 2 +
                       ((byte1 >> (7 - (offX % 8))) & 0x1);

        // Map color value using the background/window palette
        u8 color_from_pal = (bus->read_byte(0xFF47) >> (2 * color_val)) & 0x3;

        // Write pixel to the framebuffer
        frame_buffer[row * 160 + visible_x] = color_from_pal;
    }
}

auto PPU::calc_sprite(u8 row) -> void
{
    // Validate row range
    if (row >= 144)
    {
        throw runtime_error("Invalidate row range at ppu calc_sprite: " + to_string(static_cast<u16>(row)));
        return; // Ignore rows outside visible range
    }

    // Read control registers
    u16 oam_start = 0xFE00;
    u16 oam_end = 0xFE9F;
    u8 sprite_height = (bus->read_byte(0xFF40) & 0x4) ? 16 : 8;

    // Check if sprites are enabled
    if (!(bus->read_byte(0xFF40) & 0x2))
    {
        return;
    }

    // Iterate through all 40 sprites in OAM
    for (u16 oam_addr = oam_start; oam_addr <= oam_end; oam_addr += 4)
    {
        // Extract sprite attributes
        u8 y_pos = bus->read_byte(oam_addr) - 16;
        u8 x_pos = bus->read_byte(oam_addr + 1) - 8;
        u8 tile_num = bus->read_byte(oam_addr + 2);
        u8 flags = bus->read_byte(oam_addr + 3);

        // Skip sprites outside the current scanline
        if (row < y_pos || row >= y_pos + sprite_height)
            continue;

        // Handle flipping and palette selection
        bool flip_x = flags & 0x20;
        bool flip_y = flags & 0x40;
        u16 palette_addr = (flags & 0x10) ? 0xFF49 : 0xFF48;

        // Calculate the tile's base address
        u16 tile_addr = 0x8000 + tile_num * 0x10;

        // Determine the sprite line within the tile
        u8 sprite_line = flip_y ? (sprite_height - 1 - (row - y_pos)) : (row - y_pos);
        u8 byte1 = bus->read_byte(tile_addr + sprite_line * 2);
        u8 byte2 = bus->read_byte(tile_addr + sprite_line * 2 + 1);

        // Process the sprite's 8 horizontal pixels
        for (u8 pixel = 0; pixel < 8; ++pixel)
        {
            u8 bit = flip_x ? pixel : (7 - pixel);
            u8 color_val = ((byte2 >> bit) & 0x1) * 2 + ((byte1 >> bit) & 0x1);

            // Skip transparent pixels
            if (color_val == 0)
            {
                continue;
            }

            // Get the color from the palette
            u8 color_from_palette = (bus->read_byte(palette_addr) >> (2 * color_val)) & 0x3;

            // Calculate the position in the framebuffer
            u16 x = x_pos + pixel;

            // Write the color to the framebuffer
            frame_buffer[row * 160 * x] = color_from_palette;
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

    for (u8 i = 0; i < 144 * 160; i++)
    {
        u8 color_index = frame_buffer[i];
        frame_buffer_a[i * 4 + 0] = COLORS[color_index * 3 + 0]; // Red
        frame_buffer_a[i * 4 + 1] = COLORS[color_index * 3 + 1]; // Green
        frame_buffer_a[i * 4 + 2] = COLORS[color_index * 3 + 2]; // Blue
        frame_buffer_a[i * 4 + 3] = 0xFF;                        // Alpha
    }

    if (SDL_UpdateTexture(texture, nullptr, frame_buffer_a.data(), SCREEN_WIDTH * 4) != 0)
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
    ppu_cycle += cycle;

    // Read necessary registers
    u8 stat = bus->read_byte(0xFF41);
    u8 ly = bus->read_byte(0xFF44);
    u8 lyc = bus->read_byte(0xFF45);
    u8 interrupt_flag = bus->read_byte(0xFF0F);

    // Check if the LCD is enabled
    // if (!(bus->read_byte(0xFF40) & 0x80))
    // { // Bit 7 of 0xFF40 controls LCD enable
    //     mode = 0;
    //     // if (ppu_cycle >= 70224) // Full frame cycle reset
    //     //     ppu_cycle -= 70224;
    //     return;
    // }

    switch (mode)
    {
    case 0: // H-Blank
        if (ppu_cycle >= 51)
        {
            ppu_cycle -= 51;
            mode = 2;
            ly++;
            bus->write_byte(0xFF44, ly);

            // Check LY == LYC coincidence
            if (ly == lyc && (stat & 0x40)) // Bit 6 enables LYC interrupt
            {
                interrupt_flag |= 0x2;
                bus->write_byte(0xFF0F, interrupt_flag);
            }

            // Enter V-Blank if LY reaches 144
            if (ly == 144)
            {
                mode = 1;
                interrupt_flag |= 0x1; // V-Blank interrupt
                bus->write_byte(0xFF0F, interrupt_flag);
                if (stat & 0x10) // Bit 4 enables V-Blank interrupt
                {
                    interrupt_flag |= 0x2;
                    bus->write_byte(0xFF0F, interrupt_flag);
                }
            }
            else if (stat & 0x20) // Bit 5 enables OAM interrupt
            {
                interrupt_flag |= 0x2;
                bus->write_byte(0xFF0F, interrupt_flag);
            }

            // Update mode in STAT register
            bus->write_byte(0xFF41, (stat & 0xFC) | mode);
        }
        break;

    case 1: // V-Blank
        if (ppu_cycle >= 456)
        {
            ppu_cycle -= 456;
            ly++;
            bus->write_byte(0xFF44, ly);

            if (ly == 153)
            {
                ly = 0;
                mode = 2;
                bus->write_byte(0xFF44, ly);
                if (stat & 0x20) // Bit 5 enables OAM interrupt
                {
                    interrupt_flag |= 0x2;
                    bus->write_byte(0xFF0F, interrupt_flag);
                }
            }

            // Update mode in STAT register
            bus->write_byte(0xFF41, (stat & 0xFC) | mode);
        }
        break;

    case 2: // OAM (Object Attribute Memory)
        if (ppu_cycle >= 20)
        {
            ppu_cycle -= 20;
            mode = 3;

            // Update mode in STAT register
            bus->write_byte(0xFF41, (stat & 0xFC) | mode);
        }
        break;

    case 3:
        if (ppu_cycle >= 43)
        {
            ppu_cycle -= 43;
            mode = 0;

            // Render the current scanline
            calc_bg(ly);
            calc_window(ly);
            calc_sprite(ly);

            // Update mode in STAT register
            bus->write_byte(0xFF41, (stat & 0xFC) | mode);

            if (stat & 0x08) // Bit 3 enables H-Blank interrupt
            {
                interrupt_flag |= 0x2;
                bus->write_byte(0xFF0F, interrupt_flag);
            }
        }
        break;

    default:
        throw runtime_error("Unknown mode at ppu step: " + to_string(static_cast<u16>(mode)));
        break;
    }

    // Frame rendering
    if (ly == 144 && !frame_drawn_flag && (bus->read_byte(0xFF40) & 0x80))
    {
        draw_frame();
        frame_drawn_flag = true;

        frame_buffer.fill(0);
        // bg_map_a.fill(0);
        // win_map_a.fill(0);
        // sprite_map_a.fill(0);  
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