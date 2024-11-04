#ifndef CPU_HPP
#define CPU_HPP

#include <iomanip>
#include <memory>
#include "common.hpp"
#include "bus.hpp"
#include "registers.hpp"
#include "instructions.hpp"
#include "ppu.hpp"

class CPU
{
public:
    bool is_halted = false;

private:
    u8 instruction_byte = 0;
    u16 div_clocksum = 0;
    u16 ly_clocksum = 0;
    u32 timer_clocksum = 0;

    Registers *registers = nullptr;
    Instruction *inst = nullptr;
    PPU *ppu = nullptr;

public:
    CPU() = default;

    CPU(Registers *regs_ptr, Instruction *inst_ptr, PPU *ppu_ptr)
        : registers(regs_ptr), inst(inst_ptr), ppu(ppu_ptr)
    {
        if (!registers || !inst || !ppu)
        {
            throw runtime_error("Null pointer provided to CPU constructor");
        }

        registers->get_bus()->load_boot_dmg();
        registers->set_PC(0x0000);
    }

    auto log_state(const string &stage, u8 instruction_byte, bool prefixed) -> void;
    auto timer(u8 cycle) -> void;
    auto interrupts() -> void;
    auto step() -> void;
    auto execute(const Instruction &instruction) -> u8;
};

#endif // CPU_HPP