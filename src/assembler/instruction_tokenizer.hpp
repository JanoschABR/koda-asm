#pragma once
#include "../shared.hpp"

class tokenizer_result;
class instruction_tokenizer {
    public:
        virtual void tokenize (const vector<string>& parts, int line, tokenizer_result* result) {}
};

#define instr_tokenizer_body(__body) public: void tokenize (const vector<string>& parts, int line, tokenizer_result* result) override __body
#define instr_tokenizer(__name, __body) class __name : public instruction_tokenizer { instr_tokenizer_body(__body) };