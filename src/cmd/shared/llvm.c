#include "cmd/usage.h"

void cmd_usage_llvm_flags() {
    CMD_USAGE_FLAG("--target <name>", "sets the llvm target");
    CMD_USAGE_FLAG("--no-verify", "disable llvm module verification");
}
