#include "window.hpp"
#include "gameboy.hpp"
#include "cpu.hpp"

auto main(int /*argc*/, char */*argv*/[]) -> int
{
    SDL2 sdl("GameBoy Emulator");

    auto bus = make_shared<MemoryBus>();
    auto flags = make_shared<FlagsRegister>();
    auto regs = make_shared<Registers>(bus, flags);
    auto inst = make_unique<Instruction>(regs);
    auto cpu = make_unique<CPU>(regs, move(inst));

    if (!sdl.init()) return 1;

    GameBoy gb = {RUNNING};

    while (!gb.state)
    {
        do
        {
            keyboard(&gb);
        } while (gb.state == PAUSED);

        cpu->step();
        
        sdl.draw();
    }

    return 0;
}
