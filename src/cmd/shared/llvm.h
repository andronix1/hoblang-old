#pragma once

#include "cmd/cmd.h"

#define CMD_LLVM_FLAGS(output) { \
        CMD_FLAG_STR("target", (output).target); \
        CMD_FLAG_ON("no-verify", (output).no_verify); \
    }

void cmd_usage_llvm_flags();
