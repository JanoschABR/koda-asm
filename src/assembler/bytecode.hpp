#pragma once
#include "../shared.hpp"

/// The size of an instruction, in bytes
#define INSTR_FULL_SIZE 24

/// The size of the instruction address, in bytes
#define INSTR_ADDR_SIZE 2

/// The size of the instruction body, in bytes
#define INSTR_DATA_SIZE (INSTR_FULL_SIZE - INSTR_ADDR_SIZE)

struct bytecode_instruction {
    ushort code = 0;
    array<byte, INSTR_DATA_SIZE> data = array<byte, INSTR_DATA_SIZE>();

    bytecode_instruction() = default;
    explicit bytecode_instruction(ushort _code) {
        this->code = _code;
    }
};