#pragma once
#include "shared.hpp"
#include <sys/stat.h>

namespace bin {

    /// Convert two bytes into a short
    short bytes_to_short (byte a, byte b) {
        short buffer = a;
        buffer <<= 8;
        buffer |= b;
        return buffer;
    }

    /// Convert two bytes at the given pointer into a short
    short bytes_to_short (byte* start, int offset = 0) {
        return bytes_to_short(start[offset], start[offset + 1]);
    }

    /// Convert a short into a byte array
    array<byte, 2> short_to_bytes (short value) {
        array<byte, 2> buffer = array<byte, 2>();
        buffer[0] = ((value & 0xFF00) >> 8);
        buffer[1] = (value & 0x00FF);
        return buffer;
    }


    /// Convert four bytes into an int
    int bytes_to_int (byte a, byte b, byte c, byte d) {
        int buffer = a;
        buffer = ((buffer << 8) | b);
        buffer = ((buffer << 8) | c);
        buffer = ((buffer << 8) | d);
        return buffer;
    }
    /// Convert four bytes at the given pointer to an int
    int bytes_to_int (byte* start, int offset = 0) {
        return bytes_to_int(start[offset], start[offset + 1], start[offset + 2], start[offset + 3]);
    }

    /// Convert an int into a byte array
    array<byte, 4> int_to_bytes (int value) {
        array<byte, 4> buffer = array<byte, 4>();
        buffer[0] = ((value & 0xFF00) >> (8 * 3));
        buffer[1] = ((value & 0xFF00) >> (8 * 2));
        buffer[2] = ((value & 0xFF00) >> (8 * 1));
        buffer[3] = ((value & 0xFF00) >> (8 * 0));
        return buffer;
    }


    /// Dump a hexadecimal representation of the data at the pointer to std::cout
    void print_hex (byte* pointer, uint length) {
        for(int i=0; i < length; ++i)
            std::cout << full_length(byte) << std::hex << (int)pointer[i];
    }
}

string plural_string (const string& singular, const string& plural, long long number) {
    return (number == 1 ? singular : plural);
}
string plural_string (const string& singular, long long number) {
    return plural_string(singular, singular + "s", number);
}

string plural_num_string (const string& singular, long long number) {
    return str(number) + " " + plural_string(singular, singular + "s", number);
}

bool file_exists (const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}