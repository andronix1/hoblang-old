#include "cmd/private.h"

bool cmd_parse_build_obj(Cmd *output, const char **args, size_t len) {
    CMD_PARSE {
        const char *arg = POP_ARG();
        if (CMD_IS_FLAG) {
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

