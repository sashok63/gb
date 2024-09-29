#ifndef GPU_HPP
#define GPU_HPP

#include "common.hpp"
#include "cart.hpp"

class GPU {
public:
    GPU();

    static constexpr size_t VRAM_BEGIN = 0x8000;
    static constexpr size_t VRAM_END  = 0x9FFF;
    static constexpr size_t VRAM_SIZE = 0x2000;

    // static constexpr size_t OAM_BEGIN = 0xFE00;
    // static constexpr size_t OAM_END  = 0xFE9F;
    // static constexpr size_t OAM_SIZE = 0xA0;

    array<Tile, 384> tile_set;
    // array<u8, OAM_SIZE> oam;
    array<u8, VRAM_SIZE> vram;

    u8 read_vram(size_t address) const;
    void write_vram(size_t address, u8 value);
    // u8 read_oam(size_t address) const;
    // void write_oam(size_t address, u8 value);
};

#endif    //GPU_HPP