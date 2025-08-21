#pragma once

#include "Types.h"

#include <array>
#include <random>
#include <vector>

class CPU
{
public:
    void Fetch();
    void Decode();
    void Execute();
    void UpdateTimers();

    void Reset(std::vector<char> rom, size_t romSize);

    std::string Disassemble(u16 addr) const;

private:
    u16 START_ADDRESS = 0x200;
    std::array<u8, 4096> _memory{};
    std::array<u8, 16> _registers{};
    std::array<u8, 16> _key{};
    std::array<u16, 16> _stack{};

    std::array<u32, 64 * 32> _screen{};

    u16 _opcode;
    u16 _index;
    u16 _pc;
    u16 _sp;

    u16 _addr; // Lowest 12 bits
    u8 _byte; // Lowest 8 bits
    u16 _hNibble; // Highest 4 bits
    u16 _lNibble; // Lowest 4 bits
    u8 _x; // Lower 4 bits of high byte
    u8 _y; // Upper 4 bits of low byte

    u8 _delayTimer;
    u8 _soundTimer;

    std::mt19937 _engine{ std::random_device{}() };
    std::uniform_int_distribution<u16> _dist{ 0, 255 };

    u16 FONTSET_START_ADDRESS = 0x50;
    u8 _fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

public:
    const u32* GetPixelData() const { return _screen.data(); }

    void KeyDown(u8 hex) { if (hex < 16) _key[hex] = 1; }
    void KeyUp(u8 hex) { if (hex < 16) _key[hex] = 0; }
    const bool IsKeyDown(u8 hex) const { return _key[hex] == 1; }

    const u8 GetDelayTimer() const { return _delayTimer; }
    void SetDelayTimer(u8 timer) { _delayTimer = timer; }
    void DecrementDelayTimer() { _delayTimer--; }

    const u8 GetSoundTimer() const { return _soundTimer; }
    void SetSoundTimer(u8 timer) { _soundTimer = timer; }
    void DecrementSoundTimer() { _soundTimer--; }

    const u16 PeekOpcode(u16 addr) const { if (addr >= _memory.size() - 1) return 0; return _memory[addr] << 8 | _memory[addr + 1]; }
    const u16 GetOpcode() const { return _opcode; }
    void SetOpcode(u16 opcode) { _opcode = opcode; }

    const u16 GetStartAddress() const { return START_ADDRESS; }
    const size_t GetMemorySize() const { return _memory.size(); }
    const u8* GetMemory() const { return _memory.data(); }
    const std::array<u8, 4096>& GetMemoryArray() const { return _memory; }

    const u16 GetPC() const { return _pc; }
    void SetPC(u16 pc) { _pc = pc; }

    const u16* GetStack() const { return _stack.data(); }
    const u16 GetSP() const { return _sp; }

    const u8 GetVRegister(u8 reg) const { return _registers[reg]; }
    const u16 GetIndex() const { return _index; }

    // Util Helper
    template <typename T, size_t S>
    void Clear(std::array<T, S>& arr) { std::fill(std::begin(arr), std::end(arr), 0); }

private:
    //Instructions:
    void OP_0NNN();
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();
};