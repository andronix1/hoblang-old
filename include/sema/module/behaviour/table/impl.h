#pragma once

#include "sema/module/decls/decls.h"
#include "sema/type/type.h"
#include <llvm-c/Types.h>

typedef struct SemaBehaviourTableElement {
    SemaDecl *decl;
} SemaBehaviourTableElement;

typedef struct SemaBehaviourTable {
    int kind;

    union {
        SemaBehaviourTableElement *decl;
        struct SemaBehaviourTable **list;
    };

    SemaType *of;

    struct {
        LLVMValueRef value;
    } llvm;
} SemaBehaviourTable;

