#include <malloc.h>
#include "core/slice/api.h"
#include "core/location.h"
#include "core/vec.h"
#include "sema/module/loop/api.h"
#include "sema/module/impl.h"
#include "sema/module/loop/loop.h"
#include "sema/module/loop/private.h"
#include "sema/module/private.h"
#include "sema/module/scopes/scope.h"
#include "sema/module/scopes/impl.h"

#define ASSERT_SS() assert(sema->current_ss, "illegal operation outside of scope stack");

SemaLoop *sema_loop_new() {
    SemaLoop *result = malloc(sizeof(SemaLoop));
    result->is_named = false;
    result->llvm.continue_block = NULL;
    result->llvm.break_block = NULL;
    return result;
}

SemaLoop *sema_loop_new_named(Slice name) {
    SemaLoop *result = sema_loop_new();
    result->is_named = true;
    result->name = name;
    return result;
}

SemaLoop *sema_module_try_named_loop(SemaModule *sema, Slice *name) {
    ASSERT_SS();
    SemaScopeStack *ss = sema->current_ss;
    for (size_t i = 0; i < vec_len(ss->loops); i++) {
        SemaLoop *loop = ss->loops[i];
        if (loop->is_named && slice_eq(name, &loop->name)) {
            return loop;
        }
    }
    return NULL;
}

bool sema_module_push_loop(SemaModule *sema, FileLocation at, SemaLoop *loop) {
    ASSERT_SS();
    SemaScopeStack *ss = sema->current_ss;
    if (loop->is_named) {
        if (sema_module_try_named_loop(sema, &loop->name)) {
            SEMA_ERROR(at, "redefenition of named loop `{slice}`", &loop->name);
            return false;
        }
    }
    ss->loops = vec_push(ss->loops, &loop);
    return true;
}

void sema_module_pop_loop(SemaModule *sema) {
    ASSERT_SS();
    SemaScopeStack *ss = sema->current_ss;
    vec_pop(ss->loops);
}

SemaLoop *sema_module_top_loop(SemaModule *sema, FileLocation at) {
    ASSERT_SS();
    SemaScopeStack *ss = sema->current_ss;
    if (vec_len(ss->loops) == 0) {
        SEMA_ERROR(at, "no loops found");
        return NULL;
    }
    return *vec_top(ss->loops);
}

SemaLoop *sema_module_named_loop(SemaModule *sema, FileLocation at, Slice *name) {
    SemaLoop *trial = sema_module_try_named_loop(sema, name);
    if (!trial) {
        SEMA_ERROR(at, "named loop {slice} is undefined", name);
    }
    return trial;
}
