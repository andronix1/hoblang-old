#pragma once

#include "cmd/cmd.h"

#define CMD_ARCH_FLAGS(output) { \
        CMD_FLAG_SET("mode32", (output).bits, ARCH_MODE_32); \
        CMD_FLAG_SET("mode16", (output).bits, ARCH_MODE_16); \
        CMD_FLAG_SET("mode8", (output).bits, ARCH_MODE_8); \
    }

void cmd_usage_arch_flags();
