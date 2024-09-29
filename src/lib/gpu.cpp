#include "gpu.hpp"

GPU::GPU() : tile_set{}, /*oam{},*/ vram{} {
        fill(tile_set.begin(), tile_set.end(), empty_tile());
    }

u8 GPU::read_vram(size_t address) const {
    return vram[address];
}

void GPU::write_vram(size_t address, u8 value) {
    vram[address] = value;
}

// u8 GPU::read_oam(size_t address) const {
//     return oam[address];
// }

// void GPU::write_oam(size_t address, u8 value) {
//     oam[address] = value;
// }

// void GPU::write_vram(size_t address, u8 value) {
//     vram[address] = value;
    
//     if (address >= 0x1800)
//     {
//         return;
//     }

//     size_t normalazed_address = address & 0xFFFE;
//     u8 byte1 = vram[normalazed_address];
//     u8 byte2 = vram[normalazed_address + 1];

//     size_t tile_address = address / 16;
//     size_t row_address = (address % 16) / 2;

//     for (u8 pixel_address = 0; pixel_address < 8; ++pixel_address)
//     {
//         u8 mask = 1 << (7 - pixel_address);
//         bool lsb = (byte1 & mask) != 0;
//         bool msb = (byte2 & mask) != 0;

//         TilePixelValue value;
//         if (lsb && msb)
//         {
//             value = TilePixelValue::Three;
//         }
//         else if (!lsb && msb)
//         {
//             value = TilePixelValue::Two;
//         }
//         else if (lsb && !msb)
//         {
//             value = TilePixelValue::One;
//         }
//         else
//         {
//             value = TilePixelValue::Zero;
//         }

//         tile_set[tile_address][row_address][pixel_address] = value;
//     }
// }