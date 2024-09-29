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

    auto get_PC() const noexcept -> u16 { return PC; }
    auto get_SP() const noexcept -> u16 { return SP; }

    auto set_PC(u16 value) -> void { PC = value; }
    auto set_SP(u16 value) -> void { SP = value; }

    auto get_a() const noexcept -> u8 { return a; }
    auto get_b() const noexcept -> u8 { return b; }
    auto get_c() const noexcept -> u8 { return c; }
    auto get_d() const noexcept -> u8 { return d; }
    auto get_e() const noexcept -> u8 { return e; }
    auto get_f() const noexcept -> u8 { return f; }
    auto get_h() const noexcept -> u8 { return h; }
    auto get_l() const noexcept -> u8 { return l; }

    auto set_a(u8 value) -> void { a = value; }
    auto set_b(u8 value) -> void { b = value; }
    auto set_c(u8 value) -> void { c = value; }
    auto set_d(u8 value) -> void { d = value; }
    auto set_e(u8 value) -> void { e = value; }
    auto set_f(u8 value) -> void { f = value; }
    auto set_h(u8 value) -> void { h = value; }
    auto set_l(u8 value) -> void { l = value; }

    auto get_AF() const noexcept -> u16 { return static_cast<u16>(a) << 8 | f; }
    auto get_BC() const noexcept -> u16 { return static_cast<u16>(b) << 8 | c; }
    auto get_DE() const noexcept -> u16 { return static_cast<u16>(d) << 8 | e; }
    auto get_HL() const noexcept -> u16 { return static_cast<u16>(h) << 8 | l; }

    auto set_AF(u16 value) -> void { a = static_cast<u8>((value & 0xFF00) >> 8); f = static_cast<u8>(value & 0xFF); };
    auto set_BC(u16 value) -> void { b = static_cast<u8>((value & 0xFF00) >> 8); c = static_cast<u8>(value & 0xFF); };
    auto set_DE(u16 value) -> void { d = static_cast<u8>((value & 0xFF00) >> 8); e = static_cast<u8>(value & 0xFF); };
    auto set_HL(u16 value) -> void { h = static_cast<u8>((value & 0xFF00) >> 8); l = static_cast<u8>(value & 0xFF); };

    auto read_next_byte() -> u8;
    auto read_next_world() -> u16;
    
    auto get_register(ArithmeticTarget target) const -> u8;
    auto set_register(ArithmeticTarget target, u8 value) -> void;
    auto get_register_pair(ArithmeticTarget target) const -> u16;
    auto set_register_pair(ArithmeticTarget target, u16 value) -> void;
};

#endif    //REGISTERS_HPP