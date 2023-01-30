cmake_minimum_required(VERSION 3.24)
project(koda_asm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(koda_asm src/main.cpp src/assembler/tokenizer.hpp src/shared.hpp src/assembler/instructions.hpp src/assembler/instruction_tokenizer.hpp src/assembler/bytecode.hpp)
