#include "cmd/private.h"
#include "cmd/usage.h"

void cmd_usage_help() {
    CMD_USAGE_MAIN("help", "")
}

bool cmd_parse_help(
    Cmd *output __attribute__((unused)),
    const char **args __attribute__((unused)),
    size_t len __attribute__((unused))
) {
    CMD_PARSE {
        const char *arg = POP_ARG();
    }
    CMD_END(0);
    return true;
}

