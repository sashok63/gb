#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <memory>
#include <unordered_map>
#include "common.hpp"
#include "registers.hpp"

// class CPU;

enum class LoadTarget {
    A, B, C, D, E, H, L, CI, A8,
    BCI, DEI, HLI, HLIUP, HLILOW,
    BC, DE, HL, SP, A16
};

enum class LoadSource {
    A, B, C, D, E, H, L, CI, N8, A8,
    BCI, DEI, HLI, HLIUP, HLILOW, A16,
    HL, SP, SPs8, N16
};

enum class LoadType {
    Byte,
    World,
};

enum class JumpCondition {
    NotZero,
    Zero,
    NotCarry,
    Carry,
    Always
};

enum class InstructionType {
    ADD,   ADC,  SUB,
    SBC,   AND,  OR,
    XOR,   CP,   INC,
    DEC,   CCF,  SCF,
    RRA,   RLA,  RRCA,
    RLCA,  CPL,  BIT,
    ADDHL, RES,SET,
    SRL,   RR,   RL,
    RRC,   RLC,  SRA,
    SLA,   SWAP, JP,
    JR,    JPI,  LD,
    PUSH,  POP,  CALL,
    RET,   NOP,  HALT,
    STOP,  EI,   DI,
    RST,
};

class Instruction {
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
    shared_ptr<Registers> registers;

    u8 cycle_value = 0;
    
public:
    Instruction() = default;
    ~Instruction() = default;

    Instruction(shared_ptr<Registers> regs_ptr)
        : registers(regs_ptr) {}

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

    InstructionType get_inst_type() const { return type; }
    LoadType get_load_type() const { return loadtype; }
    LoadSource get_load_source() const { return loadsource; }
    LoadTarget get_load_target() const { return loadtarget; }
    ArithmeticTarget get_arithmetic_target() const { return target; }
    JumpCondition get_jump_condition() const { return jump; }

    u8 get_cycle_value() const { return cycle_value; }

    static optional<Instruction> from_byte(u8 byte, bool prefixed);
    static optional<Instruction> from_byte_prefixed(u8 byte);
    static optional<Instruction> from_byte_not_prefixed(u8 byte);

    static const unordered_map<u8, Instruction> instruction_map_not_prefixed;
    static const unordered_map<u8, Instruction> instruction_map_prefixed;

    u8 add_inst(u8 value);            u8 adc_inst(u8 value);            u8 sub_inst(u8 value);
    u8 sbc_inst(u8 value);            u8 and_inst(u8 value);            u8 or_inst(u8 value);
    u8 xor_inst(u8 value);
    u16 addhl_inst(u16 value);
    void cp_inst(u8 value);           void inc_inst(u8 value);         void dec_inst(u8 value);
    void ccf_inst();                  void scf_inst();                  void rra_inst();
    void rla_inst();                  void rrca_inst();                 void rlca_inst();
    void cpl_inst();                  
    void bit_inst(u8 bit, u8 value); 
    void res_inst(u8 bit, u8 value);
    void set_inst(u8 bit, u8 value); 
    void srl_inst(u8 value);         void rr_inst(u8 value);          void rl_inst(u8 value);
    void rrc_inst(u8 value);         void rlc_inst(u8 value);         void sra_inst(u8 value);
    void sla_inst(u8 value);         void swap_inst(u8 value);
    void jp_inst(bool should_jump);  void rst_inst();
    void jr_inst(bool should_jump);
    void jpi_inst();
    void ld_inst(const Instruction& instruction);
    void byte_load(const Instruction& instruction);
    void world_load(const Instruction& instruction);

    bool check_jump_condition(JumpCondition jump);
    u8 get_8_source(LoadSource source);
    u16 get_16_source(LoadSource source);
};

#endif    //INSTRUCTIONS_HPP