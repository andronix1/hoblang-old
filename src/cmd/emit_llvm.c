#include "cmd/private.h"
#include "cmd/usage.h"

void cmd_usage_emit_llvm() {
    CMD_USAGE_MAIN("emit-llvm", "<input> <output>")
}

bool cmd_parse_emit_llvm(Cmd *output, const char **args, size_t len) {
    CMD_PARSE {
        const char *arg = POP_ARG();
        if (CMD_IS_FLAG) {
            CMD_FLAGS_END;
        } else {
            CMD_POS_ARG(0, output->emit_llvm.input);
            CMD_POS_ARG(1, output->emit_llvm.output);
            CMD_POS_ARGS_END;
        }
    }
    CMD_END(2);
    return true;
}

