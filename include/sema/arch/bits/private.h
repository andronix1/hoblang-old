#pragma once

#include "bits.h"
#include "sema/type.h"
#include "sema/module.h"

#define SEMA_ARCH(name) SemaType *sema_arch_##name(SemaModule *sema);

SEMA_ARCH(ptr)
SEMA_ARCH(usize)
SEMA_ARCH(int)
SEMA_ARCH(float)
