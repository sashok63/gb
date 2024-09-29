#ifndef BUS_HPP
#define BUS_HPP

#include <fstream>
#include <stdexcept>
#include "common.hpp"
#include "gpu.hpp"

class MemoryBus {
public:
    static constexpr u32 GAMEBOY_MEM = 0x10000;
    static constexpr u16 BOOT_DMG = 0x00000;
    static constexpr u16 BOOT_DMG_SIZE = 0x00100;

    array<u8, GAMEBOY_MEM> memory = {};
private: 
    unique_ptr<Cartridge> cart;
    // GPU *gpu = nullptr;

public:    
    MemoryBus() = default;
    ~MemoryBus() = default;

    MemoryBus(unique_ptr<Cartridge> cart_ptr)
       : cart(move(cart_ptr))
       {
           if (!cart)
           {
               throw runtime_error("Null pointer provided to MemoryBus constructor");
           }
       }

    auto read_byte(u16 address) const noexcept -> u8 { return memory[address]; }
    auto write_byte(u16 address, u8 value) -> void { memory[address] = value; }

    auto load_boot_dmg() -> void;    
};

#endif    //BUS_HPP