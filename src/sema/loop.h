#pragma once

#include "ast/interface/body.h"
#include "core/slice/slice.h"
#include <llvm-c/Types.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct SemaLoop {
    bool is_labelled;
    Slice label;
    AstBody *body;

    struct {
        LLVMBasicBlockRef begin;
        LLVMBasicBlockRef end;
    } llvm;
} SemaLoop;

static inline SemaLoop *sema_loop(AstBody *body) {
    SemaLoop *result = malloc(sizeof(SemaLoop));
    result->is_labelled = false;
    result->body = body;
    return result;
}

static inline SemaLoop *sema_loop_named(AstBody *body, Slice name) {
    SemaLoop *result = malloc(sizeof(SemaLoop));
    result->is_labelled = true;
    result->label = name;
    result->body = body;
    return result;
}
