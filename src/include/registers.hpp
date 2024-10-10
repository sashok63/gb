#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include "common.hpp"
#include "bus.hpp"

enum class ArithmeticTarget {
	A, B, C, D, E, H, L,
	AF, BC, DE, HL, HLI,
	SP,
};

class FlagsRegister {
public:
    bool zero = false;
    bool subtract = false;
    bool half_carry = false;
    bool carry = false;

    FlagsRegister() = default;
    ~FlagsRegister() = default;
};

class Registers {
private:
    u16 PC = 0;
    u16 SP = 0;
    
    u8 a = 0;
    u8 b = 0;
    u8 c = 0;
    u8 d = 0;
    u8 e = 0;
    u8 f = 0;
    u8 h = 0;
    u8 l = 0;  

    shared_ptr<MemoryBus> bus;
    shared_ptr<FlagsRegister> flags;

public:
    Registers() = default;
    ~Registers() = default;

    Registers(shared_ptr<MemoryBus> bus_ptr, shared_ptr<FlagsRegister> flags_ptr)
        : bus(bus_ptr), flags(flags_ptr)
        {
            if (!bus || !flags)
            {
                throw runtime_error("Null pointer provided to Registers constructor");
            }
        }

    auto get_bus() const -> MemoryBus* { return bus.get(); }
    auto get_flag() const -> FlagsRegister* { return flags.get(); }

    auto get_PC() const noexcept -> u16;
    auto get_SP() const noexcept -> u16;

    auto set_PC(u16 value) -> void;
    auto set_SP(u16 value) -> void;

    auto get_a() const noexcept -> u8;
    auto get_b() const noexcept -> u8;
    auto get_c() const noexcept -> u8;
    auto get_d() const noexcept -> u8;
    auto get_e() const noexcept -> u8;
    auto get_f() const noexcept -> u8;
    auto get_h() const noexcept -> u8;
    auto get_l() const noexcept -> u8;

    auto set_a(u8 value) -> void;
    auto set_b(u8 value) -> void;
    auto set_c(u8 value) -> void;
    auto set_d(u8 value) -> void;
    auto set_e(u8 value) -> void;
    auto set_f(u8 value) -> void;
    auto set_h(u8 value) -> void;
    auto set_l(u8 value) -> void;

    auto get_AF() const noexcept -> u16;
    auto get_BC() const noexcept -> u16;
    auto get_DE() const noexcept -> u16;
    auto get_HL() const noexcept -> u16;

    auto set_AF(u16 value) -> void;
    auto set_BC(u16 value) -> void;
    auto set_DE(u16 value) -> void;
    auto set_HL(u16 value) -> void;

    auto read_next_byte() -> u8;
    auto read_next_world() -> u16;
    
    auto get_register(ArithmeticTarget target) const -> u8;
    auto set_register(ArithmeticTarget target, u8 value) -> void;
    auto get_register_pair(ArithmeticTarget target) const -> u16;
    auto set_register_pair(ArithmeticTarget target, u16 value) -> void;
};

#endif    //REGISTERS_HPP