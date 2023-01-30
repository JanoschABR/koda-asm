#pragma once
#include "tokenizer.hpp"
#include <ctgmath>
#include "../utils.hpp"

namespace instructions {
#define instr_tokenizer_no_args(__name, __code) \
instr_tokenizer(__name, { \
    if (validate_args(parts, 0, line, result)) return; \
    auto instr = bytecode_instruction(__code); \
    result->add_instruction(instr); \
})

    bool validate_args(const vector<string> &parts, int require, int line_num, tokenizer_result* result) {
        if (parts.size() - 1 > require) {
            tokenize_error(line_num,
                           "Too many arguments. Expected " + str(require) + ", got " + str(parts.size()-1), result);
            return true;
        }
        if (parts.size() - 1 < require) {
            tokenize_error(line_num,
                           "Not enough arguments. Expected " + str(require) + ", got " + str(parts.size()-1), result);
            return true;
        }
        return false;
    };

    bool parse_number(const string &content, uint *value, int max_size, int line_num, tokenizer_result* result) {
        if (content.rfind("0x", 0) == 0) {
            // It's a hex value
            std::stringstream stream;
            uint buffer;
            stream << std::hex << content.substr(2);
            stream >> buffer;
            *value = buffer;

        } else if (content.rfind("0b", 0) == 0) {
            // It's a binary value
            *value = stoi(content.substr(2), nullptr, 2);

        } else {
            // It's a normal value
            *value = stoi(content);
        }

        if (*value > pow(2, (8 * max_size))) {
            tokenize_error(line_num, "Value too large. Number must be " + str(max_size * 8) + "-bit", result);
        }

        return true;
    };

    instr_tokenizer_no_args(noop, 0x0000)

    instr_tokenizer_no_args(halt, 0x0001)

    instr_tokenizer_no_args(panic, 0x0002)

    instr_tokenizer(increment_register, {
        if (validate_args(parts, 1, line, result)) return;
        auto instr = bytecode_instruction(0x0100);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        result->add_instruction(instr);
    })

    instr_tokenizer(decrement_register, {
        if (validate_args(parts, 1, line, result)) return;
        auto instr = bytecode_instruction(0x0101);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        result->add_instruction(instr);
    })

    instr_tokenizer(store_into_register, {
        if (validate_args(parts, 2, line, result)) return;
        auto instr = bytecode_instruction(0x0102);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        uint data; // Parse the data
        parse_number(parts[2], &data, 1, line, result);
        instr.data[1] = (byte) data;

        result->add_instruction(instr);
    })

    instr_tokenizer(memory_to_register, {
        if (validate_args(parts, 2, line, result)) return;
        auto instr = bytecode_instruction(0x0103);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        uint data; // Parse the data
        parse_number(parts[2], &data, 2, line, result);
        auto data_bytes = bin::short_to_bytes((short)data);
        instr.data[1] = (byte) data_bytes[0];
        instr.data[2] = (byte) data_bytes[1];

        result->add_instruction(instr);
    })

    instr_tokenizer(register_to_memory, {
        if (validate_args(parts, 2, line, result)) return;
        auto instr = bytecode_instruction(0x0104);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        uint data; // Parse the data
        parse_number(parts[2], &data, 2, line, result);
        auto data_bytes = bin::short_to_bytes((short)data);
        instr.data[1] = (byte) data_bytes[0];
        instr.data[2] = (byte) data_bytes[1];

        result->add_instruction(instr);
    })

    instr_tokenizer(write_state_register, {
        if (validate_args(parts, 1, line, result)) return;
        auto instr = bytecode_instruction(0x0110);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        result->add_instruction(instr);
    })
    instr_tokenizer(read_state_register, {
        if (validate_args(parts, 1, line, result)) return;
        auto instr = bytecode_instruction(0x0111);

        uint reg; // Parse the register ID
        parse_number(parts[1], &reg, 1, line, result);
        instr.data[0] = (byte) reg;

        result->add_instruction(instr);
    })
}