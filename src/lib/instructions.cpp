#include "instructions.hpp"

u8 Instruction::add_inst(u8 value)
{
    u16 result = static_cast<u16>(registers->get_a()) + value;
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = (result > 0xFF);
    registers->get_flag()->carry = ((registers->get_a() & 0xF) + (value & 0xF) > 0xF);

    return new_value;
}

u8 Instruction::adc_inst(u8 value)
{
    u8 carry = registers->get_flag()->carry ? 1 : 0;
    u16 result = static_cast<u16>(registers->get_a()) + value + carry;
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = (registers->get_a() & 0xF) < (value & 0xF);
    registers->get_flag()->carry = (registers->get_a() < value);

    return new_value;
}

u8 Instruction::sub_inst(u8 value)
{
    u16 result = static_cast<u16>(registers->get_a()) - value;
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = true;
    registers->get_flag()->half_carry = (result > 0xFF);
    registers->get_flag()->carry = ((registers->get_a() & 0xF) + (value & 0xF) > 0xF);

    return new_value;
}

u8 Instruction::sbc_inst(u8 value)
{
    u16 carry = registers->get_flag()->carry ? 1 : 0;
    u16 result = static_cast<u16>(registers->get_a()) - value - carry;
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = true;
    registers->get_flag()->half_carry = ((registers->get_a() & 0xF) < (value & 0xF) + carry);
    registers->get_flag()->carry = (result > 0xFF);

    return new_value;
}

u8 Instruction::and_inst(u8 value)
{
    u8 a = registers->get_a();
    u16 result = static_cast<u16>(a &= value);
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = true;
    registers->get_flag()->carry = false;

    return new_value;
}

u8 Instruction::or_inst(u8 value)
{
    u8 a = registers->get_a();
    u16 result = static_cast<u16>(a | value);
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = false;

    return new_value;
}

u8 Instruction::xor_inst(u8 value)
{
    u8 a = registers->get_a();
    u16 result = static_cast<u16>(a ^ value);
    u8 new_value = static_cast<u8>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = false;

    return new_value;
}

void Instruction::cp_inst(u8 value)
{
    u16 result = static_cast<u16>(registers->get_a()) - value;

    registers->get_flag()->zero = ((result & 0xFF) == 0);
    registers->get_flag()->subtract = true;
    registers->get_flag()->half_carry = ((registers->get_a() & 0xF) < (value & 0xF));
    registers->get_flag()->carry = (registers->get_a() < value);
}

void Instruction::inc_inst(u8 value)
{
    // registers->get_flag()->half_carry = ((value & 0x0F) == 0); // Here or 
    value++;

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = ((value & 0x0F) == 0); // or Here
}

void Instruction::dec_inst(u8 value)
{
    // registers->get_flag()->half_carry = ((value & 0x0F) == 0); // Here or 
    value--;

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = true;
    registers->get_flag()->half_carry = ((value & 0x0F) == 0); // or Here
}

void Instruction::ccf_inst()
{
    registers->get_flag()->zero = false;
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = !registers->get_flag()->carry;
}

void Instruction::scf_inst()
{
    registers->get_flag()->zero = false;
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = true;
}

void Instruction::rra_inst()
{
    bool carry_out = registers->get_flag()->carry;
    bool lsb = (registers->get_a() & 0x01) != 0;

    u8 a = (registers->get_a() >> 1) | (carry_out ? 0x80 : 0x00);
    registers->set_a(a);

    registers->get_flag()->zero = (registers->get_a() == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = lsb;
}

void Instruction::rla_inst()
{
    bool carry_out = registers->get_flag()->carry;
    bool msb = (registers->get_a() & 0x80) != 0;

    u8 a = (registers->get_a() << 1) | (carry_out ? 0x01 : 0x00);
    registers->set_a(a);

    registers->get_flag()->zero = (registers->get_a() == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = msb;
}

void Instruction::rrca_inst()
{
    bool lsb = (registers->get_a() & 0x01) != 0;

    u8 a = (registers->get_a() >> 1) | (lsb ? 0x80 : 0x00);
    registers->set_a(a);

    registers->get_flag()->zero = (registers->get_a() == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = lsb;
}

void Instruction::rlca_inst()
{
    bool msb = (registers->get_a() & 0x80) != 0;

    u8 a = (registers->get_a() << 1) | (msb ? 0x01 : 0x00);
    registers->set_a(a);

    registers->get_flag()->zero = (registers->get_a() == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = msb;
}

void Instruction::cpl_inst()
{
    u8 a = ~registers->get_a();
    registers->set_a(a);

    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::bit_inst(u8 bit, u8 value)
{
    // cout << "Testing bit " << static_cast<int>(bit) << " of value: 0x" 
    //      << hex << static_cast<int>(value) << dec << endl;
    bool is_set = (value & (1 << bit)) != 0;

    registers->get_flag()->zero = !is_set;
    // cout << "Is bit " << static_cast<int>(bit) << " set? " << boolalpha << is_set << endl;
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = true;
    // cout << "Zero flag after BIT: " << boolalpha << registers->get_flag()->zero << endl;
}

void Instruction::res_inst(u8 bit, u8 value)
{
    value &= ~(1 << bit);
}

void Instruction::set_inst(u8 bit, u8 value)
{
    value |= (1 << bit);
}

void Instruction::srl_inst(u8 value)
{
    registers->get_flag()->carry = (value & 0x01);
    value >>= 1;

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::rr_inst(u8 value)
{
    bool old_carry = registers->get_flag()->carry;
    registers->get_flag()->carry = (value & 0x01);

    value = (value >> 1) | (old_carry << 1);

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::rl_inst(u8 value)
{
    bool old_carry = registers->get_flag()->carry;
    registers->get_flag()->carry = ((value & 0x80) != 0);

    value = (value << 1) | old_carry;

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::rrc_inst(u8 value)
{
    bool lsb = (value & 0x01);
    registers->get_flag()->carry = lsb;

    value = (value >> 1) | (lsb << 7);

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::rlc_inst(u8 value)
{
    bool msb = (value & 0x80);
    registers->get_flag()->carry = msb;

    value = (value << 1) | (msb >> 7);

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::sra_inst(u8 value)
{
    bool lsb = (value & 0x01);
    bool msb = (value & 0x80);

    registers->get_flag()->carry = lsb;
    value = (value >> 1) | msb;

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::sla_inst(u8 value)
{
    bool msb = (value & 0x80);

    registers->get_flag()->carry = msb;
    value <<= 1;

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
}

void Instruction::swap_inst(u8 value)
{
    value = (value >> 4) | (value << 4);

    registers->get_flag()->zero = (value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = false;
    registers->get_flag()->carry = false;
}

u16 Instruction::addhl_inst(u16 value)
{
    u32 result = static_cast<u32>(registers->get_HL()) + value;
    u16 new_value = static_cast<u16>(result);

    registers->get_flag()->zero = (new_value == 0);
    registers->get_flag()->subtract = false;
    registers->get_flag()->half_carry = (result > 0xFF);
    registers->get_flag()->carry = ((registers->get_a() & 0xF) + (value & 0xF) > 0xF);

    return new_value;
}

bool Instruction::check_jump_condition(JumpCondition jump)
{
    // cout << "Checking jump condition: " << static_cast<int>(jump) << endl;
    // cout << "Flags - Zero: " << boolalpha << registers->get_flag()->zero
    //      << ", Carry: " << registers->get_flag()->carry << endl;
    switch (jump)
    {
        case JumpCondition::NotZero:
            // cout << "Condition: NotZero, Zero flag is " << boolalpha << registers->get_flag()->zero << endl;
            return !registers->get_flag()->zero;

        case JumpCondition::Zero:
            // cout << "Condition: Zero, Zero flag is " << boolalpha << registers->get_flag()->zero << endl;
            return registers->get_flag()->zero;

        case JumpCondition::NotCarry:
            // cout << "Condition: NotCarry, Carry flag is " << boolalpha << registers->get_flag()->carry << endl;
            return !registers->get_flag()->carry;

        case JumpCondition::Carry:
            // cout << "Condition: Carry, Carry flag is " << boolalpha << registers->get_flag()->carry << endl;
            return registers->get_flag()->carry;

        case JumpCondition::Always:
            // cout << "Condition: Always, jumping unconditionally" << endl;
            return true;

        default:
            cout << "Unknown jump condition: " << static_cast<int>(jump) << endl;
            throw runtime_error("Unknown loadtype at check_jump_condition");
            break;
    }
}

void Instruction::jp_inst(bool should_jump)
{
    if (should_jump)
    {
        u16 lsb = static_cast<u16>(registers->get_bus()->read_byte(registers->get_PC() + 1));
        u16 msb = static_cast<u16>(registers->get_bus()->read_byte(registers->get_PC() + 2));
        u16 address = (msb << 8) | lsb;
        registers->set_PC(address);
    }
    else
    {
        registers->set_PC(registers->get_PC() + 2);
    }
}

void Instruction::jr_inst(bool should_jump)
{
    // cout << "PC before jump: 0x" << hex << static_cast<int>(registers->get_PC()) << endl;
    if (should_jump)
    {
        i8 offset = static_cast<i8>(registers->get_bus()->read_byte(registers->get_PC() + 1));
        registers->set_PC(registers->get_PC() + 1 + offset);
        // cout << "Offset: " << static_cast<int>(offset) << " Jumping: " << boolalpha << should_jump << endl;
    }
    else
    {
        registers->set_PC(registers->get_PC() + 1);
    }
}

void Instruction::jpi_inst()
{
    u16 address = (static_cast<u16>(registers->get_h()) << 8) | static_cast<u16>(registers->get_l());
    registers->set_PC(address);
    cycle_value += 1;
}

void Instruction::rst_inst()
{
    
}

void Instruction::ld_inst(const Instruction& instruction)
{
    switch (instruction.get_load_type())
    {
        case LoadType::Byte:
            byte_load(instruction);
            break;
    
        case LoadType::World:
            world_load(instruction);
            break;
        
        default:
            throw runtime_error("Unknown loadtype at ld_inst");
            break;
    }
}

void Instruction::byte_load(const Instruction& instruction)
{
    u8 value = get_8_source(instruction.get_load_source());

    switch (instruction.get_load_target())
    {
        case LoadTarget::A: registers->set_a(value); break;
        case LoadTarget::B: registers->set_b(value); break;
        case LoadTarget::C: registers->set_c(value); break;
        case LoadTarget::D: registers->set_d(value); break;
        case LoadTarget::E: registers->set_e(value); break;
        case LoadTarget::H: registers->set_h(value); break;
        case LoadTarget::L: registers->set_l(value); break;
        case LoadTarget::CI: {
            u16 address = 0xFF00 + registers->get_c();
            registers->get_bus()->write_byte(address, value);
            break;
        }
        case LoadTarget::A8: {
            u8 offset = registers->read_next_byte();
            u16 address = 0xFF00 + offset;
            registers->get_bus()->write_byte(address, offset);
            break;
        }
        case LoadTarget::BCI: registers->get_bus()->write_byte(registers->get_BC(), value); break;
        case LoadTarget::DEI: registers->get_bus()->write_byte(registers->get_DE(), value); break;
        case LoadTarget::HLI: registers->get_bus()->write_byte(registers->get_HL(), value); break;
        case LoadTarget::HLIUP: {
            registers->get_bus()->write_byte(registers->get_HL(), value);
            registers->set_HL(registers->get_HL() + 1);
            break;
        }
        case LoadTarget::HLILOW: {
            cout << "[DEBUG] Writing to address HL_LOW: " << hex << registers->get_HL() 
                 << " Value: " << static_cast<int>(value) << endl;

            cout << "[DEBUG] HL before decrement: " << hex << registers->get_HL() 
                 << " H: " << static_cast<int>(registers->get_h()) 
                 << " L: " << static_cast<int>(registers->get_l()) << endl;

            registers->get_bus()->write_byte(registers->get_HL(), value);
            registers->set_HL(registers->get_HL() - 1);
            // if  (registers->get_l() == 0x00)
            // {
            //     registers->set_l(0xFF);
            //     registers->set_h(registers->get_h() - 1);
            // }
            // else
            // {
            //     registers->set_l(registers->get_l() - 1);
            // }

            cout << "[DEBUG] HL after decrement: " << hex << registers->get_HL() 
                 << " H: " << static_cast<int>(registers->get_h()) 
                 << " L: " << static_cast<int>(registers->get_l()) << endl;

            cout << "[DEBUG] Incrementing HL_LOW to: " << hex << registers->get_HL() << endl;
            break;
        }
        case LoadTarget::A16: {
            u8 lower_byte = registers->read_next_byte();
            u8 upper_byte = registers->read_next_byte();
            u16 address = static_cast<u16>(lower_byte | (upper_byte << 8));

            registers->get_bus()->write_byte(address, value & 0xFF);
            registers->get_bus()->write_byte(address + 1, (value >> 8) & 0xFF);
            break;
        }
        default: throw runtime_error("Unknown target at byte_load");
    }
}

void Instruction::world_load(const Instruction& instruction)
{
    u16 value = get_16_source(instruction.get_load_source());

    switch (instruction.get_load_target())
    {
        case LoadTarget::BC: registers->set_BC(value); break;
        case LoadTarget::DE: registers->set_DE(value); break;
        case LoadTarget::HL: 
            cout << "[DEBUG] Writing to address HL: " << hex << registers->get_HL() 
                 << " Value: " << static_cast<int>(value) << endl;
            registers->set_HL(value);
            break;
        case LoadTarget::SP: registers->set_SP(value); break;
        case LoadTarget::A16: {
            u8 lower_byte = registers->read_next_byte();
            u8 upper_byte = registers->read_next_byte();
            u16 address = static_cast<u16>(lower_byte | (upper_byte << 8));

            registers->get_bus()->write_byte(address, value & 0xFF);
            registers->get_bus()->write_byte(address + 1, (value >> 8) & 0xFF);
            break;
        }
        default: throw runtime_error("Unknown target at world_load");
    }
}

u8 Instruction::get_8_source(LoadSource source)
{
    switch (source)
    {
        case LoadSource::A: return registers->get_a();
        case LoadSource::B: return registers->get_b();
        case LoadSource::C: return registers->get_c();
        case LoadSource::D: return registers->get_d();
        case LoadSource::E: return registers->get_e();
        case LoadSource::H: return registers->get_h();
        case LoadSource::L: return registers->get_l();
        case LoadSource::N8: return registers->read_next_byte();
        case LoadSource::CI: {
            u16 address = 0xFF00 + registers->get_c();
            return registers->get_bus()->read_byte(address);
        }
        case LoadSource::A8: {
            u8 offset = registers->read_next_byte();
            u16 address = 0xFF00 + offset;
            return registers->get_bus()->read_byte(address);
        }
        case LoadSource::BCI: return registers->get_bus()->read_byte(registers->get_BC());
        case LoadSource::DEI: return registers->get_bus()->read_byte(registers->get_DE());
        case LoadSource::HLI: return registers->get_bus()->read_byte(registers->get_HL());
        case LoadSource::HLILOW: {
            u8 value = registers->get_bus()->read_byte(registers->get_HL());
            registers->set_HL(registers->get_HL() - 1);
            return value;
        }
        case LoadSource::HLIUP: {
            u8 value = registers->get_bus()->read_byte(registers->get_HL());
            registers->set_HL(registers->get_HL() + 1);
            return value;
        }
        case LoadSource::A16: {
            u8 lower_byte = registers->read_next_byte();
            u8 upper_byte = registers->read_next_byte();
            u16 address = static_cast<u16>(lower_byte | (upper_byte << 8));
            return registers->get_bus()->read_byte(address);
        }
        default: throw runtime_error("Unknown source at get_8_source");
    }
}

u16 Instruction::get_16_source(LoadSource source)
{
    switch (source)
    {
        case LoadSource::HL: return registers->get_HL();
        case LoadSource::SP: return registers->get_SP();
        case LoadSource::N16: {
            u8 lower_byte = registers->read_next_byte();
            u8 upper_byte = registers->read_next_byte();
            cout << "N16 lower byte: " << hex << +lower_byte << endl;
            cout << "N16 upper byte: " << hex << +upper_byte << endl;
            return static_cast<u16>(lower_byte | (upper_byte << 8));
        }
        case LoadSource::SPs8: {
            i8 s8 = static_cast<i8>(registers->read_next_byte());
            cout << "Signed 8-bit offset: " << dec << +s8 << endl;
            return registers->get_SP() + s8;
        }
        default: throw runtime_error("Unknown source at get_16_source");
    }
}

const unordered_map<u8, Instruction> Instruction::instruction_map_not_prefixed = {
    //NOP
    {0x00, Instruction(InstructionType::NOP, 1)},

    //STOP
    {0x10, Instruction(InstructionType::STOP, 1)},

    //EI
    {0xF3, Instruction(InstructionType::EI, 1)},

    //DI
    {0xFB, Instruction(InstructionType::DI, 1)},    

    //JR
    {0x20, Instruction(InstructionType::JR, JumpCondition::NotZero)},
    {0x30, Instruction(InstructionType::JR, JumpCondition::NotCarry)},
    {0x18, Instruction(InstructionType::JR, JumpCondition::Always)},
    {0x28, Instruction(InstructionType::JR, JumpCondition::Zero)},
    {0x38, Instruction(InstructionType::JR, JumpCondition::Carry)},

    //JP
    {0xC2, Instruction(InstructionType::JP, JumpCondition::NotZero)},
    {0xD2, Instruction(InstructionType::JP, JumpCondition::NotCarry)},
    {0xC3, Instruction(InstructionType::JP, JumpCondition::Always)},
    {0xCA, Instruction(InstructionType::JP, JumpCondition::Zero)},
    {0xDA, Instruction(InstructionType::JP, JumpCondition::Carry)},

    //JPI
    {0xE9, Instruction(InstructionType::JPI, JumpCondition::Always)},

    //CALL
    {0xC4, Instruction(InstructionType::CALL, JumpCondition::NotZero)},
    {0xD4, Instruction(InstructionType::CALL, JumpCondition::NotCarry)},
    {0xCD, Instruction(InstructionType::CALL, JumpCondition::Always)},
    {0xCC, Instruction(InstructionType::CALL, JumpCondition::Zero)},
    {0xDC, Instruction(InstructionType::CALL, JumpCondition::Carry)},

    //PUSH
    {0xDF, Instruction(InstructionType::RST, 4)},
    {0xCF, Instruction(InstructionType::RST, 4)},
    {0xEF, Instruction(InstructionType::RST, 4)},
    {0xFF, Instruction(InstructionType::RST, 4)},

    //INC
    {0x03, Instruction(InstructionType::INC, ArithmeticTarget::BC, 2)},
    {0x13, Instruction(InstructionType::INC, ArithmeticTarget::DE, 2)},
    {0x23, Instruction(InstructionType::INC, ArithmeticTarget::HL, 2)},
    {0x33, Instruction(InstructionType::INC, ArithmeticTarget::SP, 2)},
    {0x04, Instruction(InstructionType::INC, ArithmeticTarget::B, 1)},
    {0x14, Instruction(InstructionType::INC, ArithmeticTarget::D, 1)},
    {0x24, Instruction(InstructionType::INC, ArithmeticTarget::H, 1)},
    {0x34, Instruction(InstructionType::INC, ArithmeticTarget::HLI, 3)},
    {0x0C, Instruction(InstructionType::INC, ArithmeticTarget::C, 1)},
    {0x1C, Instruction(InstructionType::INC, ArithmeticTarget::E, 1)},
    {0x2C, Instruction(InstructionType::INC, ArithmeticTarget::L, 1)},
    {0x3C, Instruction(InstructionType::INC, ArithmeticTarget::A, 1)},

    //DEC
    {0x05, Instruction(InstructionType::DEC, ArithmeticTarget::B, 1)},
    {0x15, Instruction(InstructionType::DEC, ArithmeticTarget::D, 1)},
    {0x25, Instruction(InstructionType::DEC, ArithmeticTarget::H, 1)},
    {0x35, Instruction(InstructionType::DEC, ArithmeticTarget::HLI, 3)},
    {0x0B, Instruction(InstructionType::DEC, ArithmeticTarget::BC, 2)},
    {0x1B, Instruction(InstructionType::DEC, ArithmeticTarget::DE, 2)},
    {0x2B, Instruction(InstructionType::DEC, ArithmeticTarget::HL, 2)},
    {0x3B, Instruction(InstructionType::DEC, ArithmeticTarget::SP, 2)},
    {0x0D, Instruction(InstructionType::DEC, ArithmeticTarget::C, 1)},
    {0x1D, Instruction(InstructionType::DEC, ArithmeticTarget::E, 1)},
    {0x2D, Instruction(InstructionType::DEC, ArithmeticTarget::L, 1)},
    {0x3D, Instruction(InstructionType::DEC, ArithmeticTarget::A, 1)},

    //RLCA
    {0x07, Instruction(InstructionType::RLCA, 1)},

    //RRCA
    {0x0F, Instruction(InstructionType::RRCA, 1)},

    //RLA
    {0x17, Instruction(InstructionType::RLA, 1)},

    //RRA
    {0x1F, Instruction(InstructionType::RRA, 1)},

    //CPL
    {0x2F, Instruction(InstructionType::CPL, 1)},

    //SCF
    {0x37, Instruction(InstructionType::SCF, 1)},

    //CCF
    {0x3F, Instruction(InstructionType::CCF, 1)},

    //ADDHL
    {0x09, Instruction(InstructionType::ADDHL, ArithmeticTarget::BC, 2)},
    {0x19, Instruction(InstructionType::ADDHL, ArithmeticTarget::DE, 2)},
    {0x29, Instruction(InstructionType::ADDHL, ArithmeticTarget::HL, 2)},
    {0x39, Instruction(InstructionType::ADDHL, ArithmeticTarget::SP, 2)},

    //LD
    {0x01, Instruction(InstructionType::LD, LoadType::World, LoadTarget::BC, LoadSource::N16, 3)},
    {0x11, Instruction(InstructionType::LD, LoadType::World, LoadTarget::DE, LoadSource::N16, 3)},
    {0x21, Instruction(InstructionType::LD, LoadType::World, LoadTarget::HL, LoadSource::N16, 3)},
    {0x31, Instruction(InstructionType::LD, LoadType::World, LoadTarget::SP, LoadSource::N16, 3)},
    {0x08, Instruction(InstructionType::LD, LoadType::World, LoadTarget::A16, LoadSource::SP, 5)},
    {0xF8, Instruction(InstructionType::LD, LoadType::World, LoadTarget::HL, LoadSource::SPs8, 3)},
    {0xF9, Instruction(InstructionType::LD, LoadType::World, LoadTarget::SP, LoadSource::HL, 2)},

    {0x02, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::BCI, LoadSource::A, 2)},
    {0x12, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::DEI, LoadSource::A, 2)},
    {0x22, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLIUP, LoadSource::A, 2)},
    {0x32, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLILOW, LoadSource::A, 2)},
    {0x06, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::N8, 2)},
    {0x16, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::N8, 2)},
    {0x26, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::N8, 2)},
    {0x36, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::N8, 3)},
    {0x0A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::BCI, 2)},
    {0x1A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::DEI, 2)},
    {0x2A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::HLIUP, 2)},
    {0x3A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::HLILOW, 2)},
    {0x0E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::N8, 2)},
    {0x1E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::N8, 2)},
    {0x2E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::N8, 2)},
    {0x3E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::N8, 2)},

    {0x40, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::B, 1)},
    {0x41, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::C, 1)},
    {0x42, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::D, 1)},
    {0x43, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::E, 1)},
    {0x44, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::H, 1)},
    {0x45, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::L, 1)},
    {0x46, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::HLI, 2)},
    {0x47, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::B, LoadSource::A, 1)},
    {0x48, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::B, 1)},
    {0x49, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::C, 1)},
    {0x4A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::D, 1)},
    {0x4B, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::E, 1)},
    {0x4C, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::H, 1)},
    {0x4D, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::L, 1)},
    {0x4E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::HLI, 2)},
    {0x4F, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::C, LoadSource::A, 1)},
    {0x50, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::B, 1)},
    {0x51, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::C, 1)},
    {0x52, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::D, 1)},
    {0x53, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::E, 1)},
    {0x54, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::H, 1)},
    {0x55, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::L, 1)},
    {0x56, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::HLI, 2)},
    {0x57, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::D, LoadSource::A, 1)},
    {0x58, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::B, 1)},
    {0x59, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::C, 1)},
    {0x5A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::D, 1)},
    {0x5B, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::E, 1)},
    {0x5C, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::H, 1)},
    {0x5D, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::L, 1)},
    {0x5E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::HLI, 2)},
    {0x5F, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::E, LoadSource::A, 1)},
    {0x60, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::B, 1)},
    {0x61, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::C, 1)},
    {0x62, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::D, 1)},
    {0x63, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::E, 1)},
    {0x64, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::H, 1)},
    {0x65, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::L, 1)},
    {0x66, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::HLI, 2)},
    {0x67, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::H, LoadSource::A, 1)},
    {0x68, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::B, 1)},
    {0x69, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::C, 1)},
    {0x6A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::D, 1)},
    {0x6B, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::E, 1)},
    {0x6C, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::H, 1)},
    {0x6D, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::L, 1)},
    {0x6E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::HLI, 2)},
    {0x6F, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::L, LoadSource::A, 1)},
    {0x70, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::B, 2)},
    {0x71, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::C, 2)},
    {0x72, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::D, 2)},
    {0x73, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::E, 2)},
    {0x74, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::H, 2)},
    {0x75, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::L, 2)},
    {0x77, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::HLI, LoadSource::A, 2)},
    {0x78, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::B, 1)},
    {0x79, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::C, 1)},
    {0x7A, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::D, 1)},
    {0x7B, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::E, 1)},
    {0x7C, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::H, 1)},
    {0x7D, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::L, 1)},
    {0x7E, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::HLI, 2)},
    {0x7F, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::A, 1)},

    {0xE0, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A8, LoadSource::A, 3)},
    {0xF0, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::A8, 3)},
    {0xE2, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::CI, LoadSource::A, 2)},
    {0xF2, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::CI, 2)},
    {0xEA, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A16, LoadSource::A, 4)},
    {0xFA, Instruction(InstructionType::LD, LoadType::Byte, LoadTarget::A, LoadSource::A16, 4)},

    //ADD
    {0x80, Instruction(InstructionType::ADD, ArithmeticTarget::B, 1)},
    {0x81, Instruction(InstructionType::ADD, ArithmeticTarget::C, 1)},
    {0x82, Instruction(InstructionType::ADD, ArithmeticTarget::D, 1)},
    {0x83, Instruction(InstructionType::ADD, ArithmeticTarget::E, 1)},
    {0x84, Instruction(InstructionType::ADD, ArithmeticTarget::H, 1)},
    {0x85, Instruction(InstructionType::ADD, ArithmeticTarget::L, 1)},
    {0x86, Instruction(InstructionType::ADD, ArithmeticTarget::HLI, 2)},
    {0x87, Instruction(InstructionType::ADD, ArithmeticTarget::A, 1)},

    //ADC
    {0x88, Instruction(InstructionType::ADC, ArithmeticTarget::B, 1)},
    {0x89, Instruction(InstructionType::ADC, ArithmeticTarget::C, 1)},
    {0x8A, Instruction(InstructionType::ADC, ArithmeticTarget::D, 1)},
    {0x8B, Instruction(InstructionType::ADC, ArithmeticTarget::E, 1)},
    {0x8C, Instruction(InstructionType::ADC, ArithmeticTarget::H, 1)},
    {0x8D, Instruction(InstructionType::ADC, ArithmeticTarget::L, 1)},
    {0x8E, Instruction(InstructionType::ADC, ArithmeticTarget::HLI, 2)},
    {0x8F, Instruction(InstructionType::ADC, ArithmeticTarget::A, 1)},

    //SUB
    {0x90, Instruction(InstructionType::SUB, ArithmeticTarget::B, 1)},
    {0x91, Instruction(InstructionType::SUB, ArithmeticTarget::C, 1)},
    {0x92, Instruction(InstructionType::SUB, ArithmeticTarget::D, 1)},
    {0x93, Instruction(InstructionType::SUB, ArithmeticTarget::E, 1)},
    {0x94, Instruction(InstructionType::SUB, ArithmeticTarget::H, 1)},
    {0x95, Instruction(InstructionType::SUB, ArithmeticTarget::L, 1)},
    {0x96, Instruction(InstructionType::SUB, ArithmeticTarget::HLI, 2)},
    {0x97, Instruction(InstructionType::SUB, ArithmeticTarget::A, 1)},

    //SBC
    {0x98, Instruction(InstructionType::SBC, ArithmeticTarget::B, 1)},
    {0x99, Instruction(InstructionType::SBC, ArithmeticTarget::C, 1)},
    {0x9A, Instruction(InstructionType::SBC, ArithmeticTarget::D, 1)},
    {0x9B, Instruction(InstructionType::SBC, ArithmeticTarget::E, 1)},
    {0x9C, Instruction(InstructionType::SBC, ArithmeticTarget::H, 1)},
    {0x9D, Instruction(InstructionType::SBC, ArithmeticTarget::L, 1)},
    {0x9E, Instruction(InstructionType::SBC, ArithmeticTarget::HLI, 2)},
    {0x9F, Instruction(InstructionType::SBC, ArithmeticTarget::A, 1)},

    //AND
    {0xA0, Instruction(InstructionType::AND, ArithmeticTarget::B, 1)},
    {0xA1, Instruction(InstructionType::AND, ArithmeticTarget::C, 1)},
    {0xA2, Instruction(InstructionType::AND, ArithmeticTarget::D, 1)},
    {0xA3, Instruction(InstructionType::AND, ArithmeticTarget::E, 1)},
    {0xA4, Instruction(InstructionType::AND, ArithmeticTarget::H, 1)},
    {0xA5, Instruction(InstructionType::AND, ArithmeticTarget::L, 1)},
    {0xA6, Instruction(InstructionType::AND, ArithmeticTarget::HLI, 2)},
    {0xA7, Instruction(InstructionType::AND, ArithmeticTarget::A, 1)},

    //XOR
    {0xA8, Instruction(InstructionType::XOR, ArithmeticTarget::B, 1)},
    {0xA9, Instruction(InstructionType::XOR, ArithmeticTarget::C, 1)},
    {0xAA, Instruction(InstructionType::XOR, ArithmeticTarget::D, 1)},
    {0xAB, Instruction(InstructionType::XOR, ArithmeticTarget::E, 1)},
    {0xAC, Instruction(InstructionType::XOR, ArithmeticTarget::H, 1)},
    {0xAD, Instruction(InstructionType::XOR, ArithmeticTarget::L, 1)},
    {0xAE, Instruction(InstructionType::XOR, ArithmeticTarget::HLI, 2)},
    {0xAF, Instruction(InstructionType::XOR, ArithmeticTarget::A, 1)},

    //OR
    {0xB0, Instruction(InstructionType::OR, ArithmeticTarget::B, 1)},
    {0xB1, Instruction(InstructionType::OR, ArithmeticTarget::C, 1)},
    {0xB2, Instruction(InstructionType::OR, ArithmeticTarget::D, 1)},
    {0xB3, Instruction(InstructionType::OR, ArithmeticTarget::E, 1)},
    {0xB4, Instruction(InstructionType::OR, ArithmeticTarget::H, 1)},
    {0xB5, Instruction(InstructionType::OR, ArithmeticTarget::L, 1)},
    {0xB6, Instruction(InstructionType::OR, ArithmeticTarget::HLI, 2)},
    {0xB7, Instruction(InstructionType::OR, ArithmeticTarget::A, 1)},

    //CP
    {0xB8, Instruction(InstructionType::CP, ArithmeticTarget::B, 1)},
    {0xB9, Instruction(InstructionType::CP, ArithmeticTarget::C, 1)},
    {0xBA, Instruction(InstructionType::CP, ArithmeticTarget::D, 1)},
    {0xBB, Instruction(InstructionType::CP, ArithmeticTarget::E, 1)},
    {0xBC, Instruction(InstructionType::CP, ArithmeticTarget::H, 1)},
    {0xBD, Instruction(InstructionType::CP, ArithmeticTarget::L, 1)},
    {0xBE, Instruction(InstructionType::CP, ArithmeticTarget::HLI, 2)},
    {0xBF, Instruction(InstructionType::CP, ArithmeticTarget::A, 1)},
};

const unordered_map<u8, Instruction> Instruction::instruction_map_prefixed = {
    //RLC
    {0x00, Instruction(InstructionType::RLC, ArithmeticTarget::B, 2)},
    {0x01, Instruction(InstructionType::RLC, ArithmeticTarget::C, 2)},
    {0x02, Instruction(InstructionType::RLC, ArithmeticTarget::D, 2)},
    {0x03, Instruction(InstructionType::RLC, ArithmeticTarget::E, 2)},
    {0x04, Instruction(InstructionType::RLC, ArithmeticTarget::H, 2)},
    {0x05, Instruction(InstructionType::RLC, ArithmeticTarget::L, 2)},
    {0x06, Instruction(InstructionType::RLC, ArithmeticTarget::HLI, 4)},
    {0x07, Instruction(InstructionType::RLC, ArithmeticTarget::A, 2)},

    //RRC
    {0x08, Instruction(InstructionType::RLC, ArithmeticTarget::B, 2)},
    {0x09, Instruction(InstructionType::RLC, ArithmeticTarget::C, 2)},
    {0x0A, Instruction(InstructionType::RLC, ArithmeticTarget::D, 2)},
    {0x0B, Instruction(InstructionType::RLC, ArithmeticTarget::E, 2)},
    {0x0C, Instruction(InstructionType::RLC, ArithmeticTarget::H, 2)},
    {0x0D, Instruction(InstructionType::RLC, ArithmeticTarget::L, 2)},
    {0x0E, Instruction(InstructionType::RLC, ArithmeticTarget::HLI, 4)},
    {0x0F, Instruction(InstructionType::RLC, ArithmeticTarget::A, 2)},

    //RL
    {0x10, Instruction(InstructionType::RL, ArithmeticTarget::B, 2)},
    {0x11, Instruction(InstructionType::RL, ArithmeticTarget::C, 2)},
    {0x12, Instruction(InstructionType::RL, ArithmeticTarget::D, 2)},
    {0x13, Instruction(InstructionType::RL, ArithmeticTarget::E, 2)},
    {0x14, Instruction(InstructionType::RL, ArithmeticTarget::H, 2)},
    {0x15, Instruction(InstructionType::RL, ArithmeticTarget::L, 2)},
    {0x16, Instruction(InstructionType::RL, ArithmeticTarget::HLI, 4)},
    {0x17, Instruction(InstructionType::RL, ArithmeticTarget::A, 2)},
        
    //RR
    {0x18, Instruction(InstructionType::RR, ArithmeticTarget::B, 2)},
    {0x19, Instruction(InstructionType::RR, ArithmeticTarget::C, 2)},
    {0x1A, Instruction(InstructionType::RR, ArithmeticTarget::D, 2)},
    {0x1B, Instruction(InstructionType::RR, ArithmeticTarget::E, 2)},
    {0x1C, Instruction(InstructionType::RR, ArithmeticTarget::H, 2)},
    {0x1D, Instruction(InstructionType::RR, ArithmeticTarget::L, 2)},
    {0x1E, Instruction(InstructionType::RR, ArithmeticTarget::HLI, 4)},
    {0x1F, Instruction(InstructionType::RR, ArithmeticTarget::A, 4)},

    //SLA
    {0x20, Instruction(InstructionType::SLA, ArithmeticTarget::B, 2)},
    {0x21, Instruction(InstructionType::SLA, ArithmeticTarget::C, 2)},
    {0x22, Instruction(InstructionType::SLA, ArithmeticTarget::D, 2)},
    {0x23, Instruction(InstructionType::SLA, ArithmeticTarget::E, 2)},
    {0x24, Instruction(InstructionType::SLA, ArithmeticTarget::H, 2)},
    {0x25, Instruction(InstructionType::SLA, ArithmeticTarget::L, 2)},
    {0x26, Instruction(InstructionType::SLA, ArithmeticTarget::HLI, 4)},
    {0x27, Instruction(InstructionType::SLA, ArithmeticTarget::A, 2)},

    //SRA
    {0x28, Instruction(InstructionType::SRA, ArithmeticTarget::B, 2)},
    {0x29, Instruction(InstructionType::SRA, ArithmeticTarget::C, 2)},
    {0x2A, Instruction(InstructionType::SRA, ArithmeticTarget::D, 2)},
    {0x2B, Instruction(InstructionType::SRA, ArithmeticTarget::E, 2)},
    {0x2C, Instruction(InstructionType::SRA, ArithmeticTarget::H, 2)},
    {0x2D, Instruction(InstructionType::SRA, ArithmeticTarget::L, 2)},
    {0x2E, Instruction(InstructionType::SRA, ArithmeticTarget::HLI, 4)},
    {0x2F, Instruction(InstructionType::SRA, ArithmeticTarget::A, 2)},

    //SWAP
    {0x30, Instruction(InstructionType::SWAP, ArithmeticTarget::B, 2)},
    {0x31, Instruction(InstructionType::SWAP, ArithmeticTarget::C, 2)},
    {0x32, Instruction(InstructionType::SWAP, ArithmeticTarget::D, 2)},
    {0x33, Instruction(InstructionType::SWAP, ArithmeticTarget::E, 2)},
    {0x34, Instruction(InstructionType::SWAP, ArithmeticTarget::H, 2)},
    {0x35, Instruction(InstructionType::SWAP, ArithmeticTarget::L, 2)},
    {0x36, Instruction(InstructionType::SWAP, ArithmeticTarget::HLI, 4)},
    {0x37, Instruction(InstructionType::SWAP, ArithmeticTarget::A, 2)},

    //SRL
    {0x38, Instruction(InstructionType::SRL, ArithmeticTarget::B, 2)},
    {0x39, Instruction(InstructionType::SRL, ArithmeticTarget::C, 2)},
    {0x3A, Instruction(InstructionType::SRL, ArithmeticTarget::D, 2)},
    {0x3B, Instruction(InstructionType::SRL, ArithmeticTarget::E, 2)},
    {0x3C, Instruction(InstructionType::SRL, ArithmeticTarget::H, 2)},
    {0x3D, Instruction(InstructionType::SRL, ArithmeticTarget::L, 2)},
    {0x3E, Instruction(InstructionType::SRL, ArithmeticTarget::HLI, 4)},
    {0x3F, Instruction(InstructionType::SRL, ArithmeticTarget::A, 2)},

    //BIT
    {0x40, Instruction(InstructionType::BIT, ArithmeticTarget::B, 0, 2)},
    {0x41, Instruction(InstructionType::BIT, ArithmeticTarget::C, 0, 2)},
    {0x42, Instruction(InstructionType::BIT, ArithmeticTarget::D, 0, 2)},
    {0x43, Instruction(InstructionType::BIT, ArithmeticTarget::E, 0, 2)},
    {0x44, Instruction(InstructionType::BIT, ArithmeticTarget::H, 0, 2)},
    {0x45, Instruction(InstructionType::BIT, ArithmeticTarget::L, 0, 2)},
    {0x46, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 0, 3)},
    {0x47, Instruction(InstructionType::BIT, ArithmeticTarget::A, 0, 2)},
    {0x48, Instruction(InstructionType::BIT, ArithmeticTarget::B, 1, 2)},
    {0x49, Instruction(InstructionType::BIT, ArithmeticTarget::C, 1, 2)},
    {0x4A, Instruction(InstructionType::BIT, ArithmeticTarget::D, 1, 2)},
    {0x4B, Instruction(InstructionType::BIT, ArithmeticTarget::E, 1, 2)},
    {0x4C, Instruction(InstructionType::BIT, ArithmeticTarget::H, 1, 2)},
    {0x4D, Instruction(InstructionType::BIT, ArithmeticTarget::L, 1, 2)},
    {0x4E, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 1, 3)},
    {0x4F, Instruction(InstructionType::BIT, ArithmeticTarget::A, 1, 2)},
    {0x50, Instruction(InstructionType::BIT, ArithmeticTarget::B, 2, 2)},
    {0x51, Instruction(InstructionType::BIT, ArithmeticTarget::C, 2, 2)},
    {0x52, Instruction(InstructionType::BIT, ArithmeticTarget::D, 2, 2)},
    {0x53, Instruction(InstructionType::BIT, ArithmeticTarget::E, 2, 2)},
    {0x54, Instruction(InstructionType::BIT, ArithmeticTarget::H, 2, 2)},
    {0x55, Instruction(InstructionType::BIT, ArithmeticTarget::L, 2, 2)},
    {0x56, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 2, 3)},
    {0x57, Instruction(InstructionType::BIT, ArithmeticTarget::A, 2, 2)},
    {0x58, Instruction(InstructionType::BIT, ArithmeticTarget::B, 3, 2)},
    {0x59, Instruction(InstructionType::BIT, ArithmeticTarget::C, 3, 2)},
    {0x5A, Instruction(InstructionType::BIT, ArithmeticTarget::D, 3, 2)},
    {0x5B, Instruction(InstructionType::BIT, ArithmeticTarget::E, 3, 2)},
    {0x5C, Instruction(InstructionType::BIT, ArithmeticTarget::H, 3, 2)},
    {0x5D, Instruction(InstructionType::BIT, ArithmeticTarget::L, 3, 2)},
    {0x5E, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 3, 3)},
    {0x5F, Instruction(InstructionType::BIT, ArithmeticTarget::A, 3, 2)},
    {0x60, Instruction(InstructionType::BIT, ArithmeticTarget::B, 4, 2)},
    {0x61, Instruction(InstructionType::BIT, ArithmeticTarget::C, 4, 2)},
    {0x62, Instruction(InstructionType::BIT, ArithmeticTarget::D, 4, 2)},
    {0x63, Instruction(InstructionType::BIT, ArithmeticTarget::E, 4, 2)},
    {0x64, Instruction(InstructionType::BIT, ArithmeticTarget::H, 4, 2)},
    {0x65, Instruction(InstructionType::BIT, ArithmeticTarget::L, 4, 2)},
    {0x66, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 4, 3)},
    {0x67, Instruction(InstructionType::BIT, ArithmeticTarget::A, 4, 2)},
    {0x68, Instruction(InstructionType::BIT, ArithmeticTarget::B, 5, 2)},
    {0x69, Instruction(InstructionType::BIT, ArithmeticTarget::C, 5, 2)},
    {0x6A, Instruction(InstructionType::BIT, ArithmeticTarget::D, 5, 2)},
    {0x6B, Instruction(InstructionType::BIT, ArithmeticTarget::E, 5, 2)},
    {0x6C, Instruction(InstructionType::BIT, ArithmeticTarget::H, 5, 2)},
    {0x6D, Instruction(InstructionType::BIT, ArithmeticTarget::L, 5, 2)},
    {0x6E, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 5, 3)},
    {0x6F, Instruction(InstructionType::BIT, ArithmeticTarget::A, 5, 2)},
    {0x70, Instruction(InstructionType::BIT, ArithmeticTarget::B, 6, 2)},
    {0x71, Instruction(InstructionType::BIT, ArithmeticTarget::C, 6, 2)},
    {0x72, Instruction(InstructionType::BIT, ArithmeticTarget::D, 6, 2)},
    {0x73, Instruction(InstructionType::BIT, ArithmeticTarget::E, 6, 2)},
    {0x74, Instruction(InstructionType::BIT, ArithmeticTarget::H, 6, 2)},
    {0x75, Instruction(InstructionType::BIT, ArithmeticTarget::L, 6, 2)},
    {0x76, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 6, 3)},
    {0x77, Instruction(InstructionType::BIT, ArithmeticTarget::A, 6, 2)},
    {0x78, Instruction(InstructionType::BIT, ArithmeticTarget::B, 7, 2)},
    {0x79, Instruction(InstructionType::BIT, ArithmeticTarget::C, 7, 2)},
    {0x7A, Instruction(InstructionType::BIT, ArithmeticTarget::D, 7, 2)},
    {0x7B, Instruction(InstructionType::BIT, ArithmeticTarget::E, 7, 2)},
    {0x7C, Instruction(InstructionType::BIT, ArithmeticTarget::H, 7, 2)},
    {0x7D, Instruction(InstructionType::BIT, ArithmeticTarget::L, 7, 2)},
    {0x7E, Instruction(InstructionType::BIT, ArithmeticTarget::HLI, 7, 3)},
    {0x7F, Instruction(InstructionType::BIT, ArithmeticTarget::A, 7, 2)},

    //RES
    {0x80, Instruction(InstructionType::RES, ArithmeticTarget::B, 0, 2)},
    {0x81, Instruction(InstructionType::RES, ArithmeticTarget::C, 0, 2)},
    {0x82, Instruction(InstructionType::RES, ArithmeticTarget::D, 0, 2)},
    {0x83, Instruction(InstructionType::RES, ArithmeticTarget::E, 0, 2)},
    {0x84, Instruction(InstructionType::RES, ArithmeticTarget::H, 0, 2)},
    {0x85, Instruction(InstructionType::RES, ArithmeticTarget::L, 0, 2)},
    {0x86, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 0, 4)},
    {0x87, Instruction(InstructionType::RES, ArithmeticTarget::A, 0, 2)},
    {0x88, Instruction(InstructionType::RES, ArithmeticTarget::B, 1, 2)},
    {0x89, Instruction(InstructionType::RES, ArithmeticTarget::C, 1, 2)},
    {0x8A, Instruction(InstructionType::RES, ArithmeticTarget::D, 1, 2)},
    {0x8B, Instruction(InstructionType::RES, ArithmeticTarget::E, 1, 2)},
    {0x8C, Instruction(InstructionType::RES, ArithmeticTarget::H, 1, 2)},
    {0x8D, Instruction(InstructionType::RES, ArithmeticTarget::L, 1, 2)},
    {0x8E, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 1, 4)},
    {0x8F, Instruction(InstructionType::RES, ArithmeticTarget::A, 1, 2)},
    {0x90, Instruction(InstructionType::RES, ArithmeticTarget::B, 2, 2)},
    {0x91, Instruction(InstructionType::RES, ArithmeticTarget::C, 2, 2)},
    {0x92, Instruction(InstructionType::RES, ArithmeticTarget::D, 2, 2)},
    {0x93, Instruction(InstructionType::RES, ArithmeticTarget::E, 2, 2)},
    {0x94, Instruction(InstructionType::RES, ArithmeticTarget::H, 2, 2)},
    {0x95, Instruction(InstructionType::RES, ArithmeticTarget::L, 2, 2)},
    {0x96, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 2, 4)},
    {0x97, Instruction(InstructionType::RES, ArithmeticTarget::A, 2, 2)},
    {0x98, Instruction(InstructionType::RES, ArithmeticTarget::B, 3, 2)},
    {0x99, Instruction(InstructionType::RES, ArithmeticTarget::C, 3, 2)},
    {0x9A, Instruction(InstructionType::RES, ArithmeticTarget::D, 3, 2)},
    {0x9B, Instruction(InstructionType::RES, ArithmeticTarget::E, 3, 2)},
    {0x9C, Instruction(InstructionType::RES, ArithmeticTarget::H, 3, 2)},
    {0x9D, Instruction(InstructionType::RES, ArithmeticTarget::L, 3, 2)},
    {0x9E, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 3, 4)},
    {0x9F, Instruction(InstructionType::RES, ArithmeticTarget::A, 3, 2)},
    {0xA0, Instruction(InstructionType::RES, ArithmeticTarget::B, 4, 2)},
    {0xA1, Instruction(InstructionType::RES, ArithmeticTarget::C, 4, 2)},
    {0xA2, Instruction(InstructionType::RES, ArithmeticTarget::D, 4, 2)},
    {0xA3, Instruction(InstructionType::RES, ArithmeticTarget::E, 4, 2)},
    {0xA4, Instruction(InstructionType::RES, ArithmeticTarget::H, 4, 2)},
    {0xA5, Instruction(InstructionType::RES, ArithmeticTarget::L, 4, 2)},
    {0xA6, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 4, 4)},
    {0xA7, Instruction(InstructionType::RES, ArithmeticTarget::A, 4, 2)},
    {0xA8, Instruction(InstructionType::RES, ArithmeticTarget::B, 5, 2)},
    {0xA9, Instruction(InstructionType::RES, ArithmeticTarget::C, 5, 2)},
    {0xAA, Instruction(InstructionType::RES, ArithmeticTarget::D, 5, 2)},
    {0xAB, Instruction(InstructionType::RES, ArithmeticTarget::E, 5, 2)},
    {0xAC, Instruction(InstructionType::RES, ArithmeticTarget::H, 5, 2)},
    {0xAD, Instruction(InstructionType::RES, ArithmeticTarget::L, 5, 2)},
    {0xAE, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 5, 4)},
    {0xAF, Instruction(InstructionType::RES, ArithmeticTarget::A, 5, 2)},
    {0xB0, Instruction(InstructionType::RES, ArithmeticTarget::B, 6, 2)},
    {0xB1, Instruction(InstructionType::RES, ArithmeticTarget::C, 6, 2)},
    {0xB2, Instruction(InstructionType::RES, ArithmeticTarget::D, 6, 2)},
    {0xB3, Instruction(InstructionType::RES, ArithmeticTarget::E, 6, 2)},
    {0xB4, Instruction(InstructionType::RES, ArithmeticTarget::H, 6, 2)},
    {0xB5, Instruction(InstructionType::RES, ArithmeticTarget::L, 6, 2)},
    {0xB6, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 6, 4)},
    {0xB7, Instruction(InstructionType::RES, ArithmeticTarget::A, 6, 2)},
    {0xB8, Instruction(InstructionType::RES, ArithmeticTarget::B, 7, 2)},
    {0xB9, Instruction(InstructionType::RES, ArithmeticTarget::C, 7, 2)},
    {0xBA, Instruction(InstructionType::RES, ArithmeticTarget::D, 7, 2)},
    {0xBB, Instruction(InstructionType::RES, ArithmeticTarget::E, 7, 2)},
    {0xBC, Instruction(InstructionType::RES, ArithmeticTarget::H, 7, 2)},
    {0xBD, Instruction(InstructionType::RES, ArithmeticTarget::L, 7, 2)},
    {0xBE, Instruction(InstructionType::RES, ArithmeticTarget::HLI, 7, 4)},
    {0xBF, Instruction(InstructionType::RES, ArithmeticTarget::A, 7, 2)},

    //SET
    {0xC0, Instruction(InstructionType::SET, ArithmeticTarget::B, 0, 2)},
    {0xC1, Instruction(InstructionType::SET, ArithmeticTarget::C, 0, 2)},
    {0xC2, Instruction(InstructionType::SET, ArithmeticTarget::D, 0, 2)},
    {0xC3, Instruction(InstructionType::SET, ArithmeticTarget::E, 0, 2)},
    {0xC4, Instruction(InstructionType::SET, ArithmeticTarget::H, 0, 2)},
    {0xC5, Instruction(InstructionType::SET, ArithmeticTarget::L, 0, 2)},
    {0xC6, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 0, 4)},
    {0xC7, Instruction(InstructionType::SET, ArithmeticTarget::A, 0, 2)},
    {0xC8, Instruction(InstructionType::SET, ArithmeticTarget::B, 1, 2)},
    {0xC9, Instruction(InstructionType::SET, ArithmeticTarget::C, 1, 2)},
    {0xCA, Instruction(InstructionType::SET, ArithmeticTarget::D, 1, 2)},
    {0xCB, Instruction(InstructionType::SET, ArithmeticTarget::E, 1, 2)},
    {0xCC, Instruction(InstructionType::SET, ArithmeticTarget::H, 1, 2)},
    {0xCD, Instruction(InstructionType::SET, ArithmeticTarget::L, 1, 2)},
    {0xCE, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 1, 4)},
    {0xCF, Instruction(InstructionType::SET, ArithmeticTarget::A, 1, 2)},
    {0xD0, Instruction(InstructionType::SET, ArithmeticTarget::B, 2, 2)},
    {0xD1, Instruction(InstructionType::SET, ArithmeticTarget::C, 2, 2)},
    {0xD2, Instruction(InstructionType::SET, ArithmeticTarget::D, 2, 2)},
    {0xD3, Instruction(InstructionType::SET, ArithmeticTarget::E, 2, 2)},
    {0xD4, Instruction(InstructionType::SET, ArithmeticTarget::H, 2, 2)},
    {0xD5, Instruction(InstructionType::SET, ArithmeticTarget::L, 2, 2)},
    {0xD6, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 2, 4)},
    {0xD7, Instruction(InstructionType::SET, ArithmeticTarget::A, 2, 2)},
    {0xD8, Instruction(InstructionType::SET, ArithmeticTarget::B, 3, 2)},
    {0xD9, Instruction(InstructionType::SET, ArithmeticTarget::C, 3, 2)},
    {0xDA, Instruction(InstructionType::SET, ArithmeticTarget::D, 3, 2)},
    {0xDB, Instruction(InstructionType::SET, ArithmeticTarget::E, 3, 2)},
    {0xDC, Instruction(InstructionType::SET, ArithmeticTarget::H, 3, 2)},
    {0xDD, Instruction(InstructionType::SET, ArithmeticTarget::L, 3, 2)},
    {0xDE, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 3, 4)},
    {0xDF, Instruction(InstructionType::SET, ArithmeticTarget::A, 3, 2)},
    {0xE0, Instruction(InstructionType::SET, ArithmeticTarget::B, 4, 2)},
    {0xE1, Instruction(InstructionType::SET, ArithmeticTarget::C, 4, 2)},
    {0xE2, Instruction(InstructionType::SET, ArithmeticTarget::D, 4, 2)},
    {0xE3, Instruction(InstructionType::SET, ArithmeticTarget::E, 4, 2)},
    {0xE4, Instruction(InstructionType::SET, ArithmeticTarget::H, 4, 2)},
    {0xE5, Instruction(InstructionType::SET, ArithmeticTarget::L, 4, 2)},
    {0xE6, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 4, 4)},
    {0xE7, Instruction(InstructionType::SET, ArithmeticTarget::A, 4, 2)},
    {0xE8, Instruction(InstructionType::SET, ArithmeticTarget::B, 5, 2)},
    {0xE9, Instruction(InstructionType::SET, ArithmeticTarget::C, 5, 2)},
    {0xEA, Instruction(InstructionType::SET, ArithmeticTarget::D, 5, 2)},
    {0xEB, Instruction(InstructionType::SET, ArithmeticTarget::E, 5, 2)},
    {0xEC, Instruction(InstructionType::SET, ArithmeticTarget::H, 5, 2)},
    {0xED, Instruction(InstructionType::SET, ArithmeticTarget::L, 5, 2)},
    {0xEE, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 5, 4)},
    {0xEF, Instruction(InstructionType::SET, ArithmeticTarget::A, 5, 2)},
    {0xF0, Instruction(InstructionType::SET, ArithmeticTarget::B, 6, 2)},
    {0xF1, Instruction(InstructionType::SET, ArithmeticTarget::C, 6, 2)},
    {0xF2, Instruction(InstructionType::SET, ArithmeticTarget::D, 6, 2)},
    {0xF3, Instruction(InstructionType::SET, ArithmeticTarget::E, 6, 2)},
    {0xF4, Instruction(InstructionType::SET, ArithmeticTarget::H, 6, 2)},
    {0xF5, Instruction(InstructionType::SET, ArithmeticTarget::L, 6, 2)},
    {0xF6, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 6, 4)},
    {0xF7, Instruction(InstructionType::SET, ArithmeticTarget::A, 6, 2)},
    {0xF8, Instruction(InstructionType::SET, ArithmeticTarget::B, 7, 2)},
    {0xF9, Instruction(InstructionType::SET, ArithmeticTarget::C, 7, 2)},
    {0xFA, Instruction(InstructionType::SET, ArithmeticTarget::D, 7, 2)},
    {0xFB, Instruction(InstructionType::SET, ArithmeticTarget::E, 7, 2)},
    {0xFC, Instruction(InstructionType::SET, ArithmeticTarget::H, 7, 2)},
    {0xFD, Instruction(InstructionType::SET, ArithmeticTarget::L, 7, 2)},
    {0xFE, Instruction(InstructionType::SET, ArithmeticTarget::HLI, 7, 4)},
    {0xFF, Instruction(InstructionType::SET, ArithmeticTarget::A, 7, 2)},
};

optional<Instruction> Instruction::from_byte(u8 byte, bool prefixed)
{
    if (prefixed)
    {
        return from_byte_prefixed(byte);
    }
    else
    {
        return from_byte_not_prefixed(byte);
    }
}

optional<Instruction> Instruction::from_byte_not_prefixed(u8 byte)
{
    auto it = instruction_map_not_prefixed.find(byte);
    if (it != instruction_map_not_prefixed.end())
    {
        return it->second;
    }
    return nullopt;
}

optional<Instruction> Instruction::from_byte_prefixed(u8 byte)
{
    auto it = instruction_map_prefixed.find(byte);
    if (it != instruction_map_prefixed.end())
    {
        return it->second;
    }
    return nullopt;
}

// std::optional<Instruction> Instruction::from_byte_prefixed(u8 byte) {
//     static const array<optional<Instruction>, 256> lookup_table = [] {
//         array<optional<Instruction>, 256> table{};
        
//         for (int i = 0; i < 8; ++i)
//         {
//             table[0x00 + i] = Instruction(InstructionType::RLC, static_cast<ArithmeticTarget>(i));
//             table[0x08 + i] = Instruction(InstructionType::RRC, static_cast<ArithmeticTarget>(i));
//             table[0x10 + i] = Instruction(InstructionType::RL, static_cast<ArithmeticTarget>(i));
//             table[0x18 + i] = Instruction(InstructionType::RR, static_cast<ArithmeticTarget>(i));
//             table[0x20 + i] = Instruction(InstructionType::SLA, static_cast<ArithmeticTarget>(i));
//             table[0x28 + i] = Instruction(InstructionType::SRA, static_cast<ArithmeticTarget>(i));
//             table[0x30 + i] = Instruction(InstructionType::SWAP, static_cast<ArithmeticTarget>(i));
//             table[0x38 + i] = Instruction(InstructionType::SRL, static_cast<ArithmeticTarget>(i));
//             for (int j = 0; j < 8; ++j)
//             {
//                 table[0x40 + i * 8 + j] = Instruction(InstructionType::BIT, static_cast<ArithmeticTarget>(i), j);
//                 table[0x80 + i * 8 + j] = Instruction(InstructionType::RES, static_cast<ArithmeticTarget>(i), j);
//                 table[0xC0 + i * 8 + j] = Instruction(InstructionType::SET, static_cast<ArithmeticTarget>(i), j);
//             }
//         }
//         return table;
//     }();

//     if (byte < lookup_table.size())
//     {
//         return lookup_table[byte];
//     }
//     else
//     {
//         return nullopt;
//     }
// }