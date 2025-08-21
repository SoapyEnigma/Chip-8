#include "CPU.h"

#include <cstdio>

void CPU::Fetch()
{
    _opcode = (_memory[_pc] << 8) | _memory[_pc + 1];
    _pc += 2;
}

void CPU::Decode()
{
    _addr = _opcode & 0x0FFF;
    _byte = _opcode & 0x00FF;
    _hNibble = _opcode & 0xF000;
    _lNibble = _opcode & 0x000F;
    _x = (_opcode & 0x0F00) >> 8;
    _y = (_opcode & 0x00F0) >> 4;
}

void CPU::Execute()
{
    switch (_hNibble)
    {
    case 0x0000:
    {
        switch (_byte)
        {
        case 0x0E0: OP_00E0(); break;
        case 0x0EE: OP_00EE(); break;
        default: OP_0NNN(); break;
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
        switch (_lNibble)
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

void CPU::UpdateTimers()
{
    if (_delayTimer > 0)
        _delayTimer--;

    if (_soundTimer > 0)
    {
        _beep(440, 100);
        _soundTimer--;
    }
}

void CPU::Reset(std::vector<char> rom, size_t romSize)
{
    _pc = START_ADDRESS;
    _opcode = 0;
    _index = 0;
    _sp = 0;

    _addr = 0x0;
    _byte = 0x0;
    _x = 0x0;
    _y = 0x0;

    _delayTimer = 0;
    _soundTimer = 0;

    Clear(_key);
    Clear(_screen);
    Clear(_stack);
    Clear(_registers);
    Clear(_memory);

    // Load Font
    for (i32 i = 0; i < std::size(_fontset); i++)
        _memory[FONTSET_START_ADDRESS + i] = _fontset[i];

    // Reload ROM
    if (romSize)
        std::copy(rom.begin(), rom.end(), _memory.begin() + START_ADDRESS);
}

std::string CPU::Disassemble(u16 addr) const
{
    const u16 op = PeekOpcode(addr);
    const u16 nnn = op & 0x0FFF;
    const u8  nn = u8(op & 0x00FF);
    const u8  n = u8(op & 0x000F);
    const u8  x = u8((op >> 8) & 0x0F);
    const u8  y = u8((op >> 4) & 0x0F);

    char buf[64];
    switch (op & 0xF000)
    {
    case 0x0000:
        switch (op)
        {
        case 0x00E0: return "CLS";
        case 0x00EE: return "RET";
        default: snprintf(buf, sizeof(buf), "SYS 0x%03X", nnn); return buf;
        }

    case 0x1000: snprintf(buf, sizeof(buf), "JP 0x%03X", nnn); return buf;
    case 0x2000: snprintf(buf, sizeof(buf), "CALL 0x%03X", nnn); return buf;
    case 0x3000: snprintf(buf, sizeof(buf), "SE V%X, 0x%02X", x, nn); return buf;
    case 0x4000: snprintf(buf, sizeof(buf), "SNE V%X, 0x%02X", x, nn); return buf;
    case 0x5000: snprintf(buf, sizeof(buf), "SE V%X, V%X", x, y); return buf;
    case 0x6000: snprintf(buf, sizeof(buf), "LD V%X, 0x%02X", x, nn); return buf;
    case 0x7000: snprintf(buf, sizeof(buf), "ADD V%X, 0x%02X", x, nn); return buf;

    case 0x8000:
        switch (n)
        {
        case 0x0: snprintf(buf, sizeof(buf), "LD V%X, V%X", x, y); break;
        case 0x1: snprintf(buf, sizeof(buf), "OR V%X, V%X", x, y); break;
        case 0x2: snprintf(buf, sizeof(buf), "AND V%X, V%X", x, y); break;
        case 0x3: snprintf(buf, sizeof(buf), "XOR V%X, V%X", x, y); break;
        case 0x4: snprintf(buf, sizeof(buf), "ADD V%X, V%X", x, y); break;
        case 0x5: snprintf(buf, sizeof(buf), "SUB V%X, V%X", x, y); break;
        case 0x6: snprintf(buf, sizeof(buf), "SHR V%X {,V%X}", x, y); break;
        case 0x7: snprintf(buf, sizeof(buf), "SUBN V%X, V%X", x, y); break;
        case 0xE: snprintf(buf, sizeof(buf), "SHL V%X {,V%X}", x, y); break;
        default:  snprintf(buf, sizeof(buf), "UNKNOWN 0x%04X", op);  break;
        }
        return buf;

    case 0x9000: snprintf(buf, sizeof(buf), "SNE V%X, V%X", x, y); return buf;
    case 0xA000: snprintf(buf, sizeof(buf), "LD I, 0x%03X", nnn); return buf;
    case 0xB000: snprintf(buf, sizeof(buf), "JP V0, 0x%03X", nnn); return buf;
    case 0xC000: snprintf(buf, sizeof(buf), "RND V%X, 0x%02X", x, nn); return buf;
    case 0xD000: snprintf(buf, sizeof(buf), "DRW V%X, V%X, 0x%X", x, y, n); return buf;

    case 0xE000:
        switch (nn)
        {
        case 0x9E: snprintf(buf, sizeof(buf), "SKP V%X", x); break;
        case 0xA1: snprintf(buf, sizeof(buf), "SKNP V%X", x); break;
        default: snprintf(buf, sizeof(buf), "UNKNOWN 0x%04X", op); break;
        }
        return buf;

    case 0xF000:
        switch (nn)
        {
        case 0x07: snprintf(buf, sizeof(buf), "LD V%X, DT", x); break;
        case 0x0A: snprintf(buf, sizeof(buf), "LD V%X, K", x); break;
        case 0x15: snprintf(buf, sizeof(buf), "LD DT, V%X", x); break;
        case 0x18: snprintf(buf, sizeof(buf), "LD ST, V%X", x); break;
        case 0x1E: snprintf(buf, sizeof(buf), "ADD I, V%X", x); break;
        case 0x29: snprintf(buf, sizeof(buf), "LD F, V%X", x); break;
        case 0x33: snprintf(buf, sizeof(buf), "LD B, V%X", x); break;
        case 0x55: snprintf(buf, sizeof(buf), "LD [I], V0..V%X", x); break;
        case 0x65: snprintf(buf, sizeof(buf), "LD V0..V%X, [I]", x); break;
        default: snprintf(buf, sizeof(buf), "UNKNOWN 0x%04X", op); break;
        }
        return buf;
    }

    snprintf(buf, sizeof(buf), "UNKNOWN 0x%04X", op);
    return buf;
}

void CPU::OP_0NNN()
{
    /*NOP*/
}

void CPU::OP_00E0()
{
    Clear(_screen);
}

void CPU::OP_00EE()
{
    _sp--;
    _pc = _stack[_sp];
}

void CPU::OP_1NNN()
{
    _pc = _addr;
}

void CPU::OP_2NNN()
{
    _stack[_sp] = _pc;
    _sp++;
    _pc = _addr;
}

void CPU::OP_3XNN()
{
    if (_registers[_x] == _byte)
        _pc += 2;
}

void CPU::OP_4XNN()
{
    if (_registers[_x] != _byte)
        _pc += 2;
}

void CPU::OP_5XY0()
{
    if (_registers[_x] == _registers[_y])
        _pc += 2;
}

void CPU::OP_6XNN()
{
    _registers[_x] = _byte;
}

void CPU::OP_7XNN()
{
    _registers[_x] += _byte;
}

void CPU::OP_8XY0()
{
    _registers[_x] = _registers[_y];
}

void CPU::OP_8XY1()
{
    _registers[_x] |= _registers[_y];
}

void CPU::OP_8XY2()
{
    _registers[_x] &= _registers[_y];
}

void CPU::OP_8XY3()
{
    _registers[_x] ^= _registers[_y];
}

void CPU::OP_8XY4()
{
    u16 sum = _registers[_x] + _registers[_y];

    _registers[0xF] = sum > 255 ? 1 : 0;

    _registers[_x] = sum & 0xFF;
}

void CPU::OP_8XY5()
{
    _registers[0xF] = _registers[_x] > _registers[_y] ? 1 : 0;

    _registers[_x] -= _registers[_y];
}

void CPU::OP_8XY6()
{
    _registers[0xF] = _registers[_x] & 0x1;

    _registers[_x] >>= 1;
}

void CPU::OP_8XY7()
{
    _registers[0xF] = _registers[_y] > _registers[_x] ? 1 : 0;

    _registers[_x] = _registers[_y] - _registers[_x];
}

void CPU::OP_8XYE()
{
    _registers[0xF] = (_registers[_x] & 0x80) >> 7;

    _registers[_x] <<= 1;
}

void CPU::OP_9XY0()
{
    if (_registers[_x] != _registers[_y])
        _pc += 2;
}

void CPU::OP_ANNN()
{
    _index = _addr;
}

void CPU::OP_BNNN()
{
    _pc = _registers[0] + _addr;
}

void CPU::OP_CXNN()
{
    _registers[_x] = _dist(_engine) & _byte;
}

void CPU::OP_DXYN()
{
    u8 height = _opcode & 0x000F;

    u8 xPos = _registers[_x] & 63;
    u8 yPos = _registers[_y] & 31;

    _registers[0xF] = 0;

    for (u32 row = 0; row < height; row++)
    {
        u8 spriteByte = _memory[_index + row];

        for (u32 col = 0; col < 8; col++)
        {
            u8 spritePixel = spriteByte & (0x80 >> col);

            u32 x = (xPos + col) & 63;
            u32 y = (yPos + row) & 31;
            u32 i = y * 64 + x;

            u32* screenPixel = &_screen[i];

            if (spritePixel)
            {
                if (*screenPixel == 0xFFFFFFFF)
                    _registers[0xF] = 1;

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void CPU::OP_EX9E()
{
    u8 key = _registers[_x];

    if (_key[key])
        _pc += 2;
}

void CPU::OP_EXA1()
{
    u8 key = _registers[_x];

    if (!_key[key])
        _pc += 2;
}

void CPU::OP_FX07()
{
    _registers[_x] = _delayTimer;
}

void CPU::OP_FX0A()
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

void CPU::OP_FX15()
{
    _delayTimer = _registers[_x];
}

void CPU::OP_FX18()
{
    _soundTimer = _registers[_x];
}

void CPU::OP_FX1E()
{
    _index += _registers[_x];
}

void CPU::OP_FX29()
{
    u8 value = _registers[_x];
    _index = FONTSET_START_ADDRESS + (5 * value);
}

void CPU::OP_FX33()
{
    u8 value = _registers[_x];

    _memory[_index + 2] = value % 10;
    value /= 10;

    _memory[_index + 1] = value % 10;
    value /= 10;

    _memory[_index] = value % 10;
}

void CPU::OP_FX55()
{
    for (u8 i = 0; i <= _x; i++)
        _memory[_index + i] = _registers[i];
}

void CPU::OP_FX65()
{
    for (u8 i = 0; i <= _x; i++)
        _registers[i] = _memory[_index + i];
}