#include "PtrDeobf.h"

#include "Zydis.h"

std::string Dumper::PtrDeobf::get_ptrobf_type_str(uintptr_t text_start, size_t length)
{
    return ptrobf_type_to_str(get_ptrobf_type(text_start, length));
}

PtrObfuscationType Dumper::PtrDeobf::get_ptrobf_type(uintptr_t text_start, size_t length)
{
    ZyanU8* data = (ZyanU8*)text_start;

    // Loop over the instructions in our buffer.
    ZyanUSize offset = 0;
    ZydisDisassembledInstruction instruction;
    while (ZYAN_SUCCESS(ZydisDisassembleIntel(
        /* machine_mode:    */ ZYDIS_MACHINE_MODE_LONG_64,
        /* runtime_address: */ text_start,
        /* buffer:          */ data + offset,
        /* length:          */ length - offset,
        /* instruction:     */ &instruction
    )))
    {
        switch (instruction.info.opcode)
        {
        // SUB cases (https://youtu.be/NxSbPHctke4)
        case 0x2B: //SUB r32, r/m32
            return PtrObfuscationType::SUB_O_P; // b - *b
        case 0x29:  //SUB r/m32, r32
            return PtrObfuscationType::SUB_P_O; // *b - b
        case 0x03: //ADD r32, r/m32
            return PtrObfuscationType::ADD;
        case 0x33: //XOR r32, r/m32
            return PtrObfuscationType::XOR;
        default:
            break;
        }
        offset += instruction.info.length;
    }
    
    return PtrObfuscationType::NONE;
}
