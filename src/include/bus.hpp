#ifndef BUS_HPP
#define BUS_HPP

#include "common.hpp"
#include "cart.hpp"

class MemoryBus
{
private:
    static constexpr u32 GAMEBOY_MEM = 0x10000;
    static constexpr u16 BOOT_DMG = 0x00000;
    static constexpr u16 BOOT_DMG_SIZE = 0x00100;
    static constexpr u16 NINTENDO_LOGO_ADDR = 0x0104;

    Cartridge *cart = nullptr;

    array<u8, GAMEBOY_MEM> memory = {};

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

    auto get_cart() const -> Cartridge * { return cart; }

    auto get_memory(u16 address) const noexcept -> u8;
    auto set_memory(u16 address, u8 value) noexcept -> void;

    auto read_byte(u16 address) const noexcept -> u8;
    auto write_byte(u16 address, u8 value) -> void;

    auto load_boot_dmg() -> void;
};

#endif // BUS_HPP