#include "bus.hpp"

auto MemoryBus::read_byte(u16 address) const noexcept -> u8
{
    return memory[address];
}

auto MemoryBus::write_byte(u16 address, u8 value) -> void
{
    if (address == 0xFF46) // Copy sprite from ROM to RAM
    {
        for (u16 i = 0; i < 160; i++)
        {
            write_byte(0xFE00 + i, read_byte((value << 8) + i));
        }
    }
    else if (address >= 0x8000 && address < 0x9800) // Update tile
    {
        update_tile(address, value);
    }
    else if (address == 0xFF47) // Update palette BGP
    {
        for (u8 i = 0; i < 4; i++)
        {
            palette_BGP[i] = palette[(value >> (i * 2)) & 3];
        }
    }
    else if (address == 0xFF48) // Update palette sprite 1
    {
        for (u8 i = 0; i < 4; i++)
        {
            palette_sprite[0][i] = palette[(value >> (i * 2)) & 3];
        }  
    }
    else if (address == 0xFF48) // Update palette sprite 2
    {
        for (u8 i = 0; i < 4; i++)
        {
            palette_sprite[1][i] = palette[(value >> (i * 2)) & 3];
        }  
    }

    memory[address] = value;
}

auto MemoryBus::update_tile(u16 address, u8 value) -> void
{
    (void)value;
    address &= 0x1FFE;

    u8 tile = (address >> 4) & 511;
    u8 y = (address >> 1) & 7;

    for (u8 x = 0; x < 8; x++)
    {
        u8 bit = 1 << (7 - x);
        tiles[tile][y][x] = ((memory[address] & bit) ? 1 : 0) + ((memory[address + 1] & bit) ? 2 : 0);
    }
}

auto MemoryBus::get_memory(u16 address) -> u8 &
{
    return memory[address];
}

auto MemoryBus::set_memory(u16 address, u8 value) noexcept -> void
{
    if (address < GAMEBOY_MEM)
    {
        memory[address] = value;
    }
}

auto MemoryBus::load_boot_dmg() -> void
{
    ifstream file("/home/sashok63/c++/gameboy/logo/DMG_ROM.bin", ios::binary | ios::ate);
    if (!file.is_open())
    {
        throw runtime_error("Failed to open file 'logo/DMG_ROM.bin'");
    }

    streamsize file_size = file.tellg();
    if (file_size > BOOT_DMG_SIZE)
    {
        throw runtime_error("File 'test/DMG_ROM.bin' is too large to fit in boot_dmg");
    }

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char *>(memory.data() + BOOT_DMG), file_size);
    if (!file)
    {
        throw runtime_error("Failed to read the entire file 'test/DMG_ROM.bin'");
    }

    // Load Nintendo Logo to memory
    const array<u8, 0x30> &logo = cart->get_nintedo_logo();
    for (u8 i = 0; i < logo.size(); ++i)
    {
        memory[NINTENDO_LOGO_ADDR + i] = logo[i];
    }
}

auto MemoryBus::load_test() -> void
{
    ifstream file("/home/sashok63/c++/gameboy/test/DMG_ROM.bin", ios::binary | ios::ate);
    if (!file.is_open())
    {
        throw runtime_error("Failed to open file 'test/...'");
    }

    streamsize file_size = file.tellg();

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char *>(memory.data() + BOOT_DMG), file_size);
    if (!file)
    {
        throw runtime_error("Failed to read the entire file 'test/...'");
    }
}