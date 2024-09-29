#ifndef CART_HPP
#define CART_HPP

#include "common.hpp"

enum class TilePixelValue : u8 {
    Zero, One, Two, Three
};

using Tile = array<array<TilePixelValue, 8>, 8>;

Tile empty_tile();

class Cartridge {
public:
    // static constexpr u16 ROM_BANK_BEGIN = 0x0000;
    // static constexpr u16 ROM_BANK_END  = 0x3FFF;
    // static constexpr u16 ROM_BANK_SIZE = 0x4000;

    // static constexpr u16 ROM_BANK_SWITCHABLE_BEGIN = 0x4000;
    // static constexpr u16 ROM_BANK_SWITCHABLE_END  = 0x7FFF;
    // static constexpr u16 ROM_BANK_SWITCHABLE_SIZE = 0x4000;

    // static constexpr u16 ERAM_BEGIN = 0xA000;
    // static constexpr u16 ERAM_END  = 0xBFFF;
    // static constexpr u16 ERAM_SIZE = 0x2000;

    // static constexpr u16 WRAM_BEGIN = 0xC000;
    // static constexpr u16 WRAM_END  = 0xCFFF;
    // static constexpr u16 WRAM_SIZE = 0x1000;

    // static constexpr u16 WRAM_SWITCHABLE_BEGIN = 0xD000;
    // static constexpr u16 WRAM_SWITCHABLE_END  = 0xDFFF;
    // static constexpr u16 WRAM_SWITCHABLE_SIZE = 0x1000;

    // static constexpr u16 ECHO_RAM_BEGIN = 0xE000;
    // static constexpr u16 ECHO_RAM_END  = 0xFDFF;
    // static constexpr u16 ECHO_RAM_SIZE = 0x2000;

    // static constexpr u16 UNUSABLE_START = 0xFEA0;
    // static constexpr u16 UNUSABLE_END = 0xFEFF;

    // static constexpr u16 IO_REGS_BEGIN = 0xFF00;
    // static constexpr u16 IO_REGS_END  = 0xFF7F;
    // static constexpr u16 IO_REGS_SIZE = 0x80;

    // static constexpr u16 HRAM_BEGIN = 0xFF80;
    // static constexpr u16 HRAM_END  = 0xFFFE;
    // static constexpr u16 HRAM_SIZE = 0x7F;

    // static constexpr u16 IE_REGISTER = 0xFFFF;

    // static constexpr u8 MAX_BANKS = 128;

    // array<u8, ROM_BANK_SIZE> rom_bank;
    // array<array<u8, ROM_BANK_SWITCHABLE_SIZE>, MAX_BANKS> rom_bank_sw;
    // array<array<u8, ERAM_SIZE>, MAX_BANKS> eram_bank;
    // array<u8, WRAM_SIZE> wram_bank;
    // array<array<u8, WRAM_SWITCHABLE_SIZE>, MAX_BANKS> wram_bank_sw;
    // array<u8, ECHO_RAM_SIZE> echo_ram_bank;
    // array<u8, IO_REGS_SIZE> io_regs_bank;
    // array<u8, HRAM_SIZE> hram;
    // u16 current_bank = 0;
    // u16 current_eram_bank = 0;
    // u16 current_wram_bank = 0;
    // u16 IE_enable = 0x00;

    // u8 read_bank00(u16 address) const;
    // void write_bank00(u16 address, u8 value);
    // u8 read_bank_sw(u16 address) const;
    // void write_bank_sw(u16 address, u8 value);
    // u8 read_bank_eram(u16 address) const;
    // void write_bank_eram(u16 address, u8 value);
    // u8 read_bank_wram(u16 address) const;
    // void write_bank_wram(u16 address, u8 value);
    // u8 read_bank_wram_sw(u16 address) const;
    // void write_bank_wram_sw(u16 address, u8 value);
    // u8 read_bank_echo_ram(u16 address) const;
    // void write_bank_echo_ram(u16 address, u8 value);
    // u8 read_hram(u16 address) const;
    // void write_hram(u16 address, u8 value);
    // u8 read_ie_reg() const;
    // void write_ie_reg(u8 value);
};

#endif    //CART_HPP