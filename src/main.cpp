#include "shared.hpp"
#include "assembler/tokenizer.hpp"
#include "assembler/instructions.hpp"

#define create_and_add_tokenizer(t, i, n) { i tok = i(); t.add_instruction_tokenizer(n, &tok); }

int main (int argc, char* argv[]) {
    cout << "Koda Assembler v1\n--------------------------\n" << endl;

    if (argc >= 2) {
        string file = argv[1];
        string target_file = (file + ".bin");

        cout << "Source File: " << file << endl;
        cout << "Target File: " << target_file << endl;

        if (file_exists(target_file)) {
            cout << cout_err("Target file already exists!") << endl;
            return 4;
        }

        cout << "\n";

        ifstream stream = ifstream (file);
        if (stream.is_open()) {
            tokenizer_result result = tokenizer_result();
            assembly_tokenizer tokenizer = assembly_tokenizer();

            using namespace instructions;
            create_and_add_tokenizer(tokenizer, base_noop, "noop");
            create_and_add_tokenizer(tokenizer, base_halt, "halt");
            create_and_add_tokenizer(tokenizer, base_panic, "panic");

            create_and_add_tokenizer(tokenizer, base_increment_register, "regi");
            create_and_add_tokenizer(tokenizer, base_decrement_register, "regd");
            create_and_add_tokenizer(tokenizer, base_store_into_register, "load");
            create_and_add_tokenizer(tokenizer, base_memory_to_register, "memr");
            create_and_add_tokenizer(tokenizer, base_register_to_memory, "memw");

            create_and_add_tokenizer(tokenizer, base_write_state_register, "staw");
            create_and_add_tokenizer(tokenizer, base_read_state_register, "star");

            create_and_add_tokenizer(tokenizer, base_jump, "jump");
            create_and_add_tokenizer(tokenizer, base_jump_compare, "jcmp");
            create_and_add_tokenizer(tokenizer, base_compare, "cmpb");
            create_and_add_tokenizer(tokenizer, base_compare_x16, "cmps");

            create_and_add_tokenizer(tokenizer, alu_add, "add");
            create_and_add_tokenizer(tokenizer, alu_sub, "sub");
            create_and_add_tokenizer(tokenizer, alu_and, "and");
            create_and_add_tokenizer(tokenizer, alu_or, "or");
            create_and_add_tokenizer(tokenizer, alu_not, "not");
            create_and_add_tokenizer(tokenizer, alu_xor, "xor");

            create_and_add_tokenizer(tokenizer, alu_shift_l, "shl");
            create_and_add_tokenizer(tokenizer, alu_shift_r, "shr");

            create_and_add_tokenizer(tokenizer, ext_platform_info, "plat");
            create_and_add_tokenizer(tokenizer, ext_invoke, "ext");

            tokenizer.tokenize_file(stream, &result);

            cout << "\n";

            /*for(bytecode_instruction instr : result.instructions) {
                cout << full_length(short) << std::hex << instr.code << " ";
                print_hex(&instr.data[0], instr.data.size());
                cout << endl;
            }

            cout << "\n";*/

            /*if (result.todos > 0) {
                cout << "Found " << plural_num_string("to-do comment", result.todos) << "." << endl;
            }*/

            if (result.errors > 0) {
                int e = result.errors;
                cout << "Compile failed: Tokenizer reported " << plural_num_string("errors", e) << "." << endl;
                return 3;
            } else {
                auto size = (long long)result.instructions.size();
                cout << "Successfully compiled " << plural_num_string("instruction", size) << "." << endl;
            }

            ofstream output (target_file, ios::out | ios::binary);
            if(!output) {
                cout << cout_err("Unable to open output file.") << endl;
                return 1;
            }

            for(bytecode_instruction instr : result.instructions) {
                array<byte, 2> code_bytes = bin::short_to_bytes((short)instr.code);
                output.write((char*)&code_bytes[0], 2);
                output.write((char*)&instr.data[0], instr.data.size());
            }

            output.flush();
            output.close();

            return 0;
        } else {
            cout << cout_err("Unable to open file.") << endl;
            stream.close();
            return 2;
        }

        stream.close();
    } else {
        cout << cout_err("No file specified.") << endl;
        return 1;
    }
}
