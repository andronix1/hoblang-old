#pragma once

#include "core/slice/slice.h"
#include <llvm-c/Types.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct SemaLoop {
    bool is_labelled;
    Slice label;

    struct {
        LLVMBasicBlockRef begin;
        LLVMBasicBlockRef end;
    } llvm;
} SemaLoop;

static inline SemaLoop *sema_loop() {
    SemaLoop *result = malloc(sizeof(SemaLoop));
    result->is_labelled = false;
    return result;
}

static inline SemaLoop *sema_loop_named(Slice name) {
    SemaLoop *result = malloc(sizeof(SemaLoop));
    result->is_labelled = true;
    result->label = name;
    return result;
}
