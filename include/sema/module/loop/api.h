#pragma once

#include <stdbool.h>
#include "sema/module.h"
#include "core/location.h"
#include "core/slice.h"
#include "sema/module/loop/loop.h"

SemaLoop *sema_loop_new_named(Slice name);
SemaLoop *sema_loop_new();

bool sema_module_push_loop(SemaModule *sema, FileLocation at, SemaLoop *loop);
void sema_module_pop_loop(SemaModule *sema);
SemaLoop *sema_module_top_loop(SemaModule *sema, FileLocation at);
SemaLoop *sema_module_named_loop(SemaModule *sema, FileLocation at, Slice *name);
