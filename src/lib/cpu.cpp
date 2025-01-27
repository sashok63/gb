#include "cpu.hpp"

CPU::CPU(Registers *regs_ptr, Instruction *inst_ptr, PPU *ppu_ptr)
    : registers(regs_ptr), inst(inst_ptr), ppu(ppu_ptr)
{
    if (!registers || !inst || !ppu)
    {
        throw runtime_error("Null pointer provided to CPU constructor");
    }

    div = &registers->get_bus()->get_memory(0xFF04);
    tima = &registers->get_bus()->get_memory(0xFF05);
    tma = &registers->get_bus()->get_memory(0xFF06);
    tac = &registers->get_bus()->get_memory(0xFF07);

    registers->get_bus()->load_boot_dmg();
    registers->set_PC(0x0000);
    // load_cpu_without_bootdmg();
    // registers->get_bus()->load_test();
};

auto CPU::load_cpu_without_bootdmg() -> void
{
    registers->set_a(0x01);
    registers->set_f(0xB0);
    registers->set_b(0x00);
    registers->set_c(0x13);
    registers->set_d(0x00);
    registers->set_e(0xD8);
    registers->set_h(0x01);
    registers->set_l(0x4D);

    registers->set_PC(0x0100);
    registers->set_SP(0xFFFE);

    registers->get_flag()->zero = true;
    registers->get_flag()->carry = true;
    registers->get_flag()->half_carry = true;
    registers->get_flag()->subtract = false;

    registers->set_IME(1);

    registers->get_bus()->write_byte(0xFF0F, 0xE1);
    registers->get_bus()->write_byte(0xFF10, 0x80);
	registers->get_bus()->write_byte(0xFF11, 0xBF);
	registers->get_bus()->write_byte(0xFF12, 0xF3);
	registers->get_bus()->write_byte(0xFF14, 0xBF);
	registers->get_bus()->write_byte(0xFF16, 0x3F);
	registers->get_bus()->write_byte(0xFF17, 0x00);
	registers->get_bus()->write_byte(0xFF19, 0xBF);
	registers->get_bus()->write_byte(0xFF1A, 0x7A);
	registers->get_bus()->write_byte(0xFF1B, 0xFF);
	registers->get_bus()->write_byte(0xFF1C, 0x9F);
	registers->get_bus()->write_byte(0xFF1E, 0xBF);
	registers->get_bus()->write_byte(0xFF20, 0xFF);
	registers->get_bus()->write_byte(0xFF23, 0xBF);
	registers->get_bus()->write_byte(0xFF24, 0x77);
	registers->get_bus()->write_byte(0xFF25, 0xF3);
	registers->get_bus()->write_byte(0xFF26, 0xF1);
	registers->get_bus()->write_byte(0xFF40, 0x91);
    registers->get_bus()->write_byte(0xFF41, 0x80);
	registers->get_bus()->write_byte(0xFF47, 0xFC);
	registers->get_bus()->write_byte(0xFF48, 0xFF);
	registers->get_bus()->write_byte(0xFF49, 0xFF);

    // Add timers
}

auto CPU::execute(const Instruction &instruction) -> u8
{
    bool jump_condition;
    u8 cycle;

    auto arithmetic_op = [&](auto get_reg, auto set_reg, auto func) -> void
    {
        u8 reg_value = get_reg();
        reg_value = func(reg_value);
        set_reg(reg_value);
    };

    auto push_inst = [&](u16 value) -> void
    {
        registers->set_SP(registers->get_SP() - 2);

        registers->get_bus()->write_byte(registers->get_SP(), static_cast<u8>(value & 0x00FF));
        registers->get_bus()->write_byte(registers->get_SP() + 1, static_cast<u8>((value & 0xFF00) >> 8));
    };

    auto pop_inst = [&]() -> u16
    {
        u16 value = static_cast<u16>(registers->get_bus()->read_byte(registers->get_SP())) |
                    (static_cast<u16>(registers->get_bus()->read_byte(registers->get_SP() + 1)) << 8);
        registers->set_SP(registers->get_SP() + 2);
        return value;
    };

    switch (instruction.get_inst_type())
    {
    case InstructionType::ADD:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->add_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::ADDHL:
        arithmetic_op(
            [&]()
            { return registers->get_HL(); }, // Getter
            [&](u16 value)
            { registers->set_HL(value); }, // Setter
            [this](u16 value)
            { return inst->addhl_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::ADC:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->adc_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::SUB:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->sub_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::SBC:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->sbc_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::AND:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->and_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::OR:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->or_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::XOR:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_a(value); }, // Setter
            [this](u8 value)
            { return inst->xor_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::CP:
        inst->cp_inst(registers->get_register(instruction.get_arithmetic_target()));
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::INC:
        if (instruction_byte == 0x03 || instruction_byte == 0x13 ||
            instruction_byte == 0x23 || instruction_byte == 0x33)
        {
            switch (instruction_byte)
            {
            case 0x03:
                registers->set_BC(registers->get_BC() + 1);
                break;

            case 0x13:
                registers->set_DE(registers->get_DE() + 1);
                break;

            case 0x23:
                registers->set_HL(registers->get_HL() + 1);
                break;

            case 0x33:
                registers->set_SP(registers->get_SP() + 1);
                break;

            default:
                throw runtime_error("Unkown case in INC(CPU): " + to_string(static_cast<int>(instruction_byte)));
            }
        }
        else
        {
            arithmetic_op(
                [&]()
                { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
                [&](u8 value)
                { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
                [this](u8 value)
                { return inst->inc_inst(value); } // Arithmetic operation
            );
        }
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::DEC:
        if (instruction_byte == 0x0B || instruction_byte == 0x1B ||
            instruction_byte == 0x2B || instruction_byte == 0x3B)
        {
            switch (instruction_byte)
            {
            case 0x0B:
                registers->set_BC(registers->get_BC() - 1);
                break;

            case 0x1B:
                registers->set_DE(registers->get_DE() - 1);
                break;

            case 0x2B:
                registers->set_HL(registers->get_HL() - 1);
                break;

            case 0x3B:
                registers->set_SP(registers->get_SP() - 1);
                break;

            default:
                throw runtime_error("Unkown case in DEC(CPU): " + to_string(static_cast<int>(instruction_byte)));
            }
        }
        else
        {
            arithmetic_op(
                [&]()
                { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
                [&](u8 value)
                { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
                [this](u8 value)
                { return inst->dec_inst(value); } // Arithmetic operation
            );
        }
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::JP:
        jump_condition = inst->check_jump_condition(instruction.get_jump_condition());
        inst->jp_inst(jump_condition);
        cycle = jump_condition ? 4 : 3;
        return cycle;

    case InstructionType::JR:
        jump_condition = inst->check_jump_condition(instruction.get_jump_condition());
        inst->jr_inst(jump_condition);
        cycle = jump_condition ? 3 : 2;
        return cycle;

    case InstructionType::JPI:
        inst->jpi_inst();
        cycle = 1;
        return cycle;

    case InstructionType::CCF:
        inst->ccf_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::SCF:
        inst->scf_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RRA:
        inst->rra_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RLA:
        inst->rla_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RRCA:
        inst->rrca_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RLCA:
        inst->rlca_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::CPL:
        inst->cpl_inst();
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::BIT:
        inst->bit_inst(instruction.bit, registers->get_register(instruction.get_arithmetic_target()));
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RES:
    {
        u8 bit = instruction.bit;
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this, bit](u8 value)
            { return inst->res_inst(bit, value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;
    }

    case InstructionType::SET:
    {
        u8 bit = instruction.bit;
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this, bit](u8 value)
            { return inst->set_inst(bit, value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;
    }

    case InstructionType::SRL:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->srl_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RR:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->rr_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RL:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->rl_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RRC:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->rrc_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RLC:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->rlc_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::SRA:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->sra_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::SLA:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->sla_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::SWAP:
        arithmetic_op(
            [&]()
            { return registers->get_register(instruction.get_arithmetic_target()); }, // Getter
            [&](u8 value)
            { registers->set_register(instruction.get_arithmetic_target(), value); }, // Setter
            [this](u8 value)
            { return inst->swap_inst(value); } // Arithmetic operation
        );
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::LD:
        inst->ld_inst(instruction);
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::PUSH:
        push_inst(registers->get_register_pair(instruction.get_arithmetic_target()));
        return cycle;

    case InstructionType::POP:
        registers->set_register_pair(instruction.get_arithmetic_target(), pop_inst());
        return cycle;

    case InstructionType::CALL:
    {
        jump_condition = inst->check_jump_condition(instruction.get_jump_condition());
        registers->set_PC(registers->get_PC() + 2);
        if (jump_condition)
        {
            u16 operand = registers->get_bus()->read_byte(registers->get_PC() - 1) |
                          (registers->get_bus()->read_byte(registers->get_PC() - 2) >> 8);
            push_inst(registers->get_PC());
            registers->set_PC(operand - 1); // Prevent inc in CPU Step
        }
        cycle = jump_condition ? 6 : 3;
        return cycle;
    }

    case InstructionType::RET:
        if (instruction_byte == 0xD9)
        {
            registers->set_IME(1);
        }

        jump_condition = inst->check_jump_condition(instruction.get_jump_condition());
        if (jump_condition)
        {
            registers->set_PC(pop_inst());
        }
        else
        {
            registers->set_PC(registers->get_PC() + 1);
        }

        if (instruction_byte == 0xC9 || instruction_byte == 0xD9)
        {
            cycle = 4;
        }
        else
        {
            cycle = jump_condition ? 5 : 2;
        }
        return cycle;

    case InstructionType::NOP:
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::HALT:
        // this->is_halted = true;
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::EI:
        registers->set_IME(1);
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::DI:
        registers->set_IME(0);
        cycle = instruction.get_cycle_value();
        return cycle;

    case InstructionType::RST:
    {
        u16 return_address = registers->get_PC() + 1;
        push_inst(return_address);
        switch (instruction_byte)
        {
        case 0xC7:
            registers->set_PC(0x0000);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xCF:
            registers->set_PC(0x0008);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xD7:
            registers->set_PC(0x00010);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xDF:
            registers->set_PC(0x0018);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xE7:
            registers->set_PC(0x0020);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xEF:
            registers->set_PC(0x0028);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xF7:
            registers->set_PC(0x0030);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        case 0xFF:
            registers->set_PC(0x0038);
            registers->set_PC(registers->get_PC() - 1); // Prevent inc in cpu step
            break;

        default:
            throw runtime_error("Unknown instruction_byte found at step: 0x" + to_string(instruction_byte));
            break;
        }
        cycle = instruction.get_cycle_value();
        return cycle;
    }

    default:
        cerr << "Unknown instruction at execute" << endl;
        cerr << "Instruction type: " << static_cast<u16>(instruction.get_inst_type()) << endl;
        cerr << "PC: " << hex << registers->get_PC() << dec << endl;
        cerr << "SP: " << hex << registers->get_SP() << dec << endl;
        throw runtime_error("Unknown instruction at execute");
        break;
    }
}

auto CPU::step() -> void
{
    u8 cycle = 0;
    instruction_byte = registers->get_bus()->read_byte(registers->get_PC());
    bool prefixed = (instruction_byte == 0xCB);

    if (prefixed)
    {
        instruction_byte = registers->get_bus()->read_byte(registers->get_PC() + 1);
    }

    const Instruction *inst = Instruction::from_byte(instruction_byte, prefixed);
    if (inst != nullptr)
    {
        // log_state("Before execute", instruction_byte, prefixed);
        cycle = execute(*inst);
        registers->set_PC(registers->get_PC() + (prefixed ? 2 : 1));
        if (prefixed)
        {
            cycle += 1;
        }
        log_state("After execute", instruction_byte, prefixed);
    }
    else
    {
        throw runtime_error("Unknown instruction found at step: 0x" + to_string(registers->get_PC()));
    }

    // Implement cycles in cpu(step)
    interrupts();
    if (interrupt_triggered)
    {
        cycle += 5; // Add 5 M-cycles per truggered interrupt
        interrupt_triggered = 0;
    }
    timer(cycle);     // Pass the M-cycle
    ppu->step(cycle); // Pass the M-cycle

    if (registers->get_PC() == 0x00FA)
    {
        cout << "Reached" << endl;
        exit(0);
    }
    // if (instruction_byte == 0x0027)
    // {
    //     exit(0);
    // }
    // if (registers->get_SP() == 0x002C)
    // {
    //     exit(0);
    // }
}

auto CPU::timer(u8 cycle) -> void
{
    // Update DIV register (increment every 64 M-cycles)
    *div += cycle;
    if (*div >= 64)
    {
        *div -= 63;
    }

    // Check if timer is enabled (TAC bit 2)
    if (*tac & 0x4)
    {
        *tima += cycle;

        u32 freq_lut[4] = {256, 4, 16, 64};
        u32 freq = freq_lut[*tac & 3];

        // Increment TIMA when enough cycles have passed
        while (*tima >= freq)
        {
            (*tima)++;

            // Check for overflow (TIMA == 0 after increment)
            if (*tima == 0)
            {
                // Trigger Timer Overflow interrupt
                registers->set_interrupt_flag(INTERRUPT_TIMER);
                // Reload TIMA from TMA
                *tima = *tma;
            }
            *tima -= freq; // Adjust remaining cycles
        }
    }
}

auto CPU::interrupts() -> void
{
    if (registers->get_bus()->read_byte(0xFFFF) & registers->get_bus()->read_byte(0xFF0F))
    {
        registers->set_is_halted(0);
    }

    if (registers->is_interrupt_enabled(INTERRUPT_VBANK) && registers->is_interrupt_flag_set(INTERRUPT_VBANK))
    {
        registers->trigger_interrupt(INTERRUPT_VBANK, 0x40);
        interrupt_triggered = 1;
    }

    if (registers->is_interrupt_enabled(INTERRUPT_LCD) && registers->is_interrupt_flag_set(INTERRUPT_LCD))
    {
        registers->trigger_interrupt(INTERRUPT_LCD, 0x48);
        interrupt_triggered = 1;
    }

    if (registers->is_interrupt_enabled(INTERRUPT_TIMER) && registers->is_interrupt_flag_set(INTERRUPT_TIMER))
    {
        registers->trigger_interrupt(INTERRUPT_TIMER, 0x50);
        interrupt_triggered = 1;
    }

    if (registers->is_interrupt_enabled(INTERRUPT_JOYPAD) && registers->is_interrupt_flag_set(INTERRUPT_JOYPAD))
    {
        registers->trigger_interrupt(INTERRUPT_JOYPAD, 0x60);
        interrupt_triggered = 1;
    }
}

auto CPU::log_state(const string &stage, u8 instruction_byte, bool prefixed) -> void
{
    static bool is_initialized = false;

    ofstream log_file;
    if (!is_initialized)
    {
        log_file.open("cpu_log.txt", ios::trunc);
        is_initialized = true;
    }
    else
    {
        log_file.open("cpu_log.txt", ios::app);
    }

    if (!log_file.is_open())
    {
        cerr << "Error opening log file!" << endl;
        return;
    }

    // Debug output
    log_file << "[" << stage << "] PC: 0x" << hex << setw(4) << setfill('0') << static_cast<u16>(registers->get_PC())
             << " | SP: 0x" << hex << setw(4) << setfill('0') << static_cast<u16>(registers->get_SP())
             << " | Instruction: 0x" << hex << setw(2) << setfill('0') << static_cast<u16>(instruction_byte)
             << " | Prefix: 0x" << hex << setw(1) << setfill('0') << static_cast<u16>(prefixed)
             << " | (" << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(registers->get_PC()))
             << " " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(registers->get_PC() + 1))
             << " " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(registers->get_PC() + 2))
             << " " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(registers->get_PC() + 3))
             << ")"
             << std::endl;

    // cout << "Memory write to: " << address << " | Value: " << value << endl;

    log_file << "Regs: A = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_a())
             << ", F = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_f())
             << ", B = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_b())
             << ", C = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_c())
             << ", D = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_d())
             << ", E = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_e())
             << ", H = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_h())
             << ", L = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_l())
             << " | Z: " << registers->get_flag()->zero
             << " | S: " << registers->get_flag()->subtract
             << " | H-C: " << registers->get_flag()->half_carry
             << " | C: " << registers->get_flag()->carry
             << endl;

    log_file << "Memory regs: LY = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(0xFF44))
             << ", LYC = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(0xFF45))
             << ", SCY = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(0xFF42))
             << ", SCX = " << hex << setw(2) << setfill('0') << static_cast<u16>(registers->get_bus()->read_byte(0xFF43))
             << ", LCDC = " << bitset<8>(registers->get_bus()->read_byte(0xFF40))
             << ", STAT = " << bitset<8>(registers->get_bus()->read_byte(0xFF41))
             << endl;

    log_file << "Timers: CPU = " << hex << setw(2) << setfill('0') << static_cast<u16>(*div)
             << ", GPU = " << hex << setw(2) << setfill('0') << static_cast<u16>(ppu->get_ppu_cycle())
             << endl;
    // Debug output

    log_file.close();
}