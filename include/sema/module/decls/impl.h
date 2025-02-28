#pragma once

#include <llvm-c/Types.h>
#include <stdbool.h>
#include <llvm-c/Core.h>
#include "core/slice.h"
#include "sema/type.h"
#include "ast/api/expr.h"
#include "sema/module.h"
#include "sema/value/value.h"

typedef struct SemaDecl {
    Slice name;
    SemaType *in_type;
    SemaValue *value;

    struct {
        LLVMValueRef value;
    } llvm;
} SemaDecl;

