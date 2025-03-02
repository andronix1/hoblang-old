#pragma once

#include "cmd/cmd.h"

#define CMD_LLVM_FLAGS(output) { \
        CMD_FLAG_STR("target", (output).target); \
    }

void cmd_usage_llvm_flags();
