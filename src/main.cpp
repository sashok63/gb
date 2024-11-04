#include "gameboy.hpp"
#include "cpu.hpp"
#include "ppu.hpp"

#include <csignal>

void signalHandler(int signum)
{
    cout << "Interrupt signal (" << signum << ") received.\n";
    exit(signum);
}

auto main(int /*argc*/, char * /*argv*/[]) -> int
{
    signal(SIGINT, signalHandler);

    Cartridge *cart = new Cartridge();
    MemoryBus *bus = new MemoryBus(cart);
    FlagsRegister *flags = new FlagsRegister();
    Registers *regs = new Registers(bus, flags);
    Instruction *inst = new Instruction(regs);
    PPU *ppu = new PPU(bus);
    CPU *cpu = new CPU(regs, inst, ppu);

    GameBoy gb = {RUNNING};

    while (!gb.state)
    {
        do
        {
            keyboard(&gb);
        } while (gb.state == PAUSED);

        cpu->step();
    }

    ppu->quit();

    delete cpu;
    delete inst;
    delete regs;
    delete flags;
    delete ppu;
    delete bus;

    return 0;
}
