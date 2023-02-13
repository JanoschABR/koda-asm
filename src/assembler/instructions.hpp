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

    void internal_put_x8 (const string& txt, byte* a, int line, tokenizer_result* result) {
        uint value;
        parse_number(txt, &value, 1, line, result);
        *a = (byte)value;
    }
#define put_x8(txt, a) internal_put_x8(txt, a, line, result)

    void internal_put_x16 (const string& txt, byte* a, byte* b, int line, tokenizer_result* result) {
        uint value;
        parse_number(txt, &value, 2, line, result);
        auto data_bytes = bin::short_to_bytes((short)value);
        *a = (byte)data_bytes[0];
        *b = (byte)data_bytes[1];
    }
#define put_x16(txt, a, b) internal_put_x16(txt, a, b, line, result)

#define instr_valid(num) if (validate_args(parts, num, line, result)) return;
#define instr_init(code) auto instr = bytecode_instruction(code);
#define instr_done() result->add_instruction(instr);

    instr_tokenizer_no_args(base_noop, 0x0000)

    instr_tokenizer_no_args(base_halt, 0x0001)

    instr_tokenizer_no_args(base_panic, 0x0002)

    instr_tokenizer(base_increment_register, {
        instr_valid(1);
        instr_init(0x0100);

        // Register
        put_x8(parts[1], &instr.data[0]);

        instr_done();
    })

    instr_tokenizer(base_decrement_register, {
        instr_valid(1);
        instr_init(0x0101);

        // Register
        put_x8(parts[1], &instr.data[0]);

        instr_done();
    })

    instr_tokenizer(base_store_into_register, {
        instr_valid(2);
        instr_init(0x0102);

        // Register
        put_x8(parts[1], &instr.data[0]);

        // Value
        put_x8(parts[2], &instr.data[1]);

        instr_done();
    })

    instr_tokenizer(base_memory_to_register, {
        instr_valid(2);
        instr_init(0x0103);

        // Register
        put_x8(parts[1], &instr.data[0]);

        // Address
        put_x16(parts[2], &instr.data[1], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(base_register_to_memory, {
        instr_valid(2);
        instr_init(0x0104);

        // Register
        put_x8(parts[1], &instr.data[0]);

        // Address
        put_x16(parts[2], &instr.data[1], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(base_write_state_register, {
        instr_valid(1);
        instr_init(0x0110);

        // Register
        put_x8(parts[1], &instr.data[0]);

        instr_done();
    })
    instr_tokenizer(base_read_state_register, {
        instr_valid(1);
        instr_init(0x0111);

        // Register
        put_x8(parts[1], &instr.data[0]);

        instr_done();
    })

    instr_tokenizer(base_jump, {
        instr_valid(1);
        instr_init(0x0200);

        // Address
        put_x16(parts[1], &instr.data[0], &instr.data[1]);

        instr_done();
    })

    instr_tokenizer(base_compare, {
        instr_valid(2);
        instr_init(0x0300);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        instr_done();
    })

    instr_tokenizer(base_compare_x16, {
        instr_valid(4);
        instr_init(0x0301);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Register C
        put_x8(parts[3], &instr.data[2]);

        // Register D
        put_x8(parts[4], &instr.data[3]);

        instr_done();
    })

    instr_tokenizer(base_jump_compare, {
        instr_valid(2);
        instr_init(0x0201);

        // Register
        put_x8(parts[1], &instr.data[0]);

        // Address
        put_x16(parts[2], &instr.data[1], &instr.data[2]);

        instr_done();
    })

    // #################################################

    instr_tokenizer(alu_add, {
        instr_valid(2);
        instr_init(0x0310);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(alu_sub, {
        instr_valid(2);
        instr_init(0x0311);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(alu_and, {
        instr_valid(3);
        instr_init(0x0312);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(alu_or, {
        instr_valid(3);
        instr_init(0x0313);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(alu_not, {
        instr_valid(2);
        instr_init(0x0314);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Out Register
        put_x8(parts[2], &instr.data[1]);

        instr_done();
    })

    instr_tokenizer(alu_xor, {
        instr_valid(3);
        instr_init(0x0315);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(alu_shift_l, {
        instr_valid(3);
        instr_init(0x0316);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    instr_tokenizer(alu_shift_r, {
        instr_valid(3);
        instr_init(0x0317);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Register B
        put_x8(parts[2], &instr.data[1]);

        // Out Register
        put_x8(parts[3], &instr.data[2]);

        instr_done();
    })

    // #######################

    instr_tokenizer(ext_platform_info, {
        instr_valid(2);
        instr_init(0x1000);

        // Register A
        put_x8(parts[1], &instr.data[0]);

        // Out Register
        put_x8(parts[2], &instr.data[1]);

        instr_done();
    })

    instr_tokenizer(ext_invoke, {
        instr_valid(INSTR_DATA_SIZE);
        instr_init(0x1001);

        for (int i = 0; i < INSTR_DATA_SIZE; i++) {
            put_x8(parts[1 + i], &instr.data[i]);
        }

        instr_done();
    })
}