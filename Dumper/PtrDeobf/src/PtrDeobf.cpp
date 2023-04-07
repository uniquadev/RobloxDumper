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
    ))) {
        offset += instruction.info.length;
    }
    
    return PtrObfuscationType::NONE;
}
