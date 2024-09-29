#include "cart.hpp"

// Cartridge() : {}

Tile empty_tile() {
    return {{{TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero},
             {TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero,
              TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero, TilePixelValue::Zero}}};
}

// u8 Cartridge::read_bank00(u16 address) const {
//     return rom_bank[address];
// }

// void Cartridge::write_bank00(u16 address, u8 value) {
//     rom_bank[address] = value;
// }

// u8 Cartridge::read_bank_sw(u16 address) const {
//     return rom_bank_sw[current_bank][address];
// }

// void Cartridge::write_bank_sw(u16 address, u8 value) {
//     rom_bank_sw[current_bank][address] = value;
// }

// u8 Cartridge::read_bank_eram(u16 address) const {
//     return eram_bank[current_eram_bank][address];
// }

// void Cartridge::write_bank_eram(u16 address, u8 value) {
//     eram_bank[current_eram_bank][address] = value;
// }

// u8 Cartridge::read_bank_wram(u16 address) const {
//     return wram_bank[address];
// }

// void Cartridge::write_bank_wram(u16 address, u8 value) {
//     wram_bank[address] = value;
// }

// u8 Cartridge::read_bank_wram_sw(u16 address) const {
//     return wram_bank_sw[current_wram_bank][address];
// }

// void Cartridge::write_bank_wram_sw(u16 address, u8 value) {
//     wram_bank_sw[current_wram_bank][address] = value;
// }

// u8 Cartridge::read_bank_echo_ram(u16 address) const {
//     return echo_ram_bank[address];
// }

// void Cartridge::write_bank_echo_ram(u16 address, u8 value) {
//     echo_ram_bank[address] = value;
// }

// u8 Cartridge::read_hram(u16 address) const {
//     return hram[address];
// }

// void Cartridge::write_hram(u16 address, u8 value) {
//     hram[address] = value;
// }

// u8 Cartridge::read_ie_reg() const {
//     return IE_enable;
// }

// void Cartridge::write_ie_reg(u8 value) {
//     IE_enable = value;
// }