#ifndef CPU_HPP
#define CPU_HPP

#include <bitset>
#include <iomanip>
#include <memory>
#include "common.hpp"
// #include "bus.hpp"
#include "registers.hpp"
#include "instructions.hpp"
#include "ppu.hpp"

class CPU
{
private:
    u8 instruction_byte = 0;
    u8 interrupt_triggered = 0;

    u8 *div = 0;
    u8 *tima = 0;
    u8 *tma = 0;
    u8 *tac = 0;

    auto load_cpu_without_bootdmg() -> void;

    Registers *registers = nullptr;
    Instruction *inst = nullptr;
    PPU *ppu = nullptr;

public:
    CPU(Registers *regs_ptr, Instruction *inst_ptr, PPU *ppu_ptr);

    auto log_state(const string &stage, u8 instruction_byte, bool prefixed) -> void;
    auto timer(u8 cycle) -> void;
    auto interrupts() -> void;
    auto step() -> void;
    auto execute(const Instruction &instruction) -> u8;
};

#endif // CPU_HPP