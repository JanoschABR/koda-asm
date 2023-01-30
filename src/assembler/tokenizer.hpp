#pragma once

#include <sstream>
#include <vector>
#include <map>
#include "../shared.hpp"
#include "instruction_tokenizer.hpp"
#include "bytecode.hpp"

class tokenizer_result {
    public:
        vector<bytecode_instruction> instructions = vector<bytecode_instruction>();
        int errors = 0;
        int todos = 0;

        void add_instruction (bytecode_instruction instruction) {
            instructions.push_back(instruction);
        }
};

class tokenize_error {
    public:
        tokenize_error(int line, const string& message, tokenizer_result* result) {
            cout << cout_err("Tokenizer error on line " << line << ": " << message) << endl;
            result->errors++;
        }
};

class assembly_tokenizer {
    protected:
        map<string, instruction_tokenizer*> instructions = map<string, instruction_tokenizer*>();

    public:

        void add_instruction_tokenizer (const string& keyword, instruction_tokenizer* tokenizer) {
            instructions[keyword] = tokenizer;
        }

        bool tokenize_file (ifstream& stream, tokenizer_result* result = nullptr) {
            if (result == nullptr) {
                tokenize_error(0, "No tokenizer_result pointer specified!", result);
                return false;
            }

            string line; int line_num = 1;
            while (std::getline(stream, line)){
                /*bool ok = */tokenize_line(line, line_num, result);
                //if (!ok) return false;
                line_num++;
            }

            return true;
        }

        const std::string WHITESPACE = " \n\r\t\f\v";

        std::string ltrim(const std::string &s)
        {
            size_t start = s.find_first_not_of(WHITESPACE);
            return (start == std::string::npos) ? "" : s.substr(start);
        }

        std::string rtrim(const std::string &s)
        {
            size_t end = s.find_last_not_of(WHITESPACE);
            return (end == std::string::npos) ? "" : s.substr(0, end + 1);
        }

        std::string trim(const std::string &s) {
            return rtrim(ltrim(s));
        }

        void parse_comment (vector<string> parts, int line, tokenizer_result* result) {
            string buffer;
            for(const string& part : parts) buffer += (" " + part);

            buffer = trim(buffer);
            if (buffer.find("TODO: ") != std::string::npos) {
                std::stringstream stream(buffer);

                for (int i = 0; i < 2; i++) {
                    std::getline(stream, buffer, ':');
                }

                buffer = trim(buffer);
                //cout << "TO-DO on line " << line << ": \"" << buffer << "\"" << endl;
                result->todos++;
            }
        }

        bool tokenize_line (const string& line, int line_num, tokenizer_result* result) {
            std::stringstream stream(line);

            vector<string> parts;
            vector<string> comment_parts;

            string buffer; bool comment = false;
            while (std::getline(stream, buffer, ' ')) {
                if (buffer.empty()) continue;

                if (buffer.rfind(';', 0) == 0) {
                    buffer = buffer.substr(1);
                    comment = true;
                }

                if (comment) {
                    comment_parts.push_back(buffer);
                } else {
                    parts.push_back(buffer);
                }
            }

            if (!comment_parts.empty()) {
                parse_comment(comment_parts, line_num, result);
            }

            if (!parts.empty()) {
                string name = parts[0];
                auto instruction = instructions[name];

                if (instruction == nullptr) {
                    tokenize_error(line_num, "Unknown instruction '" + name + "'", result);
                    return false;
                }

                instruction->tokenize(parts, line_num, result);
            }

            return true;
        }
};