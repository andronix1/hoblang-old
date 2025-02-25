#include <malloc.h>
#include "core/slice/api.h"
#include "core/location.h"
#include "core/vec.h"
#include "sema/module/loop/api.h"
#include "sema/module/impl.h"
#include "sema/module/loop/loop.h"
#include "sema/module/loop/private.h"
#include "sema/module/private.h"

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

inline SemaLoop *sema_module_try_named_loop(SemaModule *sema, Slice *name) {
    for (size_t i = 0; i < vec_len(sema->loops); i++) {
        SemaLoop *loop = sema->loops[i];
        if (loop->is_named && slice_eq(name, &loop->name)) {
            return loop;
        }
    }
    return NULL;
}

bool sema_module_push_loop(SemaModule *sema, FileLocation at, SemaLoop *loop) {
    if (loop->is_named) {
        if (sema_module_try_named_loop(sema, &loop->name)) {
            SEMA_ERROR(at, "redefenition of named loop `{slice}`", &loop->name);
            return false;
        }
    }
    sema->loops = vec_push(sema->loops, &loop);
    return true;
}

void sema_module_pop_loop(SemaModule *sema) {
    vec_pop(sema->loops);
}

SemaLoop *sema_module_top_loop(SemaModule *sema, FileLocation at) {
    if (vec_len(sema->loops) == 0) {
        SEMA_ERROR(at, "no loops found");
        return NULL;
    }
    return *vec_top(sema->loops);
}

SemaLoop *sema_module_named_loop(SemaModule *sema, FileLocation at, Slice *name) {
    SemaLoop *trial = sema_module_try_named_loop(sema, name);
    if (!trial) {
        SEMA_ERROR(at, "named loop {slice} is undefined", name);
    }
    return trial;
}
