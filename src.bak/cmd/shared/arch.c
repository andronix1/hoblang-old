#include "cmd/usage.h"

void cmd_usage_arch_flags() {
    CMD_USAGE_FLAG("--mode32", "switches project to 64-bit mode");
    CMD_USAGE_FLAG("--mode16", "switches project to 16-bit mode");
    CMD_USAGE_FLAG("--mode8", "switches project to 8-bit mode");
}
