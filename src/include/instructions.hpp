#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <memory>
#include "common.hpp"
#include "registers.hpp"

enum class LoadTarget
{
    A,
    B,
    C,
    D,
    E,
    H,
    L,
    CI,
    A8,
    BCI,
    DEI,
    HLI,
    HLIUP,
    HLILOW,
    BC,
    DE,
    HL,
    SP,
    A16
};

enum class LoadSource
{
    A,
    B,
    C,
    D,
    E,
    H,
    L,
    CI,
    N8,
    A8,
    BCI,
    DEI,
    HLI,
    HLIUP,
    HLILOW,
    A16,
    HL,
    SP,
    SPs8,
    N16
};

enum class LoadType
{
    Byte,
    World,
};

enum class JumpCondition
{
    NotZero,
    Zero,
    NotCarry,
    Carry,
    Always
};

enum class InstructionType
{
    ADD,
    ADC,
    SUB,
    SBC,
    AND,
    OR,
    XOR,
    CP,
    INC,
    DEC,
    CCF,
    SCF,
    RRA,
    RLA,
    RRCA,
    RLCA,
    CPL,
    BIT,
    ADDHL,
    RES,
    SET,
    SRL,
    RR,
    RL,
    RRC,
    RLC,
    SRA,
    SLA,
    SWAP,
    JP,
    JR,
    JPI,
    LD,
    PUSH,
    POP,
    CALL,
    RET,
    NOP,
    HALT,
    STOP,
    EI,
    DI,
    RST,
    CB,
};

class Instruction
{
public:
    u8 bit = 0;
    u8 N8_value = 0;
    u8 IME = 0;

private:
    InstructionType type;
    ArithmeticTarget target;
    LoadType loadtype;
    LoadTarget loadtarget;
    LoadSource loadsource;
    JumpCondition jump;

    u8 cycle_value = 0;

    Registers *registers = nullptr;

public:
    Instruction() = default;

    Instruction(Registers *regs_ptr)
        : registers(regs_ptr)
    {
        if (!registers)
        {
            throw runtime_error("Null pointer provided to Registers constructor");
        }
    }

    Instruction(InstructionType type, u8 cycle_value)
        : type(type), cycle_value(cycle_value) {}

    Instruction(InstructionType type, JumpCondition jump)
        : type(type), jump(jump) {}

    Instruction(InstructionType type, ArithmeticTarget target, u8 cycle_value)
        : type(type), target(target), cycle_value(cycle_value) {}

    Instruction(InstructionType type, ArithmeticTarget target, u8 bit, u8 cycle_value)
        : bit(bit), type(type), target(target), cycle_value(cycle_value) {}

    Instruction(InstructionType type, LoadType loadtype, LoadTarget loadtarget, LoadSource loadsource, u8 cycle_value)
        : type(type), loadtype(loadtype), loadtarget(loadtarget), loadsource(loadsource), cycle_value(cycle_value) {}

    auto get_inst_type() const -> InstructionType { return type; }
    auto get_load_type() const -> LoadType { return loadtype; }
    auto get_load_source() const -> LoadSource { return loadsource; }
    auto get_load_target() const -> LoadTarget { return loadtarget; }
    auto get_arithmetic_target() const -> ArithmeticTarget { return target; }
    auto get_jump_condition() const -> JumpCondition { return jump; }

    auto get_cycle_value() const -> u8 { return cycle_value; }

    static const array<Instruction, 0x100> instruction_map_prefixed;
    static const array<Instruction, 0x100> instruction_map_not_prefixed;

    static auto from_byte(u8 byte, bool prefixed) -> const Instruction *;
    static auto from_byte_prefixed(u8 byte) -> const Instruction *;
    static auto from_byte_not_prefixed(u8 byte) -> const Instruction *;

    auto add_inst(u8 value) -> u8;
    auto adc_inst(u8 value) -> u8;
    auto sub_inst(u8 value) -> u8;
    auto sbc_inst(u8 value) -> u8;
    auto and_inst(u8 value) -> u8;
    auto inc_inst(u8 value) -> u8;
    auto dec_inst(u8 value) -> u8;
    auto xor_inst(u8 value) -> u8;
    auto or_inst(u8 value) -> u8;
    auto addhl_inst(u16 value) -> u16;
    auto cp_inst(u8 value) -> void;
    auto ccf_inst() -> void;
    auto scf_inst() -> void;
    auto rra_inst() -> void;
    auto rla_inst() -> void;
    auto rrca_inst() -> void;
    auto rlca_inst() -> void;
    auto cpl_inst() -> void;
    auto bit_inst(u8 bit, u8 value) -> void;
    auto res_inst(u8 bit, u8 value) -> void;
    auto set_inst(u8 bit, u8 value) -> void;
    auto srl_inst(u8 value) -> void;
    auto rr_inst(u8 value) -> void;
    auto rl_inst(u8 value) -> void;
    auto rrc_inst(u8 value) -> void;
    auto rlc_inst(u8 value) -> void;
    auto sra_inst(u8 value) -> void;
    auto sla_inst(u8 value) -> void;
    auto swap_inst(u8 value) -> void;
    auto jp_inst(bool should_jump) -> void;
    auto jr_inst(bool should_jump) -> void;
    auto jpi_inst() -> void;
    auto ld_inst(const Instruction &instruction) -> void;
    auto byte_load(const Instruction &instruction) -> void;
    auto world_load(const Instruction &instruction) -> void;

    auto check_jump_condition(JumpCondition jump) -> bool;
    auto get_8_source(LoadSource source) -> u8;
    auto get_16_source(LoadSource source) -> u16;
};

#endif // INSTRUCTIONS_HPP