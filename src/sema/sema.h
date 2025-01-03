#pragma once

#include "core/log.h"
#include "parser/ast.h"
#include "base.h"

#define sema_log(level, fmt, ...) hob_log(level, fmt, ##__VA_ARGS__)
#define sema_err(fmt, ...) \
	do { \
		sema_log(LOGE, fmt, ##__VA_ARGS__); \
		sema->failed = true; \
	} while (0)

void sema_module(Sema *sema, AstModule *module);
