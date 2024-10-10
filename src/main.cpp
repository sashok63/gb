#include "gameboy.hpp"
#include "cpu.hpp"
#include "ppu.hpp"

auto main(int /*argc*/, char */*argv*/[]) -> int
{
    auto bus = make_shared<MemoryBus>();
    auto flags = make_shared<FlagsRegister>();
    auto regs = make_shared<Registers>(bus, flags);
    auto inst = make_unique<Instruction>(regs);
    auto cpu = make_unique<CPU>(regs, move(inst));
    auto ppu = make_unique<PPU>(bus);

    ppu->init();

    GameBoy gb = {RUNNING};

    while (!gb.state)
    {
        do
        {
            keyboard(&gb);
        } while (gb.state == PAUSED);

        cpu->step();
        
        ppu->draw();
    }

    ppu->quit();

    return 0;
}
