#pragma once

#include <llvm-c/Core.h>
#include <stdbool.h>
#include "core/slice.h"
#include "loop.h"

typedef struct SemaLoop {
    bool is_named;
    Slice name;

    struct {
        LLVMBasicBlockRef continue_block;
        LLVMBasicBlockRef break_block;
    } llvm;
} SemaLoop;
