#include "core/slice.h"

typedef enum {
    X86_REG_8BIT,
    X86_REG_16BIT,
    X86_REG_32BIT,
    X86_REG_64BIT,
} X86RegSize;

typedef struct {
    Slice name;
    bool is_64_bit;
    X86RegSize size;
} X86RegInfo;

#define X86(_name, _size) { .name = slice_from_const_cstr(_name), .is_64_bit = false, .size = _size }
#define X64(_name, _size) { .name = slice_from_const_cstr(_name), .is_64_bit = true, .size = _size }

#define R8(name) X86(name, X86_REG_8BIT)
#define R16(name) X86(name, X86_REG_16BIT)
#define R32(name) X86(name, X86_REG_32BIT)
#define R64(name) X64(name, X86_REG_64BIT)

const X86RegInfo regs[] = {
    R64("rax"), R64("rbx"), R64("rcx"), R64("rdx"), R64("rsp"), R64("rbp"), R64("rsi"), R64("rdi"), R64("r8"), R64("r9"), R64("r10"), R64("r11"), R64("r12"), R64("r13"), R64("r14"), R64("r15"),
    R32("eax"), R32("ebx"), R32("ecx"), R32("edx"), R32("esp"), R32("ebp"), R32("esi"), R32("edi"), R32("r8d"), R32("r9d"), R32("r10d"), R32("r11d"), R32("r12d"), R32("r13d"), R32("r14d"), R32("r15d"),
    R16("ax"), R16("bx"), R16("cx"), R16("dx"), R16("sp"), R16("bp"), R16("si"), R16("di"), R16("r8w"), R16("r9w"), R16("r10w"), R16("r11w"), R16("r12w"), R16("r13w"), R16("r14w"), R16("r15w"),
    R8("al"), R8("bl"), R8("cl"), R8("dl"), R8("spl"), R8("bpl"), R8("sil"), R8("dil"), R8("r8b"), R8("r9b"), R8("r10b"), R8("r11b"), R8("r12b"), R8("r13b"), R8("r14b"), R8("r15b"),
    R8("ah"), R8("bh"), R8("ch"), R8("dh"),
};
// float - "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7",
// xmm - "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
// ymm - "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7", "ymm8", "ymm9", "ymm10", "ymm11", "ymm12", "ymm13", "ymm14", "ymm15",
// zmm - "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7", "zmm8", "zmm9", "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15",
