#include "bus.hpp"

auto MemoryBus::read_byte(u16 address) const noexcept -> u8
{
    return memory[address];
}

auto MemoryBus::write_byte(u16 address, u8 value) -> void
{
    memory[address] = value;
}

auto MemoryBus::get_memory(u16 address) const noexcept -> u8
{
    if (address < GAMEBOY_MEM)
    {
        return memory[address];
    }
    return 0xFF; // Return 0xFF if the address is out of range (Game Boy convention)
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