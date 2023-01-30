#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <array>
using namespace std;

#define byte unsigned char
#define uint unsigned int
#define ushort unsigned short
#define ulong unsigned long

#define data8 byte
#define data16 short
#define data32 int
#define data64 long

#define str to_string

#include <iomanip>
#define full_length(type) std::setfill('0') << std::setw(sizeof(type)*2)

void print_hex (byte* pointer, uint length) {
    for(int i=0; i < length; ++i)
        std::cout << full_length(byte) << std::hex << (int)pointer[i];
}

#define cout_err(__sub) __sub