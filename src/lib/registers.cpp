#include "registers.hpp"

// u16 Registers::get_PC() const
// {
//     return PC;
// }

// u16 Registers::get_SP() const
// {
//     return SP;
// }

// void Registers::set_PC(u16 value)
// {
//     PC = value;
// }

// void Registers::set_SP(u16 value)
// {
//     SP = value;
// }

// u8 Registers::get_a() const
// {
//     return a;
// }
// u8 Registers::get_b() const
// {
//     return b;
// }
// u8 Registers::get_c() const
// {
//     return c;
// }
// u8 Registers::get_d() const
// {
//     return d;
// }
// u8 Registers::get_e() const
// {
//     return e;
// }
// u8 Registers::get_f() const
// {
//     return f;
// }
// u8 Registers::get_h() const
// {
//     return h;
// }
// u8 Registers::get_l() const
// {
//     return l;
// }

// void Registers::set_a(u8 value)
// {
//     a = value;
// }
// void Registers::set_b(u8 value)
// {
//     b = value;
// }
// void Registers::set_c(u8 value)
// {
//     c = value;
// }
// void Registers::set_d(u8 value)
// {
//     d = value;
// }
// void Registers::set_e(u8 value)
// {
//     e = value;
// }
// void Registers::set_f(u8 value)
// {
//     f = value;
// }
// void Registers::set_h(u8 value)
// {
//     h = value;
// }
// void Registers::set_l(u8 value)
// {
//     l = value;
// }

// u16 Registers::get_AF() const
// {
//     return static_cast<u16>(a) << 8 | f;
// }

// u16 Registers::get_BC() const
// {
//     return static_cast<u16>(b) << 8 | c;
// }

// u16 Registers::get_DE() const
// {
//     return static_cast<u16>(d) << 8 | e;
// }

// u16 Registers::get_HL() const
// {
//     return static_cast<u16>(h) << 8 | l;
// }

// void Registers::set_AF(u16 value)
// {
//     a = static_cast<u8>((value & 0xFF00) >> 8);
//     f = static_cast<u8>(value & 0xFF);
// }

// void Registers::set_BC(u16 value)
// {
//     b = static_cast<u8>((value & 0xFF00) >> 8);
//     c = static_cast<u8>(value & 0xFF);
// }

// void Registers::set_DE(u16 value)
// {
//     d = static_cast<u8>((value & 0xFF00) >> 8);
//     e = static_cast<u8>(value & 0xFF);
// }

// void Registers::set_HL(u16 value)
// {
//     h = static_cast<u8>((value & 0xFF00) >> 8);
//     l = static_cast<u8>(value & 0xFF);
// }

auto Registers::read_next_byte() -> u8
{
    u8 byte = get_bus()->read_byte(PC);
    PC += 1;
    return byte;
}

auto Registers::read_next_world() -> u16
{
    u8 lower_byte = get_bus()->read_byte(PC);
    u8 upper_byte = get_bus()->read_byte(PC + 1);
    PC += 2;
    return (static_cast<u16>(upper_byte) << 8) | lower_byte;
}

auto Registers::get_register(ArithmeticTarget target) const -> u8
{
    switch (target)
    {
        case ArithmeticTarget::A: return a;
        case ArithmeticTarget::B: return b;
        case ArithmeticTarget::C: return c;
        case ArithmeticTarget::D: return d;
        case ArithmeticTarget::E: return e;
        case ArithmeticTarget::H: return h;
        case ArithmeticTarget::L: return l;
        case ArithmeticTarget::HLI: {
            static u8 temp;
            temp = bus->read_byte(get_HL());
            return temp;
        }
        default: {
            throw runtime_error("Unknown target at get_register: " + to_string(static_cast<u16>(target)));
        }
    }
}

auto Registers::set_register(ArithmeticTarget target, u8 value) -> void
{
    switch (target)
    {
        case ArithmeticTarget::A: a = value; break;
        case ArithmeticTarget::B: b = value; break;
        case ArithmeticTarget::C: c = value; break;
        case ArithmeticTarget::D: d = value; break;
        case ArithmeticTarget::E: e = value; break;
        case ArithmeticTarget::H: h = value; break;
        case ArithmeticTarget::L: l = value; break;
        case ArithmeticTarget::HLI: bus->write_byte(get_HL(), value); break;
        default: {
            ostringstream error_message;
            error_message << "Unknown target at set_register: " << static_cast<int>(target);
            throw runtime_error(error_message.str());
        }
    }
}

auto Registers::get_register_pair(ArithmeticTarget target) const -> u16
{
    switch (target)
    {
        case ArithmeticTarget::BC: return get_BC();
        case ArithmeticTarget::DE: return get_DE();
        case ArithmeticTarget::HL: return get_HL();
        case ArithmeticTarget::AF: return get_AF();
        default: {
            ostringstream error_message;
            error_message << "Unknown target at get_register_pair: " << static_cast<int>(target);
            throw runtime_error(error_message.str());
        }
    }
}

auto Registers::set_register_pair(ArithmeticTarget target, u16 value) -> void
{
    switch (target)
    {
        case ArithmeticTarget::BC: set_BC(value); break;
        case ArithmeticTarget::DE: set_DE(value); break;
        case ArithmeticTarget::HL: set_HL(value); break;
        case ArithmeticTarget::AF: set_AF(value); break;
        default: {
            ostringstream error_message;
            error_message << "Unknown target at set_register_pair: " << static_cast<int>(target);
            throw runtime_error(error_message.str());
        }
    }
}