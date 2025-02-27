#include "cmd/shared/arch.h"
#include "cmd/private.h"
#include "cmd/usage.h"

void cmd_usage_build_obj() {
    CMD_USAGE_MAIN("build-obj", "<input> <output>")
    cmd_usage_arch_flags();
}

bool cmd_parse_build_obj(Cmd *output, const char **args, size_t len) {
    CMD_PARSE {
        const char *arg = POP_ARG();
        if (CMD_IS_FLAG) {
            CMD_ARCH_FLAGS(output->build_obj.arch);
            CMD_FLAGS_END;
        } else {
            CMD_POS_ARG(0, output->build_obj.input);
            CMD_POS_ARG(1, output->build_obj.output);
            CMD_POS_ARGS_END;
        }
    }
    CMD_END(2);
    return true;
}

