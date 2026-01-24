#pragma once
#include <vector>

class BinaryConverter {
public:
    template<typename T>
    static inline std::vector<uint8_t> IntToBytes(T value) {
        return {
            static_cast<uint8_t>((value >> 24) & 0xFF),
            static_cast<uint8_t>((value >> 16) & 0xFF),
            static_cast<uint8_t>((value >> 8) & 0xFF),
            static_cast<uint8_t>(value & 0xFF)
        };
    }

    template<typename T>
    static inline const char* IntToRawBinary(T value)
    {
        return reinterpret_cast<const char*>(&value);
    }
};