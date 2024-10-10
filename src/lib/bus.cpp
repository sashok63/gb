#include "bus.hpp"

auto MemoryBus::read_byte(u16 address) const noexcept -> u8
{
    return memory[address];
}

auto MemoryBus::write_byte(u16 address, u8 value) -> void
{
    memory[address] = value;
}

auto MemoryBus::load_boot_dmg() -> void
{
    // ifstream file("/home/sashok63/c++/gameboy/test/DMG_ROM.bin", ios::binary | ios::ate);
    ifstream file("/home/sashok63/c++/gameboy/test/gb_boot_rom.bin", ios::binary | ios::ate);
    if (!file.is_open())
    {
        throw runtime_error("Failed to open file 'test/DMG_ROM.bin'");
    }

    streamsize file_size = file.tellg();
    // if (file_size > BOOT_DMG_SIZE)
    if (file_size > BOOT_DMG_TEST_SIZE)
    {
        throw runtime_error("File 'test/DMG_ROM.bin' is too large to fit in boot_dmg");
    }

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char*>(memory.data() + BOOT_DMG), file_size);
    if (!file)
    {
        throw runtime_error("Failed to read the entire file 'test/DMG_ROM.bin'");
    }
}