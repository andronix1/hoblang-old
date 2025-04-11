#pragma once

#include <stdbool.h>
#include <malloc.h>
#include "core/slice/slice.h"
#include "sema/interface/loop.h"

typedef struct {
    bool is_labelled;
    Slice label;

    struct {
        SemaLoop *loop;
    } sema;
} AstLoopControl;

typedef AstLoopControl AstBreak;
typedef AstLoopControl AstContinue;

static inline AstLoopControl *ast_loop_control_new_labelled(Slice label) {
    AstLoopControl *result = malloc(sizeof(AstLoopControl));
    result->is_labelled = true;
    result->label = label;
    return result;
}

static inline AstLoopControl *ast_loop_control_new() {
    AstLoopControl *result = malloc(sizeof(AstLoopControl));
    result->is_labelled = false;
    return result;
}

