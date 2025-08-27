#pragma once

#include "Types.h"
#include "Audio.h"

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
    std::array<u8, 8> _flags{};

    bool _isHiRes = false;
    std::array<u32, 64 * 32> _lowRes{};
    std::array<u32, 128 * 64> _hiRes{};

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

    Audio _audio = {};
    bool _audioStarted = false;

    std::mt19937 _engine{ std::random_device{}() };
    std::uniform_int_distribution<u16> _dist{ 0, 255 };

    const u16 HIRES_FONT_START = 0xA0;
    const u16 LOWRES_FONT_START = 0x50;

    const u8 _lowResFont[80] =
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

    const u8 _hiResFont[160] =
    {
        0x3C, 0x66, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x00, // 0
        0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, // 1
        0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x66, 0x7E, 0x00, // 2
        0x3C, 0x66, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x66, 0x3C, 0x00, // 3
        0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x00, // 4
        0x7E, 0x60, 0x60, 0x7C, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00, // 5
        0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, // 6
        0x7E, 0x66, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, // 7
        0x3C, 0x66, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, // 8
        0x3C, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x18, 0x70, 0x00, // 9
        0x3C, 0x66, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, // A
        0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, // B
        0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, // C
        0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, // D
        0xFE, 0x62, 0x68, 0x78, 0x70, 0x78, 0x68, 0x62, 0xFE, 0x00, // E
        0xFE, 0x62, 0x68, 0x78, 0x70, 0x70, 0x70, 0x70, 0x70, 0x00  // F
    };

public:
    const u32* GetPixelData() const { return _isHiRes ? _hiRes.data() : _lowRes.data(); }

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

    // Util Helpers
public:
    const u8 GetWidth() const { return _isHiRes ? 128 : 64; }
    const u8 GetHeight() const { return _isHiRes ? 64 : 32; }

private:
    template <typename T, size_t S>
    void Clear(std::array<T, S>& arr) { std::fill(std::begin(arr), std::end(arr), 0); }

    bool ValidateKey(u8 key) { return (key >= 0 && key <= 15); }

    //Instructions:
private:
    void OP_0NNN();
    void OP_00CN();
    void OP_00E0();
    void OP_00EE();
    void OP_00FB();
    void OP_00FC();
    void OP_00FE();
    void OP_00FF();
    void OP_00FD();
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
    void OP_DXY0();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX30();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();
    void OP_FX75();
    void OP_FX85();
};