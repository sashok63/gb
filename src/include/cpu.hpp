#ifndef CPU_HPP
#define CPU_HPP

#include <iomanip>
#include <memory>
#include "common.hpp"
#include "bus.hpp"
#include "registers.hpp"
#include "instructions.hpp"

class CPU {
public:
    bool is_halted = false;
    
private:
    u16 div_clocksum = 0;
    u32 timer_clocksum = 0;

    shared_ptr<Registers> registers;
    unique_ptr<Instruction> inst;

public:
    CPU() = default;
    ~CPU() = default;

    CPU(shared_ptr<Registers> regs_ptr, unique_ptr<Instruction> inst_ptr)
        : registers(regs_ptr), inst(move(inst_ptr))
        {
            if (!registers || !inst)
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
    auto execute(const Instruction& instruction) -> u8;
};

#endif    //CPU_HPP