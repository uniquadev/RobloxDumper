#include "PtrDeobf.h"
#include "Zydis.h"

#include <vector>
#include <algorithm>

std::string Dumper::PtrDeobf::get_ptrobf_type_str(uintptr_t text_start, size_t length)
{
    return ptrobf_type_to_str(get_ptrobf_type(text_start, length));
}


PtrObfPosition get_ptrpos(ZydisRegister left, ZydisRegister right, uintptr_t text_start, size_t lenght)
{
    ZyanU8* data = (ZyanU8*)text_start;

    ZyanUSize offset = 0;
    ZydisDisassembledInstruction instruction;
    while (ZYAN_SUCCESS(ZydisDisassembleIntel(
        ZYDIS_MACHINE_MODE_LONG_COMPAT_32,
        text_start,
        data + offset,
        lenght - offset,
        &instruction
    )))
    {
        offset += instruction.info.length;
        if (instruction.info.opcode == 0x8D) // we look for lea operations on left or right registers
        {
            auto operand = instruction.operands[0];
            if (operand.type != ZYDIS_OPERAND_TYPE_REGISTER)
                continue;
            if (operand.reg.value == left)
                return PtrObfPosition::LEFT;
            else if (operand.reg.value == right)
                return PtrObfPosition::RIGHT;
        }
        else if (instruction.info.opcode == 0x8B)
        {
            // check if mov a, [b] case
            if (instruction.operands[0].type != ZYDIS_OPERAND_TYPE_REGISTER || instruction.operands[1].type != ZYDIS_OPERAND_TYPE_MEMORY)
                continue;
            auto reg = instruction.operands[0].reg.value;
            if (reg == left)
                return PtrObfPosition::LEFT;
            else if (reg == right)
                return PtrObfPosition::RIGHT;
        }
    }
    return PtrObfPosition::NONE_POSITION;
}

PtrObfuscationType Dumper::PtrDeobf::get_ptrobf_type(uintptr_t text_start, size_t length)
{
    ZyanU8* data = (ZyanU8*)text_start;

    std::vector<ZydisRegister> holding_imm = {};

    // Loop over the instructions in our buffer.
    ZyanUSize offset = 0;
    ZydisDisassembledInstruction instruction;
    while (ZYAN_SUCCESS(ZydisDisassembleIntel(
        /* machine_mode:    */ ZYDIS_MACHINE_MODE_LONG_COMPAT_32,
        /* runtime_address: */ text_start,
        /* buffer:          */ data + offset,
        /* length:          */ length - offset,
        /* instruction:     */ &instruction
    )))
    {
        switch (instruction.info.opcode)
        {
        case 0xB8: // MOV, r32, imm32
        {
            holding_imm.push_back(instruction.operands[0].reg.value);
            break;
        }
        case 0x2B: //SUB r32, r/m32
        case 0x29:  //SUB r/m32, r32
        {
            // this opcode accept:
            // #1 sub     edx, esi
            // #2 sub     [edx], esi / edx, [esi]
            // #2 case is simple
            // #1 case require us to figure out which of the reg got deref
            // #2 case
            if (instruction.operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY)
                return PtrObfuscationType::SUB_P_X; // *b - b
            else if (instruction.operands[1].type == ZYDIS_OPERAND_TYPE_MEMORY)
                return PtrObfuscationType::SUB_X_P; // b - *b
            // #1 case
            auto pos = get_ptrpos(
                instruction.operands[0].reg.value,
                instruction.operands[1].reg.value,
                text_start,
                offset
            );
            if (pos == PtrObfPosition::LEFT)
                return PtrObfuscationType::SUB_P_X;
            else if (pos == PtrObfPosition::RIGHT)
                return PtrObfuscationType::SUB_X_P;

            return PtrObfuscationType::NONE_TYPE;
        }
        case 0x03: //ADD r32, r/m32
        {
            // check if add is using a 2nd operand with immediate value
            if (instruction.operands[1].type == ZYDIS_OPERAND_TYPE_REGISTER)
            {
                auto it = std::find(holding_imm.begin(), holding_imm.end(), instruction.operands[1].reg.value);
                if (it != holding_imm.end())
                    break;
            }
            return PtrObfuscationType::ADD;
        }
        case 0x33: //XOR r32, r/m32
            return PtrObfuscationType::XOR;
        default:
            break;
        }
        offset += instruction.info.length;
    }
    
    return PtrObfuscationType::NONE_TYPE;
}
