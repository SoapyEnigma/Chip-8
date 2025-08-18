#include "Chip8.h"

#include <filesystem>
#include <fstream>
#include <iostream>

Chip8::Chip8()
{
    Init();
}

void Chip8::Cycle()
{
    Fetch();

    Decode();

    Execute();

    UpdateTimers();
}

void Chip8::LoadROM(std::string_view filePath)
{
    std::ifstream file(filePath.data(), std::ios::binary | std::ios::ate);

    if (!file)
        return; // Fails

    const std::streamsize size = file.tellg();
    if (size <= 0)
        return; // Fails
    if (START_ADDRESS + static_cast<size_t>(size) > _memory.size())
        return; // Fails

    std::vector<char> buffer(size);
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), size);

    for (u32 i = 0; i < size; i++)
        _memory[START_ADDRESS + i] = buffer[i];

    _engine = std::mt19937(_rd());
}

void Chip8::Init()
{
    _pc = START_ADDRESS;
    _opcode = 0;
    _index = 0;
    _sp = 0;

    _addr = 0x0;
    _byte = 0x0;
    _x = 0x0;
    _y = 0x0;

    _delayTimer = 60;
    _soundTimer = 60;

    Clear(_screen);
    Clear(_stack);
    Clear(_registers);
    Clear(_memory);

    // Load Font
    for (i32 i = 0; i < std::size(_fontset); i++)
        _memory[FONTSET_START_ADDRESS + i] = _fontset[i];
}

void Chip8::Fetch()
{
    _opcode = (_memory[_pc] << 8u) | _memory[_pc + 1];
    _pc += 2;
}

void Chip8::Decode()
{
    _addr = _opcode & 0x0FFF;
    _byte = _opcode & 0x00FF;
    _nibble = _opcode & 0xF000;
    _x = (_opcode & 0x0F00) >> 8;
    _y = (_opcode & 0x00F0) >> 4;
}

void Chip8::Execute()
{
    switch (_nibble)
    {
    case 0x0000:
    {
        switch (_byte)
        {
        case 0x0E0: OP_00E0(); break;
        case 0x0EE: OP_00EE(); break;
        default: printf("Unknown 00??: 0x%04X\n", _opcode); break;
        }
    } break;
    case 0x1000: OP_1NNN(); break;
    case 0x2000: OP_2NNN(); break;
    case 0x3000: OP_3XNN(); break;
    case 0x4000: OP_4XNN(); break;
    case 0x5000: OP_5XY0(); break;
    case 0x6000: OP_6XNN(); break;
    case 0x7000: OP_7XNN(); break;
    case 0x8000:
    {
        switch (_opcode & 0x000F)
        {
        case 0x0: OP_8XY0(); break;
        case 0x1: OP_8XY1(); break;
        case 0x2: OP_8XY2(); break;
        case 0x3: OP_8XY3(); break;
        case 0x4: OP_8XY4(); break;
        case 0x5: OP_8XY5(); break;
        case 0x6: OP_8XY6(); break;
        case 0x7: OP_8XY7(); break;
        case 0xE: OP_8XYE(); break;
        default: printf("Unknown 8XY?: 0x%04X\n", _opcode); break;
        }
    } break;
    case 0x9000: OP_9XY0(); break;
    case 0xA000: OP_ANNN(); break;
    case 0xB000: OP_BNNN(); break;
    case 0xC000: OP_CXNN(); break;
    case 0xD000: OP_DXYN(); break;
    case 0xE000:
    {
        switch (_byte)
        {
        case 0x009E: OP_EX9E(); break;
        case 0x00A1: OP_EXA1(); break;
        default: printf("Unknown EX??: 0x%04X\n", _opcode); break;
        }
    } break;
    case 0xF000:
    {
        switch (_byte)
        {
        case 0x0007: OP_FX07(); break;
        case 0x000A: OP_FX0A(); break;
        case 0x0015: OP_FX15(); break;
        case 0x0018: OP_FX18(); break;
        case 0x001E: OP_FX1E(); break;
        case 0x0029: OP_FX29(); break;
        case 0x0033: OP_FX33(); break;
        case 0x0055: OP_FX55(); break;
        case 0x0065: OP_FX65(); break;
        default: printf("Unknown FX??: 0x%04X\n", _opcode); break;
        }
    } break;
    default: printf("Unknown opcode: 0x%04X\n", _opcode); break;
    }
}

void Chip8::UpdateTimers()
{
    if (_delayTimer > 0)
        _delayTimer--;

    if (_soundTimer > 0)
    {
        if (_soundTimer == 1)
            printf("BEEP!\n");

        _soundTimer--;
    }
}

void Chip8::OP_0NNN()
{
}

void Chip8::OP_00E0()
{
    Clear(_screen);
}

void Chip8::OP_00EE()
{
    _sp--;
    _pc = _stack[_sp];
}

void Chip8::OP_1NNN()
{
    _pc = _addr;
}

void Chip8::OP_2NNN()
{
    _stack[_sp] = _pc;
    _sp++;
    _pc = _addr;
}

void Chip8::OP_3XNN()
{
    if (_registers[_x] == _byte)
        _pc += 2;
}

void Chip8::OP_4XNN()
{
    if (_registers[_x] != _byte)
        _pc += 2;
}

void Chip8::OP_5XY0()
{
    if (_registers[_x] == _registers[_y])
        _pc += 2;
}

void Chip8::OP_6XNN()
{
    _registers[_x] = _byte;
}

void Chip8::OP_7XNN()
{
    _registers[_x] += _byte;
}

void Chip8::OP_8XY0()
{
    _registers[_x] = _registers[_y];
}

void Chip8::OP_8XY1()
{
    _registers[_x] |= _registers[_y];
}

void Chip8::OP_8XY2()
{
    _registers[_x] &= _registers[_y];
}

void Chip8::OP_8XY3()
{
    _registers[_x] ^= _registers[_y];
}

void Chip8::OP_8XY4()
{
    u16 sum = _registers[_x] + _registers[_y];

    _registers[0xF] = sum > 255 ? 1 : 0;

    _registers[_x] = sum & 0xFF;
}

void Chip8::OP_8XY5()
{
    _registers[0xF] = _registers[_x] > _registers[_y] ? 1 : 0;

    _registers[_x] -= _registers[_y];
}

void Chip8::OP_8XY6()
{
    _registers[0xF] = _registers[_x] & 0x1;

    _registers[_x] >>= 1;
}

void Chip8::OP_8XY7()
{
    _registers[0xF] = _registers[_y] > _registers[_x] ? 1 : 0;

    _registers[_x] = _registers[_y] - _registers[_x];
}

void Chip8::OP_8XYE()
{
    _registers[0xF] = (_registers[_x] & 0x80) >> 7;

    _registers[_x] <<= 1;
}

void Chip8::OP_9XY0()
{
    if (_registers[_x] != _registers[_y])
        _pc += 2;
}

void Chip8::OP_ANNN()
{
    _index = _addr;
}

void Chip8::OP_BNNN()
{
    _pc = _registers[0] + _addr;
}

void Chip8::OP_CXNN()
{
    _registers[_x] = _dist(_engine) & _byte;
}

void Chip8::OP_DXYN()
{
    u8 height = _opcode & 0x000F;

    u8 xPos = _registers[_x] % WIDTH;
    u8 yPos = _registers[_y] % HEIGHT;

    _registers[0xF] = 0;

    for (u32 row = 0; row < height; row++)
    {
        u8 spriteByte = _memory[_index + row];

        for (u32 col = 0; col < 8; col++)
        {
            u8 spritePixel = spriteByte & (0x80 >> col);
            u32* screenPixel = &_screen[(yPos + row) * WIDTH + (xPos + col)];

            if (spritePixel)
            {
                if (*screenPixel == 0xFFFFFFFF)
                    _registers[0xF] = 1;

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::OP_EX9E()
{
    u8 key = _registers[_x];

    if (_key[key])
        _pc += 2;
}

void Chip8::OP_EXA1()
{
    u8 key = _registers[_x];

    if (!_key[key])
        _pc += 2;
}

void Chip8::OP_FX07()
{
    _registers[_x] = _delayTimer;
}

void Chip8::OP_FX0A()
{
    for (u8 k = 0; k < 16; k++)
    {
        if (_key[k])
        {
            _registers[_x] = k;
            return;
        }
    }

    _pc -= 2;
}

void Chip8::OP_FX15()
{
    _delayTimer = _registers[_x];
}

void Chip8::OP_FX18()
{
    _soundTimer = _registers[_x];
}

void Chip8::OP_FX1E()
{
    _index += _registers[_x];
}

void Chip8::OP_FX29()
{
    u8 value = _registers[_x];
    _index = FONTSET_START_ADDRESS + (5 * value);
}

void Chip8::OP_FX33()
{
    u8 value = _registers[_x];

    _memory[_index + 2] = value % 10;
    value /= 10;

    _memory[_index + 1] = value % 10;
    value /= 10;

    _memory[_index] = value % 10;
}

void Chip8::OP_FX55()
{
    for (u8 i = 0; i <= _x; i++)
        _memory[_index + i] = _registers[i];
}

void Chip8::OP_FX65()
{
    for (u8 i = 0; i <= _x; i++)
        _registers[i] = _memory[_index + i];
}
