#ifndef BUS_HPP
#define BUS_HPP

#include "common.hpp"
#include "cart.hpp"

typedef class Colour
{
public:
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;

    constexpr Colour() = default;
    constexpr Colour(u8 red, u8 green, u8 blue)
        : r(red), g(green), b(blue) {}
} Colour;

class MemoryBus
{
private:
    static constexpr u32 GAMEBOY_MEM = 0xFFFF;
    static constexpr u16 BOOT_DMG = 0x00000;
    static constexpr u16 BOOT_DMG_SIZE = 0x00100;
    static constexpr u16 NINTENDO_LOGO_ADDR = 0x0104;

    Cartridge *cart = nullptr;

    array<u8, GAMEBOY_MEM> memory = {};

    auto update_tile(u16 address, u8 value) -> void;

public:
    MemoryBus() = default;

    MemoryBus(Cartridge *cart_ptr)
        : cart(cart_ptr)
    {
        if (!cart)
        {
            throw runtime_error("Null pointer provided to MemoryBus constructor");
        }
    }
    
    static constexpr array<Colour, 4> palette = {
        Colour{255, 255, 255},
        Colour{192, 192, 192},
        Colour{96, 96, 96},
        Colour{0, 0, 0},
    };

    array<array<array<u8, 8>, 8>, 384> tiles = {};
    array<Colour, 4> palette_BGP = {};
    array<array<Colour, 4>, 2> palette_sprite = {};

    auto get_cart() const -> Cartridge * { return cart; }

    auto read_byte(u16 address) const noexcept -> u8;
    auto write_byte(u16 address, u8 value) -> void;

    auto get_memory(u16 address) -> u8 &; // For reference
    auto set_memory(u16 address, u8 value) noexcept -> void;

    auto load_boot_dmg() -> void;
    auto load_test() -> void;
};

#endif // BUS_HPP