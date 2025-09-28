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
        case 0x0C0: OP_00CN(); break;
        case 0x0E0: OP_00E0(); break;
        case 0x0EE: OP_00EE(); break;
        case 0x0FB :OP_00FB(); break;
        case 0x0FC :OP_00FC(); break;
        case 0x0FE :OP_00FE(); break;
        case 0x0FF :OP_00FF(); break;
        case 0x0FD:OP_00FD(); break;
        default: (_opcode & 0xFFF0) == 0x00C0 ? OP_00CN() : OP_0NNN(); break;
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
    case 0xD000: (_opcode & 0x000F) == 0x0000 ? OP_DXY0() : OP_DXYN(); break;
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
        case 0x0030: OP_FX30(); break;
        case 0x0033: OP_FX33(); break;
        case 0x0055: OP_FX55(); break;
        case 0x0065: OP_FX65(); break;
        case 0x0075: OP_FX75(); break;
        case 0x0085: OP_FX85(); break;
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
        if (!_audioStarted)
            _audio.StartTone();

        _soundTimer--;
    }
    else
    {
        _audio.StopTone();
        _audioStarted = false;
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

    Clear(_lowRes);
    Clear(_hiRes);

    Clear(_key);
    Clear(_stack);
    Clear(_registers);
    Clear(_memory);
    Clear(_flags);

    // Load Font
    for (i32 i = 0; i < std::size(_lowResFont); i++)
        _memory[LOWRES_FONT_START + i] = _lowResFont[i];

    for (i32 i = 0; i < std::size(_hiResFont); i++)
        _memory[HIRES_FONT_START + i] = _hiResFont[i];

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
        if ((op & 0xFFF0) == 0x00C0)
        {
            snprintf(buf, sizeof(buf), "SCD 0x%X", n);
            return buf;
        }

        switch (op)
        {
        case 0x00E0: return "CLS";
        case 0x00EE: return "RET";
        case 0x00FB: return "SCR";
        case 0x00FC: return "SCL";
        case 0x00FD: return "EXIT";
        case 0x00FE: return "LOW";
        case 0x00FF: return "HIGH";
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
        default:  snprintf(buf, sizeof(buf), "UNKNOWN 8X??: 0x%04X", op);  break;
        }
        return buf;

    case 0x9000: snprintf(buf, sizeof(buf), "SNE V%X, V%X", x, y); return buf;
    case 0xA000: snprintf(buf, sizeof(buf), "LD I, 0x%03X", nnn); return buf;
    case 0xB000: snprintf(buf, sizeof(buf), "JP V0, 0x%03X", nnn); return buf;
    case 0xC000: snprintf(buf, sizeof(buf), "RND V%X, 0x%02X", x, nn); return buf;
    case 0xD000:
    {
        n == 0 ?
            snprintf(buf, sizeof(buf), "DRW16 V%X, V%X", x, y)
            : snprintf(buf, sizeof(buf), "DRW V%X, V%X, 0x%X", x, y, n);

        return buf;
    }
    case 0xE000:
        switch (nn)
        {
        case 0x9E: snprintf(buf, sizeof(buf), "SKP V%X", x); break;
        case 0xA1: snprintf(buf, sizeof(buf), "SKNP V%X", x); break;
        default: snprintf(buf, sizeof(buf), "UNKNOWN EX??: 0x%04X", op); break;
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
        case 0x30: snprintf(buf, sizeof(buf), "LD  HF, V%X", x); break;
        case 0x33: snprintf(buf, sizeof(buf), "LD B, V%X", x); break;
        case 0x55: snprintf(buf, sizeof(buf), "LD [I], V0..V%X", x); break;
        case 0x65: snprintf(buf, sizeof(buf), "LD V0..V%X, [I]", x); break;
        case 0x75: snprintf(buf, sizeof(buf), "LD  R, V0..V%X", x); break;
        case 0x85: snprintf(buf, sizeof(buf), "LD  V0..V%X, R", x); break;
        default: snprintf(buf, sizeof(buf), "UNKNOWN FX??: 0x%04X", op); break;
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

void CPU::OP_00CN()
{
    // Scroll Down N
    const i32 w = GetWidth();
    const i32 h = GetHeight();

    u32* buff = _isHiRes ? _hiRes.data() : _lowRes.data();
    std::vector<u32> src(buff, buff + (w * h));

    for (i32 y = 0; y < h; y++)
    {
        for (i32 x = 0; x < w; x++)
        {
            const i32 sy = y - (_byte & 0x0F);
            buff[y * w + x] = (sy >= 0 && sy < h) ? src[sy * w + x] : 0;
        }
    }
}

void CPU::OP_00E0()
{
    _isHiRes ? Clear(_hiRes) : Clear(_lowRes);
}

void CPU::OP_00EE()
{
    _sp--;
    _pc = _stack[_sp];
}

void CPU::OP_00FB()
{
    // Scroll Right 4
    const i32 w = GetWidth();
    const i32 h = GetHeight();

    u32* buff = _isHiRes ? _hiRes.data() : _lowRes.data();
    std::vector<u32> src(buff, buff + (w * h));

    for (i32 y = 0; y < h; y++)
    {
        for (i32 x = 0; x < w; x++)
        {
            const i32 sx = x - 4;
            buff[y * w + x] = (sx >= 0 && sx < w) ? src[y * w + sx] : 0;
        }
    }
}

void CPU::OP_00FC()
{
    // Scroll Left 4
    const i32 w = GetWidth();
    const i32 h = GetHeight();

    u32* buff = _isHiRes ? _hiRes.data() : _lowRes.data();
    std::vector<u32> src(buff, buff + (w * h));

    for (i32 y = 0; y < h; y++)
    {
        for (i32 x = 0; x < w; x++)
        {
            const i32 sx = x + 4;
            buff[y * w + x] = (sx >= 0 && sx < w) ? src[y * w + sx] : 0;
        }
    }
}

void CPU::OP_00FE()
{
    _isHiRes = false;
    Clear(_lowRes);
}

void CPU::OP_00FF()
{
    _isHiRes = true;
    Clear(_hiRes);
}

void CPU::OP_00FD()
{
    // Exit
}

void CPU::OP_1NNN()
{
    _pc = _addr;
}

void CPU::OP_2NNN()
{
    if (_addr < 0x200) // Legacy calls
        return;

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
    const u8 x = (_opcode >> 8) & 0xF;
    const u8 y = (_opcode >> 4) & 0xF;

    const u16 sum = uint16_t(_registers[x]) + uint16_t(_registers[y]);
    const u8 result = uint8_t(sum & 0xFF);
    const u8 carry = (sum > 0xFF) ? 1 : 0;

    _registers[x] = result;
    _registers[0xF] = carry;
}

void CPU::OP_8XY5()
{
    const u8 x = (_opcode >> 8) & 0xF;
    const u8 y = (_opcode >> 4) & 0xF;

    const u8 noBorrow = (_registers[x] >= _registers[y]) ? 1 : 0;
    const u8 result = uint8_t(_registers[x] - _registers[y]);

    _registers[x] = result;
    _registers[0xF] = noBorrow;
}

void CPU::OP_8XY6()
{
    const u8 x = (_opcode >> 8) & 0xF;

    const u8 old = _registers[x];
    const u8 flag = old & 0x1;
    const u8 result = old >> 1;

    _registers[x] = result;
    _registers[0xF] = flag;
}

void CPU::OP_8XY7()
{
    const u8 x = (_opcode >> 8) & 0xF;
    const u8 y = (_opcode >> 4) & 0xF;

    const u8 noBorrow = (_registers[y] >= _registers[x]) ? 1 : 0;
    const u8 result = uint8_t(_registers[y] - _registers[x]);

    _registers[x] = result;
    _registers[0xF] = noBorrow;
}

void CPU::OP_8XYE()
{
    const u8 x = (_opcode >> 8) & 0xF;

    const u8 old = _registers[x];
    const u8 flag = (old >> 7) & 0x1;
    const u8 result = uint8_t((old << 1) & 0xFF);

    _registers[x] = result;
    _registers[0xF] = flag;
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
    const u32 w = GetWidth();
    const u32 h = GetHeight();

    _registers[0xF] = 0;
    auto* buff = _isHiRes ? _hiRes.data() : _lowRes.data();

    u8 xPos = _registers[_x] % w;
    u8 yPos = _registers[_y] % h;

    u8 height = _opcode & 0x000F;
    for (u32 row = 0; row < height; row++)
    {
        u8 byte = _memory[_index + row];
        for (u32 col = 0; col < 8; col++)
        {
            if (byte & (0x80 >> col))
            {
                u32 x = (xPos + col) % w;
                u32 y = (yPos + row) % h;
                u32 i = y * w + x;

                if (buff[i] == 0xFFFFFFFF)
                    _registers[0xF] = 1;

                buff[i] ^= 0xFFFFFFFF;
            }
        }
    }
}

void CPU::OP_DXY0()
{
    if (!_isHiRes)
        return;

    const u32 w = GetWidth();
    const u32 h = GetHeight();

    _registers[0xF] = 0;
    auto& buff = _hiRes;

    const u8 xPos = _registers[_x] % w;
    const u8 yPos = _registers[_y] % h;

    for (u32 row = 0; row < 16; row++)
    {
        const u8 left = _memory[_index + row * 2];
        const u8 right = _memory[_index + row * 2 + 1];

        for (u32 col = 0; col < 8; col++)
        {
            if (left & (0x80 >> col))
            {
                const u32 x = (xPos + col) % w;
                const u32 y = (yPos + row) % h;
                const u32 i = y * w + x;

                if (buff[i] == 0xFFFFFFFF)
                    _registers[0xF] = 1;

                buff[i] ^= 0xFFFFFFFF;
            }

            if (right & (0x80 >> col))
            {
                const u32 x = (xPos + 8 + col) % w;
                const u32 y = (yPos + row) % h;
                const u32 i = y * w + x;

                if (buff[i] == 0xFFFFFFFF)
                    _registers[0xF] = 1;

                buff[i] ^= 0xFFFFFFFF;
            }
        }
    }
}

void CPU::OP_EX9E()
{
    u8 key;
    if (ValidateKey(_registers[_x]))
        key = _registers[_x];
    else
        return;

    if (_key[key])
        _pc += 2;
}

void CPU::OP_EXA1()
{
    u8 key;
    if (ValidateKey(_registers[_x]))
        key = _registers[_x];
    else
        return;

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
    _index = LOWRES_FONT_START + (5 * value);
}

void CPU::OP_FX30()
{
    u8 value = _registers[_x] & 0x0F;
    _index = HIRES_FONT_START + (10 * value);
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

void CPU::OP_FX75()
{
    u8 maxR = std::min<u8>(_x, 7);
    for (u8 r = 0; r <= maxR; r++)
        _flags[r] = _registers[r];
}

void CPU::OP_FX85()
{
    u8 maxR = std::min<u8>(_x, 7);
    for (u8 r = 0; r <= maxR; r++)
        _registers[r] = _flags[r];
}
